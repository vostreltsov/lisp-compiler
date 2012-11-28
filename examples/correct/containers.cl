(setf mahList (list 1 2 3))
(setf mahVector (setf tmp (vector 4 5 6)))	; to be sure that a = b = c = ... works.
(print "Here's a list             : " mahList "\n")
(print "Here's a vector           : " mahVector "\n")
(print "The 3rd element of list   : " (elt mahList 2) "\n")
(print "The 3rd element of vector : " (elt mahVector 2) "\n")
