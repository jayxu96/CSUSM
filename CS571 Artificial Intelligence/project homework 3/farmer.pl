cross(a,b). % cross from side a to b
cross(b,a). % cross from side b to a

move([X,X,G,C],wolf,[Y,Y,G,C]):- cross(X,Y). %cross with wolf
move([X,W,X,C],goat,[Y,W,Y,C]):- cross(X,Y). %cross with goat
move([X,W,G,X],cabbage,[Y,W,G,Y]):- cross(X,Y). %cross with cabbage
move([X,W,G,C],farmer,[Y,W,G,C]):- cross(X,Y). %farmer crosses alone

safe([F,_,G,_]):- F = G, !. % farmer with goat, then safe
safe([F,W,_,C]):- F = W, F = C. % goat is alone, then safe

path([b,b,b,b],[]). % final case: all characters are on side b
path(Start,[X|Remain]):- move(Start,X,Next),safe(Next),
  path(Next,Remain). % recursion to find such a path 

%recursively get the length of the path, the minimum one is 7 steps
pathLength(Start,L, X):- length(X,L),path(Start,X); 
                    (L<7-> L1 is L+1, pathLength(Start,L1,X)).

getPath(Start, Path):- pathLength(Start, 0, Path). % get the path

printPath([]). % print each step
printPath([farmer|Remain]):- write('Farmer crosses alone.\n'),printPath(Remain).    
printPath([wolf|Remain]):- write('Farmer crosses with wolf.\n'),printPath(Remain).
printPath([goat|Remain]):- write('Farmer crosses with goat.\n'),printPath(Remain).
printPath([cabbage|Remain]):- write('Farmer crosses with cabbage.\n'),printPath(Remain).