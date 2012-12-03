(defun factorial(number)
	(if (= number 1)
		(progn 2 1)
		(* number (factorial (- number 1)))))

(print (factorial 5) "\n")