SELECT 
    branch_name
FROM
    branch b
WHERE
    b.assets >some (SELECT 
            assets
        FROM
            branch)
        AND b.branch_city = '부산'