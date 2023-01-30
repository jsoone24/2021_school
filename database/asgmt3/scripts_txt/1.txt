CREATE VIEW all_customer AS
    SELECT 
        branch_name, customer_name
    FROM
        customer c
            INNER JOIN
        borrower b ON (c.customer_id = b.customer_id)
            INNER JOIN
        loan_branch l ON (b.loan_number = l.loan_number)