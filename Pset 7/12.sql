-- write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Johnny Depp.
-- You may assume that there is only one person in the database with the name Helena Bonham Carter.

-- idea is to obtain the intersection between the movie_id of Johnny and that of Helena
SELECT title FROM movies WHERE id in (SELECT J.movie_id
FROM (SELECT stars.movie_id FROM stars JOIN people ON stars.person_id = people.id WHERE people.name = "Johnny Depp") AS J
JOIN (SELECT stars.movie_id FROM stars JOIN people ON stars.person_id = people.id WHERE people.name ="Helena Bonham Carter") AS H
ON J.movie_id = H.movie_id);