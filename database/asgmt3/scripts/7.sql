DELIMITER  //

CREATE FUNCTION get_loan_counts(customer_id varchar(50)) RETURNS INT
DETERMINISTIC
BEGIN
	DECLARE rval INT	DEFAULT 0;
	SELECT	COUNT(*)	INTO rval
    FROM	borrower
	WHERE	borrower.customer_id = customer_id;
	RETURN rval;
END//

DELIMITER  ;