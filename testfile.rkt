; (* 23 45 1.2 3.4 5.22 0 23 4.566423 23.44)
; (*)
; (* 2)
; (* 2 1.5)
; (* 2 1 2)
; (* 10 0.2 2.5)
; (* 2 1.5 4 .5)

; (/ 4.5 3.1415926535897)
; (/ 3 1.5) 
; (/ 9 3)
; (/ 6 4) 

; (modulo 5 4)
; (modulo 5 1)
; (modulo 2 45)

; (- 4.5 3.1415926535897)
; (- 3 1.5)
; (- 9 3)
; (- 6 4)
; (- 4 -6)

; (<= 5 4)
; (<= 3 4)
; (<= 3.5 4)
; (<= 3.5 4.4)
; (<= 3 3.0)
; (<= 3.5 2.4)
; (<= 3 1)

; (>= 5 4)
; (>= 3 4)
; (>= 3.5 4)
; (>= 3.5 4.4)
; (>= 3 3.0)
; (>= 3.5 2.4)
; (>= 3 1)

; (< 5 4)
; (< 3 4)
; (< 3.5 4)
; (< 3.5 4.4)
; (< 3 3.0)
; (< 3.5 2.4)
; (< 3 1)

; (> 5 4)
; (> 3 4)
; (> 3.5 4)
; (> 3.5 4.4)
; (> 3 3.0)
; (> 3.5 2.4)
; (> 3 1)

; (= 3.0 3)
; (= 2.0 5)
; (= -3 3)
; (= 2.0 2.0)
; (= 3 3.0)
; (= 2.0000005 2.0)


; (display "hello\nworld\"hello\tworld")
; (display "123456")
; (display "\n\t")

; (string-length "hello\nworld\"hello\tworld")
; (string-length "123456")
; (string-length "\n\t")

; (equal? 5 5.0)
; (equal? 5 5)
; (equal? 4 4.0)
; (equal? "yolo" "fish")
; (equal? "nootNoot" (quote nootNoot) )
; (define hullabaloo (quote (a b (c) ((d)) ((e f) g))) )
; (equal? hullabaloo (quote (a b (c) ((d)) ((e f) g))) )

