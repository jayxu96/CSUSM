%define 8 basic moves
moves((X,Y),(NextX,NextY)):- NextX is X-1, NextY is Y-2.%Left 1, up 2
moves((X,Y),(NextX,NextY)):- NextX is X-2, NextY is Y-1.%Left 2, up 1
moves((X,Y),(NextX,NextY)):- NextX is X-1, NextY is Y+2.%Left 1, down 2
moves((X,Y),(NextX,NextY)):- NextX is X-2, NextY is Y+1.%Left 2, down 1
moves((X,Y),(NextX,NextY)):- NextX is X+1, NextY is Y-2.%Right 1, up 2
moves((X,Y),(NextX,NextY)):- NextX is X+2, NextY is Y-1.%Right 2, up 1
moves((X,Y),(NextX,NextY)):- NextX is X+1, NextY is Y+2.%Right 1, down 2
moves((X,Y),(NextX,NextY)):- NextX is X+2, NextY is Y+1.%Right 2, down 1

path((X,Y),(GoalX,GoalY)):- path((X,Y),(GoalX,GoalY),[(X,Y)]).

path((X,Y),(X,Y),L):- !, printlist(L). %basic case,reach goal, print 
path((X,Y),(GoalX,GoalY),L):- moves((X,Y),(NextX,NextY)), %move one square
    NextX>0, NextX=<8, %check if the square is valid
    NextY>0, NextY=<8,
    not(member((NextX,NextY),L)), % check if the square is not visited
    path((NextX,NextY),(GoalX,GoalY),[(NextX,NextY)|L]).% recursive find next square

%print the path
printlist([]).
printlist([H|T]):- printlist(T),nl,write(H).