(defun BubbleSort (v)
    (loop for i from 0 to (- (length v) 1) do
        (loop for j from (+ i 1) to (- (length v) 1) do
            (if (> (elt v i) (elt v j))
                (progn (setf tmp (elt v j))
                       (setf(elt v j) (elt v i))
                       (setf(elt v i) tmp))))))


(setf |{0,0}| (vector 7 15 1 4 16 8 15))
(BubbleSort |{0,0}|)
(print |{0,0}| "\n")
