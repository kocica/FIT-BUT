%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: flp20-log.pl
% FLP - Rubiks cube
% Filip Kocica <xkocic01@stud.fit.vutbr.cz>
% 4/11/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%
% Application entry point - main/0
%
main :-
	prompt(_, ''),
	read_lines(LL),
	split_lines(LL,S),
	to_cube(S,C),
	print_cube(C),
	( is_cube_solved(C) -> halt ; solve_cube(C,7,[]) ),
	halt.

read_line(L,C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		read_line(LL,_),
		[C|LL] = L).

isEOFEOL(C) :-
	C == end_of_file;
	(char_code(C,Code), Code==10).

read_lines(Ls) :-
	read_line(L,C),
	( C == end_of_file, Ls = [] ;
	  read_lines(LLs), Ls = [L|LLs]
	).

split_line([],[[]]) :- !.
split_line([' '|T], [[]|S1]) :- !, split_line(T,S1).
split_line([32|T], [[]|S1]) :- !, split_line(T,S1).
split_line([H|T], [[H|G]|S1]) :- split_line(T,[G|S1]).

split_lines([],[]).
split_lines([L|Ls],[H|T]) :- split_lines(Ls,T), split_line(L,H).

%
% Union lists together
%
union_lists([], []).
union_lists([X|XS], R) :-
	union_lists(XS, RR),
	append(X, RR, R).

%
% Extract n-th element from list
%
get_element([], _, []) :- false.
get_element([X|_], 1, R) :- R = X.
get_element([_|X], N, R) :- N1 is N-1, get_element(X, N1, R).

%
% Convert input cube format to internal represened rubiks cube format
%
to_cube([[A1], [A2], [A3], X, Y, Z, [F1], [F2], [F3]], R) :-
	get_element(X, 1, B1), get_element(Y, 1, B2), get_element(Z, 1, B3),
	get_element(X, 2, C1), get_element(Y, 2, C2), get_element(Z, 2, C3),
	get_element(X, 3, D1), get_element(Y, 3, D2), get_element(Z, 3, D3),
	get_element(X, 4, E1), get_element(Y, 4, E2), get_element(Z, 4, E3),
	union_lists([A1, A2, A3], S5), union_lists([B1, B2, B3], S1), union_lists([C1, C2, C3], S2),
	union_lists([D1, D2, D3], S3), union_lists([E1, E2, E3], S4), union_lists([F1, F2, F3], S6),
	R = [S1, S2, S3, S4, S5, S6].

%
% Print internal representation of rubics cube to the standard output
%
print_cube(
		[
			[A1, A2, A3, A4, A5, A6, A7, A8, A9],
			[B1, B2, B3, B4, B5, B6, B7, B8, B9],
			[C1, C2, C3, C4, C5, C6, C7, C8, C9],
			[D1, D2, D3, D4, D5, D6, D7, D8, D9],
			E, F
		]) :-
	format('~w~w~w\n~w~w~w\n~w~w~w\n', E),
	format('~w~w~w ~w~w~w ~w~w~w ~w~w~w\n', [A1,A2,A3,B1,B2,B3,C1,C2,C3,D1,D2,D3]),
	format('~w~w~w ~w~w~w ~w~w~w ~w~w~w\n', [A4,A5,A6,B4,B5,B6,C4,C5,C6,D4,D5,D6]),
	format('~w~w~w ~w~w~w ~w~w~w ~w~w~w\n', [A7,A8,A9,B7,B8,B9,C7,C8,C9,D7,D8,D9]),
	format('~w~w~w\n~w~w~w\n~w~w~w\n', F).

print_cubes([]) :- true.
print_cubes([X|XS]) :-
	nl,
	print_cube(X),
	print_cubes(XS).

%
% Check if rubiks cube is solved (all six sides have only one color)
%
is_cube_solved([]) :- true.
is_cube_solved([X|EX]) :-
	sort(X,[_]), !,
	is_cube_solved(EX).

