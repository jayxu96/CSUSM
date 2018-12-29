%facts
father(i,s1).
father(f,i).
father(f,s2).
mother(w,d).
mother(w,s1).
mother(d,s2).
couple(i,w).
couple(f,d).

%rules
step_father(X,Y):-couple(X,Z),mother(Z,Y),not(father(X,Y)).
father_in_law(X,Y):- step_father(X,Z),couple(Y,Z).
brother(Y,Z):- father(X,Y),father(X,Z).
grandfather(X,Z):- father(X,Y),father(Y,Z);step_father(X,Y),mother(Y,Z);
father_in_law(X,Y),father(Y,Z),brother(X,Z).