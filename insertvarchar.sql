CREATE DATABASE test;
USE test;
CREATE TABLE test (a INT, b INT, c DATE, d VARCHAR(255), PRIMARY KEY(d));
DESC test;
CREATE INDEX test (c);
INSERT INTO test VALUES (1,1,'20150101','1'), (2,2,'20130101','ewbcionaodna'), (3,3,'20170101','this should be the last.'), (5,5,'20150536','0');
DROP DATABASE test;
