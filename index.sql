CREATE DATABASE test;
USE test;
CREATE TABLE test (a INT, b INT NOT NULL);
INSERT INTO test VALUES (1, 1), (2, 2), (3, 3), (4, 4), (9465, 5), (-156, 7), (-999, -89156), (81, 156431), (789465, 1326544);
CREATE INDEX test (b);
DROP DATABASE test;
