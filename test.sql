USE test;
DESC customer;
SELECT idd FROM customer WHERE publisher='Monday Morning Books';
SELECT gender, id FROM customer WHERE publisher='Monday Morning Books';
SELECT customer.gender, customer.id FROM customer WHERE publisher='Monday Morning Books';
SELECT customers.gender, customer.id FROM customers WHERE publisher='Monday Morning Books';

