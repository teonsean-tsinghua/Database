CREATE DATABASE test;
USE test;
CREATE DATABASE test2;
USE test2;
CREATE TABLE test2 (a INT NOT NULL, b INT NOT NULL, c DATE NOT NULL, d VARCHAR(255), PRIMARY KEY(a, b, c));
USE test;
CREATE TABLE test (a INT, b FLOAT NOT NULL, PRIMARY KEY(b));
DESC test;
USE test2;
DESC test2;
DROP TABLE test2;
DROP DATABASE test2;
DROP DATABASE test;
