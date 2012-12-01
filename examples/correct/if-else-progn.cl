(progn (print "seeing this means ")
       (print "that progn works just fine.\n"))

(setf TRUE t)
(if TRUE (progn (print "datz tru!") (print "yeah!"))
         (print "r u kiddin' me?"))
