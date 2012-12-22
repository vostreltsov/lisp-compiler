(defun print_shape(shape)
    (loop for row in shape do
        (loop for col in row do
            (print col " "))
        (print "\n")))

(defun fill (shape x y old_color new_color)
    (setf height (length shape))
    (setf width (length (elt shape 0)))

    (setf (elt (elt shape y) x) new_color)

    (setf left nil)
    (setf right nil)
    (setf top nil)
    (setf bottom nil)

    (if (> x 0) (setf left (elt (elt shape y) (- x 1))))
    (if (< x (- width 1)) (setf right (elt (elt shape y) (+ x 1))))
    (if (> y 0) (setf top (elt (elt shape (- y 1)) x)))
    (if (< y (- height 1)) (setf bottom (elt (elt shape (+ y 1)) x)))


    (if (and (not (= left nil)) (= left old_color))
        (fill shape (- x 1) y old_color new_color))

    (if (and (not (= right nil)) (= right old_color))
        (fill shape (+ x 1) y old_color new_color))

    (if (and (not (= top nil)) (= top old_color))
        (fill shape x (- y 1) old_color new_color))

    (if (and (not (= bottom nil)) (= bottom old_color))
        (fill shape x (+ y 1) old_color new_color))
    )

(setf shape1 (vector 1 1 1 1 1 0 0 0 0))
(setf shape2 (vector 1 1 0 1 1 1 0 1 0))
(setf shape3 (vector 1 0 1 1 1 1 1 0 0))
(setf shape4 (vector 0 1 1 1 1 0 1 1 0))
(setf shape5 (vector 1 1 0 1 1 1 1 1 1))
(setf shape6 (vector 0 1 1 1 1 1 1 1 0))
(setf shape7 (vector 0 0 1 1 1 1 1 0 0))
(setf shape8 (vector 0 0 0 1 1 1 0 0 0))
(setf shape9 (vector 0 0 0 0 1 0 0 0 0))

(setf shape (vector shape1 shape2 shape3 shape4 shape5 shape6 shape7 shape8 shape9))

(fill shape 4 4 1 2)

(print_shape shape)
