SELECT DISTINCT
    customer_id, customer_name
FROM
    (SELECT DISTINCT
        customer_id, customer_name
    FROM
        customer
    INNER JOIN depositor USING (customer_id)) AS de
WHERE
    de.customer_id NOT IN (SELECT 
            customer_id
        FROM
            borrower);