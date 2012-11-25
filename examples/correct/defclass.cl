#| dis iz da clazz lol |#

(defclass LolCat ()
    ((Pew :accessor doPew :initform "pew")
     (Meow :accessor doMeow :initform "meeeeooow")))

(setf mahCat (make-instance 'LolCat))
(setf (slot-value mahCat 'Pew) "PEW!!!11")
(print (slot-value mahCat 'Pew))
