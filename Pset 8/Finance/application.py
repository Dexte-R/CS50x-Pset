import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import pandas as pd

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # get total number of shares for each type of share
    rows = db.execute("SELECT symbol, SUM(shares) AS counts FROM purchases WHERE purchase_id = :current GROUP BY symbol  HAVING counts > 0", current=session["user_id"])

    share_value = 0
    for row in rows:
        info = lookup(row["symbol"])
        row["name"] = info["name"]
        # display current price of share to user
        row["price"] = info["price"]
        row["total"] = row["counts"] * row["price"]
        share_value += row["total"]

        # format share value to display 2dp
        row["total"] = "{:.2f}".format(row["total"])


    cash = db.execute("SELECT cash FROM users WHERE id = :current", current=session["user_id"])[0]["cash"]
    total = "{:.2f}".format(cash + share_value)

    return render_template("index.html", data=rows, cash="{:.2f}".format(cash), total=total)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # check for empty fields
        if not symbol or not shares:
            return apology("all fields mandatory")

        info = lookup(symbol)
        # check if symbol(stock) exists
        if info == None:
            return apology("Stock does not exists")

        # check if #shares is positive
        if int(shares) < 1:
            return apology("buy at least 1 share")

        # lookup price of shares
        price = info["price"]

        # obtain user data (SQL query returns a dict object)
        row = db.execute("SELECT cash FROM users WHERE id = :current", current=session["user_id"])
        # otain cash available
        cash = row[0]["cash"]

        # check if user has enough cash to buy #shares
        total = int(shares) * price
        if total > cash:
            return apology("Need more cash")
        else:
            # purchase successful; update user's cash
            new_cash = cash - total
            db.execute("UPDATE users SET cash = :val1 WHERE id = :val2", val1=new_cash, val2=session["user_id"])

        # insert transaction into 'purchases' table
        db.execute("INSERT INTO purchases (purchase_id, symbol, shares, price) VALUES (:val1, :val2, :val3, :val4)", val1=session["user_id"], val2=info["symbol"], val3=shares, val4=info["price"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    # obtain user's transaction history
    rows = db.execute("SELECT * FROM purchases WHERE purchase_id = :current", current=session["user_id"])

    return render_template("history.html", data=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # check if symbol is blank
        if not symbol:
            return apology("must provide symbol")

        info = lookup(symbol)

        # check if symbol(stock) exists
        if info == None:
            return apology("Stock does not exists")
        else:
            display = "A share of {} ({}) costs ${}.".format(info["name"], info["symbol"], info["price"])
            return render_template("quoted.html", quoted=display)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    # Forget any user_id
    session.clear()

    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        # obtain user inpts
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        filled = username or password or confirmation
        # group the checks by logic.
        # check for empty forms
        # Ensure username was submitted
        if not filled:
            return apology("all fields mandatory", 403)

        # check if username already exist
        db_username = db.execute("SELECT username FROM users")
        if username in db_username:
            return apology("Username already exists.")

        # check if password matches "confirm password"
        if password != confirmation:
            return apology("Passwords do not match.")

        # all checks passed
        # hash password
        hashed = generate_password_hash(password)
        # add new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (:val1, :val2)", val1=username, val2=hashed)
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # get total number of shares for each type of share
    rows = db.execute("SELECT symbol, SUM(shares) AS counts FROM purchases WHERE purchase_id = :current GROUP BY symbol HAVING counts > 0", current=session["user_id"])

    # display dropdown list of stocks that user owns
    if request.method == "GET":
        return render_template("sell.html", data=rows)

    else:
        symbol = request.form.get("symbol")
        selling = request.form.get("shares")

        # check if empty
        if not symbol:
            return apology("which shares to be sold?")

        if not selling:
            return apology("how many shares to be sold?")

        # check if shares to be sold is positive
        if int(selling) < 1:
            return apology("At least 1 share needed")

        # prohibit user form selling more shares than owned
        for dictionary in rows:
            if dictionary["symbol"] == symbol:
                shares_owned = dictionary["counts"]
        if int(selling) > shares_owned:
            return apology("cannot sell more shares than owned")

        # perform transaction
        info = lookup(symbol)
        price = info["price"]
        sale = price * int(selling)

        # obtain user data (SQL query returns a dict object)
        row = db.execute("SELECT cash FROM users WHERE id = :current", current=session["user_id"])
        # otain cash available
        cash = row[0]["cash"]

        # update user cash
        new_cash = cash + sale
        db.execute("UPDATE users SET cash = :val1 WHERE id = :val2", val1=new_cash, val2=session["user_id"])

        # record transaction
        db.execute("INSERT INTO purchases (purchase_id, symbol, shares, price) VALUES (:val1, :val2, :val3, :val4)", val1=session["user_id"], val2=symbol, val3 = -int(selling), val4=price)

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
