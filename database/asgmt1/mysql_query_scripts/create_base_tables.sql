CREATE TABLE customer (
    customer_id VARCHAR(50) PRIMARY KEY,
    customer_name VARCHAR(50),
    customer_street VARCHAR(50),
    customer_city VARCHAR(50)
);

CREATE TABLE loan (
    loan_number VARCHAR(50) PRIMARY KEY,
    amount INT
);

CREATE TABLE account (
    account_number VARCHAR(50) PRIMARY KEY,
    balance INT CHECK (0 <= balance <= 150000)
);

CREATE TABLE branch (
    branch_name VARCHAR(50) PRIMARY KEY,
    branch_city VARCHAR(50),
    assets INT(50)
);