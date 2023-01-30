SELECT 
    c.customer_id, AVG(a.balance)
FROM
    customer AS c
        INNER JOIN
    depositor AS d ON (c.customer_id = d.customer_id)
        INNER JOIN
    account AS a ON (d.account_number = a.account_number)
WHERE
    c.customer_city = '서울'
GROUP BY c.customer_id
HAVING COUNT(a.account_number) >= 3