SELECT 
    branch_name
FROM
    branch
WHERE
    branch.assets > (SELECT 
            AVG(assets)
        FROM
            branch);