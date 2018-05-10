;;;; The converter of English and NATO codes
;;; author: Runyu Xu

(defun transe (N)                                        
"translate an English letter to NATO code"
    (cond ((equalp N 'A) "Alfa")
          ((equalp N 'B) "Bravo")
          ((equalp N 'C) "Charlie")
          ((equalp N 'D) "Delta")
          ((equalp N 'E) "Echo")
        ((equalp N 'F) "Foxtrot")
        ((equalp N 'G) "Golf")
        ((equalp N 'H) "Hotel")
        ((equalp N 'I) "India")
        ((equalp N 'J) "Juliet")
        ((equalp N 'K) "Kilo")
        ((equalp N 'L) "Lima")
        ((equalp N 'M) "Mike")
        ((equalp N 'N) "November")
        ((equalp N 'O) "Oscar")
        ((equalp N 'P) "Papa")
        ((equalp N 'Q) "Quebec")
        ((equalp N 'R) "Romeo")
        ((equalp N 'S) "Sierra")
        ((equalp N 'T) "Tango")
        ((equalp N 'U) "Uniform")
        ((equalp N 'V) "Victor")
        ((equalp N 'W) "Whiskey")
        ((equalp N 'X) "Xray")
        ((equalp N 'Y) "Yankee")
        ((equalp N 'Z) "Zulu")))

(defun tranline (list)
 "convert a sublist of English word into NATO code using recursion"
    (cond ((NULL list) (format t " ~a" ""))   ; if the list is empty, print blank
        (t (format t (transe(car list)))             ; translate the first letter in the list
           (tranline (cdr list)))))                     ; process recursively

(defun conve (list)
"convert a list of English phrase into NATO code using recursion"
    (cond ((NULL list) ())                   ; if the list is empty, do nothing
        (t (tranline(car list))                   ; convert the fisrt sublist
           (conve (cdr list)))))                ; read sublist recursively

(defun transn (N)
"translate a NATO code to English letter "
    (cond ((equalp N 'Alfa) "a")
          ((equalp N 'Bravo) "b")
          ((equalp N 'Charlie) "c")
          ((equalp N 'Delta) "d")
          ((equalp N 'Echo) "e")
        ((equalp N 'Foxtrot) "f")
        ((equalp N 'Golf) "g")
        ((equalp N 'Hotel) "h")
        ((equalp N 'India) "i")
        ((equalp N 'Juliet) "j")
        ((equalp N 'Kilo) "k")
        ((equalp N 'Lima) "l")
        ((equalp N 'Mike) "m")
        ((equalp N 'November) "n")
        ((equalp N 'Oscar) "o")
        ((equalp N 'Papa) "p")
        ((equalp N 'Quebec) "q")
        ((equalp N 'Romeo) "r")
        ((equalp N 'Sierra) "s")
        ((equalp N 'Tango) "t")
        ((equalp N 'Uniform) "u")
        ((equalp N 'Victor) "v")
        ((equalp N 'Whiskey) "w")
        ((equalp N 'Xray) "x")
        ((equalp N 'Yankee) "y")
        ((equalp N 'Zulu) "z")))

(defun tranlist (list)
"convert a sublist of  NATO codes into English word using recursion"
    (cond ((NULL list) (format t " ~a" ""))      ; if the list is empty, print blank
        (t (format t (transn(car list)))                ; translate the first letter in the list
           (tranlist(cdr list)))))                           ; process recursively

(defun convn (list)
"convert a list of NATO codes into English phrase using recursion"
    (cond ((NULL list) ())                             ; if the list is empty, do nothing
        (t (tranlist(car list))                              ; convert the fisrt sublist
           (convn (cdr list)))))                          ; read sublist recursively


(print "This translator developed by Runyu Xu")
(setf code '((A Alfa) (B Bravo) (C Charlie) (D Delta) (E Echo) (F Foxtrot) (G Golf) (H Hotel) (I India) (J Juliet) (K Kilo) (L Lima) (M Mike) (N November) (O Oscar) (P Papa) (Q Quebec) (R Romeo) (S Sierra) (T Tango) (U Uniform) (V Victor) (W Whiskey) (X Xray) (Y Yankee) (Z Zulu)))
(print "using this table below to input")
(print code)