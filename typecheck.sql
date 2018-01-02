CREATE DATABASE test;
USE test;
CREATE TABLE test (a INT NOT NULL, b FLOAT NOT NULL, c DATE NOT NULL, d VARCHAR(50) NOT NULL, PRIMARY KEY(d));
DESC test;
CREATE INDEX test (c);
INSERT INTO test VALUES (1,1,'2015/01/01','1'), ('2',2.0,'2013/01/01','ewbcionaodna'), (3,3.0,'2017001001','this should be the last.'), (5,5.5,'2012/02/29','0');
SELECT * FROM test WHERE a > 0;
DROP DATABASE test;
