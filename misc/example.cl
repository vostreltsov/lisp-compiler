(defclass Myclass ()
    ((Width :accessor Width :initform 200)
     (Height :accessor Height :initform 100)))

#| dis iz da
   comment lol |#

(defun random ()
    4)

(defun BubbleSort (v)
    (loop for i from 0 to (- (length v) 1) do
        (loop for j from (+ i 1) to (- (length v) 1) do
            (if (> (elt v i) (elt v j))
                (progn (setf tmp (elt v j))
                       (setf(elt v j) (elt v i))
                       (setf(elt v i) tmp)))))
    (print "bubble sort done"))


(setf |{0,0}| (vector 7 15 1))
(BubbleSort |{0,0}|)
(print |{0,0}|)
kinda_weird_symbol_azAZ0-9+-*/@$%^&_=<>~.