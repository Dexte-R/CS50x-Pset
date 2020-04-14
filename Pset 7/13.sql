-- write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

-- get movie_id that contains star_id of Kevin Bacon
-- get all star_id except Kevin Bacon's from that list of movie_id
SELECT name FROM people WHERE id in
(SELECT DISTINCT(person_id) FROM stars WHERE movie_id in
(SELECT movie_id FROM stars JOIN people ON stars.person_id = people.id WHERE people.id =
(SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958))
AND person_id <> (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958))

-- Step 1: find id of Kevin Bacon from people table.
-- Step 2: follow id to its reference in foreign key of stars table to obtain list of movie_id .
-- Step 3: obtain star_id from list of movie_id, excluding the star_id of Kevin Bacon.
-- Step 4: follow foreign key back to primary key in people table to obtain list of names.