%
% Solve rubics cube using DFS
%
solve_cube(C, S, R) :-
	integer(S),
	S > 0,
	S1 is S-1,
	(
		rotate(1, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(2, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(3, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(4, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(5, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(6, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(9, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(10, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(7, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(8, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(11, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) );
		rotate(12, C, CC),
		( is_cube_solved(CC), print_cubes(R), nl, print_cube(CC) ; union_lists([R, [CC]], RR), solve_cube(CC, S1, RR) )
	).

rotate(N, [
			[A1, A2, A3, A4, A5, A6, A7, A8, A9],
			[B1, B2, B3, B4, B5, B6, B7, B8, B9],
			[C1, C2, C3, C4, C5, C6, C7, C8, C9],
			[D1, D2, D3, D4, D5, D6, D7, D8, D9],
			[E1, E2, E3, E4, E5, E6, E7, E8, E9],
			[F1, F2, F3, F4, F5, F6, F7, F8, F9]
		  ], R) :-
	integer(N), N > 0, N < 13,
	(
		(N == 1, R = [
						[B1, B2, B3, A4, A5, A6, A7, A8, A9],
						[C1, C2, C3, B4, B5, B6, B7, B8, B9],
						[D1, D2, D3, C4, C5, C6, C7, C8, C9],
						[A1, A2, A3, D4, D5, D6, D7, D8, D9],
						[E7, E4, E1, E8, E5, E2, E9, E6, E3],
						[F1, F2, F3, F4, F5, F6, F7, F8, F9]
					 ]);
		(N == 2, R = [
						[D1, D2, D3, A4, A5, A6, A7, A8, A9],
						[A1, A2, A3, B4, B5, B6, B7, B8, B9],
						[B1, B2, B3, C4, C5, C6, C7, C8, C9],
						[C1, C2, C3, D4, D5, D6, D7, D8, D9],
						[E3, E6, E9, E2, E5, E8, E1, E4, E7],
						[F1, F2, F3, F4, F5, F6, F7, F8, F9]
					 ]);
		(N == 3, R = [
						[D1, D2, D3, A4, A5, A6, A7, A8, A9],
						[A1, A2, A3, B4, B5, B6, B7, B8, B9],
						[B1, B2, B3, C4, C5, C6, C7, C8, C9],
						[C1, C2, C3, D4, D5, D6, D7, D8, D9],
						[E1, E2, E3, E4, E5, E6, E7, E8, E9],
						[F7, F4, F1, F8, F5, F2, F9, F6, F3]
					 ]);
		(N == 4, R = [
						[A1, A2, A3, A4, A5, A6, B7, B8, B9],
						[B1, B2, B3, B4, B5, B6, C7, C8, C9],
						[C1, C2, C3, C4, C5, C6, D7, D8, D9],
						[D1, D2, D3, D4, D5, D6, A7, A8, A9],
						[E1, E2, E3, E4, E5, E6, E7, E8, E9],
						[F3, F6, F9, F2, F5, F8, F1, F4, F7]
					 ]);
		(N == 5, R = [
						[A7, A4, A1, A8, A5, A2, A9, A6, A3],
						[E7, B2, B3, E8, B5, B6, E9, B8, B9],
						[C1, C2, C3, C4, C5, C6, C7, C8, C9],
						[D1, D2, F1, D4, D5, F2, D7, D8, F3],
						[E1, E2, E3, E4, E5, E6, D3, D6, D9],
						[B7, B4, B1, F4, F5, F6, F7, F8, F9]
					 ]);
		(N == 6, R = [
						[A3, A6, A9, A2, A5, A8, A1, A4, A7],
						[F3, B2, B3, F2, B5, B6, F1, B8, B9],
						[C1, C2, C3, C4, C5, C6, C7, C8, C9],
						[D1, D2, E9, D4, D5, E8, D7, D8, E7],
						[E1, E2, E3, E4, E5, E6, B1, B4, B7],
						[D3, D6, D9, F4, F5, F6, F7, F8, F9]
					 ]);
		(N == 7, R = [
						[A1, A2, F3, A4, A5, F6, A7, A8, F9],
						[B7, B4, B1, B8, B5, B2, B9, B6, B3],
						[E9, C2, C3, E6, C5, C6, E3, C8, C9],
						[D1, D2, D3, D4, D5, D6, D7, D8, D9],
						[E1, E2, A3, E4, E5, A6, E7, E8, A9],
						[F1, F2, C7, F4, F5, C4, F7, F8, C1]
					 ]);
		(N == 8, R = [
						[A1, A2, E3, A4, A5, E6, A7, A8, E9],
						[B3, B6, B9, B2, B5, B8, B1, B4, B7],
						[F9, C2, C3, F6, C5, C6, F3, C8, C9],
						[D1, D2, D3, D4, D5, D6, D7, D8, D9],
						[E1, E2, C7, E4, E5, C4, E7, E8, C1],
						[F1, F2, A3, F4, F5, A6, F7, F8, A9]
					 ]);
		(N == 9, R = [
						[E1, A2, A3, E4, A5, A6, E7, A8, A9],
						[B1, B2, B3, B4, B5, B6, B7, B8, B9],
						[C1, C2, F7, C4, C5, F4, C7, C8, F1],
						[D7, D4, D1, D8, D5, D2, D9, D6, D3],
						[C9, E2, E3, C6, E5, E6, C3, E8, E9],
						[A1, F2, F3, A4, F5, F6, A7, F8, F9]
					 ]);
		(N == 10, R = [
						[F1, A2, A3, F4, A5, A6, F7, A8, A9],
						[B1, B2, B3, B4, B5, B6, B7, B8, B9],
						[C1, C2, E7, C4, C5, E4, C7, C8, E1],
						[D3, D6, D9, D2, D5, D8, D1, D4, D7],
						[A1, E2, E3, A4, E5, E6, A7, E8, E9],
						[C9, F2, F3, C6, F5, F6, C3, F8, F9]
					 ]);
		(N == 11, R = [
						[A1, A2, A3, A4, A5, A6, A7, A8, A9],
						[B1, B2, F9, B4, B5, F8, B7, B8, F7],
						[C7, C4, C1, C8, C5, C2, C9, C6, C3],
						[E3, D2, D3, E2, D5, D6, E1, D8, D9],
						[B3, B6, B9, E4, E5, E6, E7, E8, E9],
						[F1, F2, F3, F4, F5, F6, D1, D4, D7]
					 ]);
		(N == 12, R = [
						[A1, A2, A3, A4, A5, A6, A7, A8, A9],
						[B1, B2, E1, B4, B5, E2, B7, B8, E3],
						[C3, C6, C9, C2, C5, C8, C1, C4, C7],
						[F7, D2, D3, F8, D5, D6, F9, D8, D9],
						[D7, D4, D1, E4, E5, E6, E7, E8, E9],
						[F1, F2, F3, F4, F5, F6, B9, B6, B3]
					 ])
	).
