USE test;
DESC ints;
UPDATE ints SET a = 11 WHERE d = 1;
SELECT a, b, c, d FROM ints WHERE b <> 0;
