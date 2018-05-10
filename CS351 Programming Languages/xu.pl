% Project: Prolog assignment
% Author: Runyu Xu
% 1. use whoSatisfied(Student) rule to find the students who have satisfied both 
% core and elective courses requirements;
% 2. use passCoreList(List,Student) rule to find which core courses are fulfilled by 
% a student
% 3. use passElectList(List,Student) rule to find which elective courses are fulfilled
% by a student
% 4. use whoPassedCore(Student) rule to find who has satisfied core course requirements
% 5. use whoPassedEle(Student) rule to find who has satisfied elective course requirements
% 6. use passCore(Student,Course) / passElect(Student,Course) rule to find which courses are passed by a student or who has passed this course
% 7. use coreFulfill(Student) / elective(Student) rule to check if a student fulfill the core or elective requirements

%facts
%%students
student(homer).
student(ada).
student(troy).
student(gina).
student(marty).
% student list
% return a list of all student
studentList(List):- findall(Name,student(Name),List).

% courses
courses(cs, cs311).
courses(cs, cs211).
courses(cs, cs111).
courses(cs, cs315).
courses(cs, cs343).
courses(cs, cs436).
courses(math,math270).
courses(math,math242).
courses(math,math372).
courses(math,math480).
courses(phys,phys201).
courses(phys,phys301).

% enrolled courses
enrolled(troy,cs311).
enrolled(troy,cs436).
enrolled(troy,math372).

enrolled(homer,math480).
enrolled(homer,cs343).
enrolled(homer,phys301).

enrolled(ada,cs436).
enrolled(ada,cs343).

enrolled(gina,phys301).
enrolled(gina,math372).

enrolled(marty,math480).

% core and elective courses, (department,course,grade)
cores(cs,cs111,c).
cores(cs,cs211,c).
cores(cs,cs311,c).
cores(math,math270,c).
cores(phys,phys201,c).
% core courses list
% return a list of all core courses
coreList(List):- 
    findall(Course,(cores(Depart,Course,c)),List).

elect(cs,cs315,c).
elect(cs,cs436,c).
elect(math,math242,c).
elect(math,math372,c).
elect(phys,phys301,c).

% passed courses
passed(homer,cs111,b).
passed(homer,cs211,b).
passed(homer,cs311,b).
passed(homer,math270,b).
passed(homer,phys201,b).
passed(homer,math372,b).
passed(homer,cs436,b).

passed(ada,cs111,b).
passed(ada,cs211,b).
passed(ada,cs311,b).
passed(ada,math270,b).
passed(ada,phys201,b).
passed(ada,phys301,b).
passed(ada,math372,b).

passed(marty,cs111,b).
passed(marty,cs211,b).
passed(marty,cs311,b).
passed(marty,math270,b).
passed(marty,phys201,b).
passed(marty,cs315,b).
passed(marty,cs436,b).
passed(marty,math242,b).
passed(marty,math372,b).
passed(marty,phys301,b).

passed(gina,cs111,b).
passed(gina,cs211,b).

passed(troy,cs111,b).
passed(troy,cs211,b).
%====================================================
% rule to find who has satisfied all reuqirements
whoSatisfied(Student):-studentList(L),
   satisfied(L).
satisfied([H|R]):-
    coreFulfill(H),elective(H)->print(H),
    print('    '),
    satisfied(R);
    satisfied(R).
%====================================================
% core courses part
% rule to find what are the core requirments
% fulfilled by a student
passCore(Student,Course):- 
    passed(Student,Course,Grade),
    travailCore(Student,Course,Grade).
% rule to check if a student is pass a course
travailCore(Student,Course,Grade):- 
    (Grade=a|Grade=b|Grade=c)->
    passed(Student,Course,Grade), cores(Depart,Course,c).
% rule to list which core courses a student has passed 
passCoreList(List,Student):- 
    findall(Course,(passCore(Student,Course)),List).

% rule to answer if a student satisfied core requirment
% enter a student name and return true or false
coreFulfill(Student):-
    coreList(List),
    passCoreList(L1,Student),
    same(List,L1).
% rule to search studentlist and find who 
% has satisfied core requirments
whoPassedCore(Student):- studentList(L),
    studentpass(Student,L).
studentpass(Student, [H|R]):- 
       coreFulfill(H)-> write(H),
    write('   '),
    studentpass(Student,R);
    studentpass(Student,R).
% rule to check if two lists are same
same([],[]).
same([H1|R1],[H2|R2]):-
    H1 == H2, same(R1,R2).

%==========================================
% elective courses part
% rule to find what are the elective courses
% passed by a student
passElect(Student,Course):- 
   passed(Student,Course,Grade),
    travailElect(Student,Course,Grade).
travailElect(Student,Course,Grade):- 
     (Grade=a|Grade=b|Grade=c)->
    passed(Student,Course,Grade), elect(Depart,Course,c).

% rule to list all elective course passed by a student
passElectList(List,Student):-
    findall(Course,(passElect(Student,Course)),List).

% rule to check is a student satisfied the elective
% course requirements
% enter student name and return true or false
elective(Student):-
    passElectList(L,Student),
    fulfillCS(L,C),fulfillMath(L,M),
    fulfillPhys(L,P),
    C>0 , M>0 , P>0.

% rule to find who has satisfied elective requirements
whoPassedEle(Student):-studentList(L),
    studentpassE(Student,L).
studentpassE(Student, [H|R]):- 
       elective(H)-> write(H),
    write('     '),
    studentpassE(Student,R);
    studentpassE(Student,R).

% rule to count the number of passed elective courses
% of each department
fulfillCS([],0). 
fulfillCS([H|R],N):-
    elect(Depart,H,c),
    ( Depart = cs) -> fulfillCS(R,N1),
    N is N1+1;
    fulfillCS(R,N).

fulfillMath([],0). 
fulfillMath([H|R],N):-
    elect(Depart,H,c),
    ( Depart = math) -> fulfillMath(R,N1),
    N is N1+1;
    fulfillMath(R,N).

fulfillPhys([],0). 
fulfillPhys([H|R],N):-
     elect(Depart,H,c),
    ( Depart = phys) -> fulfillPhys(R,N1),
    N is N1+1;
    fulfillPhys(R,N).
