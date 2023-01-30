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
    balance INT CHECK (0 <= balance AND balance <= 150000)
);

CREATE TABLE branch (
    branch_name VARCHAR(50) PRIMARY KEY,
    branch_city VARCHAR(50),
    assets INT
);

CREATE TABLE payment (
    loan_number VARCHAR(50),
    payment_number VARCHAR(50),
    payment_amount INT,
    payment_date DATE,
    PRIMARY KEY (loan_number , payment_number),
    FOREIGN KEY (loan_number)
        REFERENCES loan (loan_number)
);

CREATE TABLE borrower (
    customer_id VARCHAR(50),
    loan_number VARCHAR(50),
    PRIMARY KEY (customer_id , loan_number),
    FOREIGN KEY (customer_id)
        REFERENCES customer (customer_id),
    FOREIGN KEY (loan_number)
        REFERENCES loan (loan_number)
);

CREATE TABLE depositor (
    customer_id VARCHAR(50),
    account_number VARCHAR(50),
    access_date DATE,
    PRIMARY KEY (customer_id , account_number),
    FOREIGN KEY (customer_id)
        REFERENCES customer (customer_id),
    FOREIGN KEY (account_number)
        REFERENCES account (account_number)
);

CREATE TABLE loan_branch (
    loan_number VARCHAR(50),
    branch_name VARCHAR(50),
    PRIMARY KEY (loan_number),
    FOREIGN KEY (loan_number)
        REFERENCES loan (loan_number),
    FOREIGN KEY (branch_name)
        REFERENCES branch (branch_name)
);
