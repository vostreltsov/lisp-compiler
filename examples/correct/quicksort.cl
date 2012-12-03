(defun qsort(array low high)
    (setf i low)
    (setf j high)

    (if (and (>= i 0) (>= j 0)) (progn
        (setf mid (elt array (/ (+ low high) 2)))

        (loop while (< i j) do
            (loop while (< (elt array i) mid) do
                (setf i (+ i 1)))
            (loop while (> (elt array j) mid) do
                (setf j (- j 1)))
            (if (<= i j)
                (progn (setf temp (elt array i))
                       (setf (elt array i) (elt array j))
                       (setf (elt array j) temp)
                       (setf i (+ i 1))
                       (setf j (- j 1)))))

        (if (< low j)
            (qsort array low j))
        (if (< i high)
            (qsort array i high)))))

(setf a (vector))
(print a "\n")
(qsort a 0 (- (length a) 1))
(print a "\n")

(setf a (vector 1))
(print a "\n")
(qsort a 0 (- (length a) 1))
(print a "\n")

(setf a (vector 3 9 53 0 7 36 65 4 3 2 1))
(print a "\n")
(qsort a 0 (- (length a) 1))
(print a "\n")
