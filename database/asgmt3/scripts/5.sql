CREATE VIEW customer_avg_balance (customer_id , customer_name , avg_balance) AS
    SELECT 
        c.customer_id, c.customer_name, AVG(IFNULL(a.balance, 0))
    FROM
        customer c
            INNER JOIN
        depositor d ON (c.customer_id = d.customer_id)
            INNER JOIN
        account a ON (d.account_number = a.account_number)
    GROUP BY customer_id , customer_name