; (begin 1)
; (if #t (begin 1 2 4) 0)
; (begin 1 (display "yolo") 2)
; (begin 1 (define x 3))
; (begin 10 (display "hi") (display "bye") x)
; (begin)


; (let ((is-odd?
;           (lambda (n)
;             (if (= n 0) #f
;                   (if (= n 1) #t
;                   (is-even? (- n 1))))))
;          (is-even?
;           (lambda (n)
;             (if (= n 0) #f
;                   (if (= n 1) #t
;                   (is-odd? (- n 1)))))))
;   (is-odd? 223))

; (let* ((x 5) (y x)) y)
; (let* ((x (lambda (y) (+ y 5))) (y x)) (y 4))

; (letrec ((x y)
;        (y 5))
;   x)

; ((lambda () 0))

; (letrec ((x (lambda () y) )
;        (y (lambda () 5)))
;   ((x)) )

; (letrec ((is-odd?
;           (lambda (n)
;             (if (= n 0) #f
;                   (if (= n 1) #t
;                   (is-even? (- n 1))))))
;          (is-even?
;           (lambda (n)
;             (if (= n 0) #f
;                   (if (= n 1) #t
;                   (is-odd? (- n 1)))))))
;   (is-odd? 223))



; (define case #t)
; (define statement "whee")
; (define case2 (lambda () #f))
; (define state2 "aww")
; (define elseState "nooo")

;  (cond (#f statement)
;        ((case2) state2))


; (and 1 #t 0 4 5)
; (and 1 #f 3 #t 5)
; (and)
; (and #t #f)
; (and (quote yollo))
; (and #f #f 1 2)
; (or)
; (or #f #f)
; (or #t #f)
; (or #f #f 1 2)

; (list? (quote gravy))
; (list? (quote (gravy)) )
; (list? 1)
; (list? (quote (()) ))



#| COMIN ATCHA!!! |#
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------


(define myLength (lambda (L)
                 (letrec ((lengthy (lambda (L num) (if (null? L) num (lengthy (cdr L) (+ 1 num))))))
                   (lengthy L 0)))
                 )

(define myOrmap (lambda (proc llst) 
  (letrec ((noot (lambda (L) (cond ;ala N00t N00t 
    [(null? L) #f]
    [(equal? #f (proc (car L))) (noot (cdr L))]
    [#t (proc (car L))]))))
  (noot llst))))

(define myAndmap (lambda (proc llst) 
  (letrec ((noot (lambda (L lastVal) (cond
    [(null? L) lastVal]
    [(equal? #f (proc (car L))) #f]
    [#t (noot (cdr L) (proc (car L)))]))))
  (noot llst #t))))


(define is-element-of? (lambda (a S)
                         (let ((proc (if (list? a)
                                         ;if a is a list, our procedure will be set-equal
                                         (lambda (L) (set-equal? a L))
                                         ;if a is a list, our procedure will be equal
                                         (lambda (b) (equal? a b)))))
                           (myOrmap proc S)))) ;myOrmap is lazy (will return as soon as it finds true :D )


(define set-equal?
  (lambda (x y)
    (if (list? x)  ;x must be a list
         (if (list? y) ;y must be a list
          (if (= (myLength x) (myLength y)) ;the length is equal
              (let ((f (lambda (a) (is-element-of? a y)))) ;check if each element of x is a member of y
                (myAndmap f x)) #f) #f) #f))) ;make sure x is contained in y (also myAndmap is lazy :D )


(define union (lambda (x y)
  (letrec ((union-helper (lambda (x y L) ;a helper function that allows us to pass in an initial value for L
                           ;(this way y doesnt get larger and slower as we recurse through the method)
    (cond ((null? x) ;if car(x) is not in y, add it to L
           L)
          ((is-element-of? (car x) y)
           (union-helper (cdr x) y L)) ;if car(x) is in y, it is already in L
          (#t
           (union-helper (cdr x) y (cons (car x) L))))))) ;if car(x) is not in y, add it to L
    (union-helper x y y))))

(define intersection (lambda (x y)
  (letrec ((intersection-helper (lambda (x y L) ;a helper function that allows us to pass in an initial empty list as L
    (cond ((null? x)
           L)
          ((is-element-of? (car x) y)  
           (intersection-helper (cdr x) y (cons (car x) L))) ;if car(x) is in y, it gets added to L
          (#t
           (intersection-helper (cdr x) y L)))))) ;if car(x) is not in y, it does not get placed in L  
    (intersection-helper x y (quote ()) ))))


(equal? #f (set-equal? (quote (1 (2 3 (4 5)))) (quote (((3 (5 4) 2) 1)))) )
(equal? #t (set-equal? (quote (1 (2 3 (4 5)))) (quote ((3 (5 4) 2) 1)) ))
(equal? #t (set-equal? (quote (1 (2 3))) (quote ((3 2) 1))) )               
(equal? #f (set-equal? (quote (1 2 3)) (quote ((3 2) 1))) )                 
(equal? #f (set-equal? (quote (1 2 3)) (quote ((1 2 3)))) )                 
(equal? #t (set-equal? (union (quote (1 (2) 3)) (quote (3 2 1))) (quote (1 2 3 (2)))))
(equal? #t (set-equal? (union (quote ((1 2 3))) (quote ((3 4 5)))) (quote ((1 2 3) (3 4 5)))))
(equal? #t (set-equal? (union (quote ((1 2 3))) (quote ((3 2 1)))) (quote ((1 2 3)))))
(equal? #t (set-equal? (intersection (quote ((1 2 3))) (quote ((3 2 1)))) (quote ((1 2 3)))))
(equal? #t (set-equal? (intersection (quote ((1 2 3))) (quote ((4 5 6)))) (quote ())))
(equal? #t (set-equal? (intersection (quote ((1) (2) (3))) (quote ((2) (3) (4)))) (quote ((2) (3)))))



;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------


(define FAC (lambda (f)
              (lambda (n)
                (if (= 0 n)
                    1
                    (* n (f (- n 1)))))))


(define FIX (lambda (f)
              ((lambda (x) (f (lambda (v) ((x x) v))))
               (lambda (x) (f (lambda (v) ((x x) v)))))))

((FIX FAC) 0)
((FIX FAC) 1)
((FIX FAC) 2)
((FIX FAC) 3)
((FIX FAC) 4)
((FIX FAC) 5)
((FIX FAC) 6)
