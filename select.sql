CREATE DATABASE test;
USE test;
CREATE TABLE test (a INT, b INT);
INSERT INTO test VALUES (1, 1), (2, 2), (3, 3), (4, 4), (9465, 5), (-156, 7), (-999, -89156);
SELECT * FROM test WHERE a<1 AND b = 4;
DROP DATABASE test;
