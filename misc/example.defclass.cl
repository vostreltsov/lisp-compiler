#| dis iz da clazz lol |#

(defclass LolCat ()
    ((Pew :accessor doPew :initform "pew")
     (Meow :accessor doMeow :initform "meeeeooow")))

(setf mahCat (make-instance 'LolCat))