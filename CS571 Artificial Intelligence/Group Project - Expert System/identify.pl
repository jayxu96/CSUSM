% Main Gui/interface file
% CS571 Bird Identifer - Fall 2018
% By Kyle Zampell and Runyu Xu

% init
% purges all facts from working memory.
% Note this fails if no facts or untrue exist in working memory?
init :- retractm(fact(X)), retractm(fact(X,Y)), retractm(untrue(X)).

% solve(Goal): solve(identify_bird(Advice)) what bird the user sees.
% Top level call.  Initializes working memory; attempts to solve Goal
% with certainty factor; prints results; asks user if they would like a
% trace.
solve(Goal) :- 
    init, %print_help,
    solve(Goal,C,[],1),
    nl,write('Solved '),write(Goal),
    write(' With Certainty = '),write(C),nl,nl,
    ask_for_trace(Goal).

% init
% purges all facts from working memory.

init :- retractm(fact(X)), retract(fact(X,Y)), retractm(untrue(X)).

% solve(Goal,CF,Rulestack,Cutoff_context)
% Attempts to solve Goal by backwards chaining on rules;  CF is
% certainty factor of final conclusion; Rulestack is stack of
% rules, used in why queries, Cutoff_context is either 1 or -1
% depending on whether goal is to be proved true or false
% (e.g. not Goal requires Goal be false in oreder to succeed).

solve(true,100,Rules,_).

solve(A,100,Rules,_) :- 
    fact(A).

solve(A,-100,Rules,_) :-
    untrue(A).

solve(not(A),C,Rules,T) :- 
    T2 is -1 * T,
    solve(A,C1,Rules,T2),
    C is -1 * C1.

solve((A,B),C,Rules,T) :- 
    solve(A,C1,Rules,T), 
    above_threshold(C1,T),
    solve(B,C2,Rules,T),
    above_threshold(C2,T),
    minimum(C1,C2,C).

solve(A,C,Rules,T) :- 
    rule((A :- B),C1), 
    solve(B,C2,[rule(A,B,C1)|Rules],T),
    C is (C1 * C2) / 100,
    above_threshold(C,T).

solve(A,C,Rules,T) :- 
    rule((A), C),
    above_threshold(C,T).

solve(A,C,Rules,T) :- 
    ( atom(A) ->
    askable(A), 
    not(known(A)),
    ask(A,Answer), respond(Answer,A,C,Rules) ;
    with_output_to(atom(A1), write(A)),
    atomic_list_concat(A2, '(', A1),
    grab_head(A2, A3),
    not(known(A3, _)),
    ( askablev(A3) ->
    askv(A3,Answer), respond(Answer,A3,C))).

