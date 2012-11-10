(setf li (list "a" "b" "c" 1 2 3))

(print "letters are:")
(loop for i from 0 to 2 do
	(print (elt li i)))

(print "container is:")
(loop for ch in li do
	(print ch))