SELECT DISTINCT
    s.customer_id, s.customer_name
FROM
    (SELECT 
        l.branch_name, c.customer_id, c.customer_name
    FROM
        customer AS c
    INNER JOIN borrower AS b ON (c.customer_id = b.customer_id)
    INNER JOIN loan_branch AS l ON (b.loan_number = l.loan_number)) AS s
        LEFT JOIN
    (SELECT 
        branch_name
    FROM
        branch
    WHERE
        branch_city = '서울') AS t ON (t.branch_name = s.branch_name)
WHERE
    t.branch_name IS NOT NULL