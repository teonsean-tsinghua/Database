USE test;
DESC customer;
SELECT gender, id FROM customer WHERE publisher=1;
SELECT customer.gender, customer.id FROM customer WHERE publisher=2 AND customer.id=publisher AND gender IS NOT NULL AND id IS NULL;

