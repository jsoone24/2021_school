CREATE VIEW only_account_customer(customer_name, customer_city) AS
    SELECT DISTINCT
        customer_name, customer_city
    FROM
        customer c
            INNER JOIN
        depositor d ON (c.customer_id = d.customer_id)
            LEFT JOIN
        borrower b ON (c.customer_id = b.customer_id)
    WHERE
        loan_number IS NULL