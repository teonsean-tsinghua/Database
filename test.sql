USE test;
CREATE TABLE primKeyTest (a INT, b INT, c INT, d INT, PRIMARY KEY(c, d), FOREIGN KEY (d) REFERENCES a(b), FOREIGN KEY (c) REFERENCES a(b));
DESC primKeyTest;
DROP TABLE primKeyTest;
