SELECT 
    customer_id, customer_city
FROM
    customer c
WHERE
    GET_LOAN_COUNTS(c.customer_id) >= 2; 