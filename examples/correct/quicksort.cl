(setf y 13)
(loop while (>= y 10) do
	(print "one more iter " y "\n")
	(setf y (- y 1)))