/* Program1 for a list within the list counts as one element
Basic case is an empty list, count will be 0
Recursively trace the list until it reach the basic case
Each time add 1 to the count */
count([_ | T], N) :- count(T, N1), N is N1 + 1.
count([], 0).

/* 
Program 2 for a list that counts the elements with any sublist
Basic case: 1. if the list is empty, return 0
2. if there's no list, only an element, return 1
Assume all members in a list are sublists
then trace the head sublist until it reach the basic case
Do same thing for the remaining of the list until the list is empty
Using ! to prevent backtracking (hint from CS351 notes)
*/
count_all([H | T], N) :- count_all(H, N1), count_all(T,N2), !, N is N1 + N2.
count_all([],0).
count_all(_,1).
