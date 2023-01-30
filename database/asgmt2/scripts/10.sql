UPDATE customer c 
SET 
    customer_type = 'VIP'
WHERE
    10000 <= (SELECT 
            SUM(balance)
        FROM
            account AS a
                INNER JOIN
            depositor AS d ON (a.account_number = d.account_number)
        WHERE
            c.customer_id = d.customer_id);
            
select * from customer