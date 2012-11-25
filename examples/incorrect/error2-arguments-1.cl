;wrong argumnets in functions elt
(defun helpMeWatman()(print "WAT?"))
(setf (vector 1 2 3))
(elt v (funcall helpMeWatman)))