% respond( Answer, Query, CF, Rule_stack).
% respond will process Answer (yes, no, how, why, help).
% asserting to working memory (yes or no)
% displaying current rule from rulestack (why)
% showing proof trace of a goal (how(Goal)
% displaying help (help).
% Invalid responses are detected and the query is repeated.

respond(Bad_answer,A,C,Rules) :- 
    not(member(Bad_answer,[help, yes,no,why,how(_)])),
    write('answer must be either help, (y)es, (n)o, (h)ow(X), (w)hy'),nl,nl,
    ask(A,Answer),
    respond(Answer,A,C,Rules).

respond(yes,A,100,_) :- 
    assert(fact(A)).

respond(no,A,-100,_) :- 
    assert(untrue(A)).

respond(why,A,C,[Rule|Rules]) :- 
    display_rule(Rule),
    ask(A,Answer),
    respond(Answer,A,C,Rules).

respond(why,A,C,[]) :-
    write('Back to goal, no more explanation  possible'),nl,nl,
    ask(A,Answer),
    respond(Answer,A,C,[]).

respond(how(Goal),A,C,Rules) :- 
    respond_how(Goal),
    ask(A,Answer),
    respond(Answer,A,C,Rules).

respond(help,A,C,Rules) :- 
    print_help,
    ask(A,Answer),
    respond(Answer,A,C,Rules).

respond(Answer,A3,100) :-
	assert(fact([A3, Answer])).

% ask(Query, Answer)
% Writes Query and reads the Answer.  Abbreviations (y, n, h, w) are
% trnslated to appropriate command be filter_abbreviations

ask(Query,Answer) :- 
    display_query(Query),
    read(A),
    filter_abbreviations(A,Answer), !.


askv(Query, Answer) :-
	display_query(Query),
	read(Answer).
	%with_output_to(atom(Q1), write(Query)),
	%atomic_list_concat(Q2, '(', Q1),
	%grab_head(Q2, Q3),
	%atom_concat(Ans, ')', X1),
	%atom_concat('(', X1, X2),
	%atom_concat(Q3, X2, Answer).

grab_head([Out|_], Out).

% filter_abbreviations( Answer, Command)
% filter_abbreviations will expand Answer into Command.  If
% Answer is not a known abbreviation, then Command = Answer.

filter_abbreviations(y,yes).
filter_abbreviations(n,no).
filter_abbreviations(w,why).
filter_abbreviations(h,help).
filter_abbreviations(h(X),how(X)).
filter_abbreviations(X,X).

% known(Goal)
% Succeeds if Goal is known to be either true or untrue.

known(Goal) :- askable(Goal), fact(Goal).
known(Goal) :- askable(Goal), untrue(Goal).
known(Goal, V) :- fact([Goal, V]).

% ask_for_trace(Goal).
% Invoked at the end of a consultation, ask_for_trace asks the user if
% they would like a trace of the reasoning to a goal.

ask_for_trace(Goal) :-
    write('Trace of reasoning to goal ? '),
    read(Answer),nl,
    show_trace(Answer,Goal),!.

% show_trace(Answer,Goal)
% If Answer is ``yes'' or ``y,'' show trace will display  a trace
% of Goal, as in a ``how'' query.  Otherwise, it succeeds, doing nothing.

show_trace(yes,Goal) :- respond_how(Goal).

show_trace(y,Goal) :- respond_how(Goal).

show_trace(_,_).

% print_help
% Prints a help screen.

print_help :- 
    write('Exshell allows the following responses to queries:'),nl,nl,
    write('   yes - query is known to be true.'),nl,
    write('   no - query is false.'),nl,
    write('   why - displays rule currently under consideration.'),nl,
    write('   how(X) - if X has been inferred, displays trace of reasoning.'),nl,
    write('   help - prints this message.'),nl,
    write('   Your response may be abbreviated to the first letter and must end with a period (.).'),nl.

% display_query(Goal)
% Shows Goal to user in the form of a query.

display_query(Goal) :-
    write(Goal),
    write('? ').

% display_rule(rule(Head, Premise, CF))
% prints rule in IF...THEN form.

display_rule(rule(Head,Premise,CF)) :-
    write('IF       '),
    write_conjunction(Premise),
    write('THEN     '),
    write(Head),nl,
    write('CF   '),write(CF),
    nl,nl.

% write_conjunction(A)
% write_conjunction will print the components of a rule premise.  If any
% are known to be true, they are so marked.

write_conjunction((A,B)) :-
    write(A), flag_if_known(A),!, nl,
    write('     AND '),
    write_conjunction(B).

write_conjunction(A) :- write(A),flag_if_known(A),!, nl.

% flag_if_known(Goal).
% Called by write_conjunction, if Goal follows from current state
% of working memory, prints an indication, with CF.

flag_if_known(Goal) :- 
    build_proof(Goal,C,_,1), 
    write('     ***Known, Certainty = '),write(C).

flag_if_known(A). 

% Predicates concerned with how queries.

% respond_how(Goal).
% calls build_proof to determine if goal follows from current state of working
% memory.  If it does, prints a trace of reasoning, if not, so indicates.

respond_how(Goal) :- 
    build_proof(Goal,C,Proof,1),
    interpret(Proof),nl,!.

respond_how(Goal) :- 
    build_proof(Goal,C,Proof,-1),
    interpret(Proof),nl,!.

respond_how(Goal) :- 
    write('Goal does not follow at this stage of consultation.'),nl.

% build_proof(Goal, CF, Proof, Cutoff_context).
% Attempts to prove Goal, placing a trace of the proof in Proof.
% Functins the same as solve, except it does not ask for unknown information.
% Thus, it only proves goals that follow from the rule base and the current 
% contents of working memory.

build_proof(true,100,(true,100),_).

build_proof(Goal, 100, (Goal :- given,100),_) :- fact(Goal).

build_proof(Goal, -100, (Goal :- given,-100),_) :- untrue(Goal).

build_proof(not(Goal), C, (not(Proof),C),T) :- 
    T2 is -1 * T,
    build_proof(Goal,C1,Proof,T2),
    C is -1 * C1.

build_proof((A,B),C,(ProofA, ProofB),T) :-
    build_proof(A,C1,ProofA,T),
    above_threshold(C1,T),
    build_proof(B,C2,ProofB,T),
    above_threshold(C2,T),
    minimum(C1,C2,C).

build_proof(A, C, (A :- Proof,C),T) :-
    rule((A :- B),C1), 
    build_proof(B, C2, Proof,T),
    C is (C1 * C2) / 100,
    above_threshold(C,T).

build_proof(A, C, (A :- true,C),T) :-
    rule((A),C),
    above_threshold(C,T).

% interpret(Proof).
% Interprets a Proof as constructed by build_proof,
% printing a trace for the user.

interpret((Proof1,Proof2)) :-
    interpret(Proof1),interpret(Proof2).

interpret((Goal :- given,C)):-
    write(Goal),
    write(' was given. CF = '), write(C),nl,nl.

interpret((not(Proof), C)) :-
    extract_body(Proof,Goal),
    write('not '),
    write(Goal),
    write(' CF = '), write(C),nl,nl,
    interpret(Proof).

interpret((Goal :- true,C)) :-
    write(Goal),
        write(' is a fact, CF = '),write(C),nl.

interpret(Proof) :-
    is_rule(Proof,Head,Body,Proof1,C),
    nl,write(Head),write(' CF = '),
    write(C), nl,write('was proved using the rule'),nl,nl,
    rule((Head :- Body),CF),
    display_rule(rule(Head, Body,CF)), nl,
    interpret(Proof1).

% isrule(Proof,Goal,Body,Proof,CF)
% If Proof is of the form Goal :- Proof, extracts
% rule Body from Proof.

is_rule((Goal :- Proof,C),Goal, Body, Proof,C) :-
    not(member(Proof, [true,given])),
    extract_body(Proof,Body).

% extract_body(Proof).
% extracts the body of the top level rule from Proof.

extract_body((not(Proof), C), (not(Body))) :-
    extract_body(Proof,Body).

extract_body((Proof1,Proof2),(Body1,Body2)) :-
    !,extract_body(Proof1,Body1),
    extract_body(Proof2,Body2).

extract_body((Goal :- Proof,C),Goal).
    
% Utility Predicates.

retractm(X) :- retract(X), fail.
retractm(X) :- retract((X:-Y)), fail.
retractm(X).

member(X,[X|_]).
member(X,[_|T]) :- member(X,T).

minimum(X,Y,X) :- X =< Y.
minimum(X,Y,Y) :- Y < X.

above_threshold(X,1) :- X >= 20.
above_threshold(X,-1) :- X =< -20.


% Knowledge Base goes below here  -----------------

% Top level goal, starts search.
rule((identify_bird(Bird) :-  
	habitat(A),
	diet(B),
	wingspan(C),
	beak_shape(D),
	beak_size(E),
	talons(F),
	color(G),
	species(Bird)),100).

% rules for habitat
rule((habitat(coast) :-	coastal_habitat), 100).
rule((habitat(inland) :- inland_habitat), 100).
rule((habitat(forest) :- forest_habitat), 100).
rule((habitat(desert) :- desert_habitat), 100).

% rules to infer type of diet?
rule((diet(insects) :- eats_insects), 100).
rule((diet(fish) :- eats_fish), 100).
rule((diet(small_animals) :- eats_small_animals), 100).
rule((diet(vegetation) :- eats_vegetation), 100).

% rules for beak shape
rule((beak_shape(sharp) :- sharp_beak), 100).
rule((beak_shape(soft) :- soft_beak), 100).

% rules for beak size
rule((beak_size(small) :- small_beak), 100).
rule((beak_size(large) :- large_beak), 100).
rule((beak_size(oversized) :- oversized_beak), 100).

% rules for talons
rule((talons(sharp) :- sharp_talons), 100).
rule((talons(pointy) :- pointy_talons), 100).
rule((talons(dull) :- dull_talons), 100).

% rules to infer whether it has wings?
%rule((wings(vestigial) :- not(can_fly)), 100).
%rule((wings(flight) :- can_fly), 100).
rule((wingspan(X) :- [wingspan, X]), 100).
rule((color(X) :- [color, X]), 100).

%special support
%wingspan(X) :- fact(wingspan, X).
%color(X) :- fact(color, X).
%habitat(X) :- fact(habitat, X).
%beak_size(X) :- fact(beak_size, X).
%beak_shape(X) :- fact(beak_shape, X).
%talons(X) :- fact(talons, X).
%diet(X) :- fact(diet, X).
%wings(X) :- fact(wings, X).

% askable features ----
% misc
askable(migrates).
askable(nocturnal).
% variable inputs
askablev(color).
askablev(wingspan).
% diets
askable(eats_insects).
askable(eats_fish).
askable(eats_small_animals).
askable(eats_vegetation).
% special wing type
askable(can_fly).
% habitat
askable(forest_habitat).
askable(coastal_habitat).
askable(inland_habitat).
askable(desert_habitat).
% beak shape
askable(soft_beak).
askable(sharp_beak).
% beak size
askable(small_beak).
askable(large_beak).
askable(oversized_beak).
% talon sharpness
askable(dull_talons).
askable(pointy_talons).
askable(sharp_talons).

% Note that facts must be sorted the same way the rules are introduced!
% Habitat first: Coast > Inland > Forest > Desert
% Diet second: Insects > Fish > Small Animals > Vegetation
% Beak Shape third: Sharp > Soft
% Beak Size fourth: Small > Large > Oversized
% Talons fifth: Sharp > Pointy > Dull
% Misc facts go before Color
% Coastal birds ----------------
rule((species(sandpiper) :-
	coastal_habitat,
	eats_insects,
	can_fly, [wingspan, 1],
	soft_beak,
	large_beak,
	pointy_talons,
[color, brown]), 100).

rule((species(eagle) :-
	coastal_habitat,
	eats_fish,
	can_fly, [wingspan, 4],
	sharp_beak,
	large_beak,
	sharp_talons,
[color, black]), 100).

rule((species(penguin) :-
	coastal_habitat,
	eats_fish,
	not(can_fly), [wingspan, 2],
	sharp_beak,
	large_beak,
	dull_talons,
[color, black]), 100).

rule((species(albatross) :-
	coastal_habitat,
	eats_fish,
	can_fly, [wingspan, 6],
	soft_beak,
	large_beak,
	pointy_talons,
[color, white]), 100).

rule((species(seagull) :-
	coastal_habitat,
	eats_fish,
	can_fly, [wingspan, 2],
	soft_beak,
	large_beak,
	dull_talons,
[color, white]), 100).

rule((species(pelican) :-
	coastal_habitat,
	eats_fish,
	can_fly, [wingspan, 4],
	soft_beak,
	oversized_beak,
	dull_talons,
[color, white]), 100).

% inland birds -------------------

rule((species(mockingbird) :-
	inland_habitat,
	eats_insects,
	can_fly, [wingspan, 1],
	soft_beak,
	small_beak,
	pointy_talons,
	migrates,
[color, gray]), 100).

rule((species(raven) :-
	inland_habitat,
	eats_insects,
	can_fly, [wingspan, 2],
	soft_beak,
	large_beak,
	pointy_talons,
[color, black]), 100).

rule((species(stork) :-
	inland_habitat,
	eats_fish,
	can_fly, [wingspan, 4],
	soft_beak,
	oversized_beak,
	pointy_talons,
[color, white]), 100).

rule((species(barn_owl) :-
	inland_habitat,
	eats_small_animals,
	can_fly, [wingspan, 3],
	sharp_beak,
	small_beak,
	sharp_talons,
	nocturnal,
[color, white]), 100).

rule((species(hawk) :-
	inland_habitat,
	eats_small_animals,
	can_fly, [wingspan, 4],
	sharp_beak,
	large_beak,
	sharp_talons,
[color, brown]), 100).

rule((species(turkey) :-
inland_habitat,
	eats_vegetation,
	not(can_fly), [wingspan, 4],
	soft_beak,
	small_beak,
	pointy_talons,
[color, black]), 100).

rule((species(swan) :-
	inland_habitat,
	eats_vegetation,
	can_fly, [wingspan, 3],
	soft_beak,
	small_beak,
	dull_talons,
[color, white]), 100).

rule((species(canadian_goose) :-
	inland_habitat,
	eats_vegetation,
	can_fly, [wingspan, 4],
	migrates,
	soft_beak,
	large_beak,
	dull_talons,
[color, brown]), 100).

rule((species(duck) :-
	inland_habitat,
	eats_vegetation,
	can_fly, [wingspan, 2],
	soft_beak,
	large_beak,
	dull_talons,
	migrates,
[color, brown]), 100).

% forest birds -------------

rule((species(cardinal) :-
	forest_habitat,
	eats_insects,
	can_fly, [wingspan, 1],
	soft_beak,
	small_beak,
	pointy_talons,
[color, red]), 100).

rule((species(bluejay) :-
	forest_habitat,
	eats_insects,
	can_fly, [wingspan, 1],
	soft_beak,
	small_beak,
	pointy_talons,
[color, blue]), 100).

rule((species(kiwi) :-
	forest_habitat,
	eats_insects,
	not(can_fly), [wingspan, 0],
	soft_beak,
	large_beak,
	pointy_talons,
[color, brown]), 100).

rule((species(parrot) :-
	forest_habitat,
	eats_vegetation,
	can_fly, [wingspan, 1],
	soft_beak,
	small_beak,
	pointy_talons,
[color, green]), 100).

rule((species(toucan) :-
	forest_habitat,
	eats_vegetation,
	can_fly, [wingspan, 3],
	soft_beak,
	oversized_beak,
	pointy_talons,
[color, black]), 100).

% desert birds --------------

rule((species(nighthawk) :-
	desert_habitat,
	eats_insects,
	can_fly, [wingspan, 2],
	sharp_beak,
	small_beak,
	pointy_talons,
	nocturnal,
[color, brown]), 100).

rule((species(roadrunner) :-
	desert_habitat,
	eats_insects,
	not(can_fly), [wingspan, 1],
	soft_beak,
	large_beak,
	pointy_talons,
[color, brown]), 100).

rule((species(ostrich) :-
	desert_habitat,
	eats_vegetation,
	not(can_fly), [wingspan, 5],
	soft_beak,
	large_beak,
	dull_talons,
[color, black]), 100).
