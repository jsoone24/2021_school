SELECT 
    SUM(amount)
FROM
    (SELECT 
        loan_number
    FROM
        (SELECT 
        customer_id
    FROM
        customer
    WHERE
        customer_name LIKE '김%') AS ci
    INNER JOIN borrower USING (customer_id)) AS ln
        INNER JOIN
    loan USING (loan_number)