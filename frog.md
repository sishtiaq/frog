
#Frogs

The game of frogs is a bit like chinese checkers: you have get all the 
frogs on the left to right, and all the ones on the right to the left. 
The game board for the 3-frog version, for instance, is 7 columns wide 
and starts at
```
		XXX.OOO
```
That is, 3 X frogs are on the left of the board and 3 O frogs are 
on the right. There's an unoccupied space in the middle. The object
of the game is to move each frog until the board ends at
```
		OOO.XXX
```
Frogs can only move forwards (X moves rightwards, O moves leftwards) 
in the following ways:

1. A frog can move forward by one move into an unoccupied space. E.g.,
from the initial board, we can move to XX.XOOO.

2. A frog can jump over the frog in front of it into an unoccupied
space.  E.g., from XX.XOOO, we can move to XXOX.OO.

It'll take you a few minutes to solve the 3-frog game. The solution
is:
```
		XXX.OOO
		XX.XOOO
		XXOX.OO
		XXOXO.O
		XXO.OXO
		X.OXOXO
		.XOXOXO
		OX.XOXO
		OXOX.XO
		OXOXOX.
		OXOXO.X
		OXO.OXX
		O.OXOXX
		OO.XOXX
		OOOX.XX
		OOO.XXX
```

If you're stuck doing the 3-frog game, try the 2-frog version
first. But there's a general question: Is there a solution to an 
n-frog version of the game? The answer is yes, purely by some
syntactic analysis. 

My solution is based on the idea that the first half of the set
of moves are a kind of "unrolling" out of the X's. This gets us from
XXX.OOO to .XOXOXO. The second half of the set of moves are a kind of
"rolling" back of the O's in which .XOXOXO is transformed to
OOO.XXX.

What does unrolling mean? The pattern that I saw in it is that 
XXX.OOO gets transformed to XX(something)OO, which then gets 
transformed to X(something)O. In the case of unrolling, the 
"(something)" is always repetitions of the "OX"
pattern. Now let's write X^n to mean n repetitions of X and use
brackets to dis-ambiguate any patterns.  Then, as long as you can
describe a procedure which does either this transformation:
```
	(X^n).((OX)^p)(O^n) --> (X^(n-1))((OX)^(p+1)).(O^(n-1))
```
or this one:
```
	(X^n)((OX)^p).(O^n) --> (X^(n-1)).((OX)^(p+1))(O^(n-1))
```
then you'll have solved unrolling. (Both initial boards are the same
except for the '.' being on the left or the right of the
(OX)-pattern.)

Let's look at the first one of these; I call it UnrollLeft. You can
show that UnrollLeft
```
	(X^n).((OX)^p)(O^n) --> (X^(n-1))((OX)^(p+1)).(O^(n-1))
```
can indeed be made by an argument based on what the value of p (i.e.,
how many times OX is repeated) is. The argument is as follows.

- If p=0 (this is how you normally start the game), then do the
following transformations:
```
	(X^n).(O^n)
	=   (X^(n-1))X.O(O^(n-1))
	--> (X^(n-1)).XO(O^(n-1))
	--> (X^(n-1))OX.(O^(n-1))
	=   (X^(n-1))((OX)^1).(O^(n-1))
```
- If p is any other number (p>0), then in fact the board actually looks
like this
```
	(X^(n-1))X.OX((OX)^(p-1))O(O^(n-1))
```
(All I've done is expanded some of the patterns.) Now you can do
the following transformations:
```
	(X^n).OX((OX)^(p-1))(O^n)
	=   (X^(n-1))X.OX((OX)^(p-1))O(O^(n-1))
	--> (X^(n-1)).XOX((OX)^(p-1))O(O^(n-1))
	--> (X^(n-1))OX.X((OX)^(p-1))O(O^(n-1))
	--> (X^(n-1))OX((OX)^p).(O^(n-1))      (*)
```
The last transformation, marked (*), does the following sequence of
transformations on the central segment of the board:
```
	.X(OX^(n-1))O --> (OX)^n.
```
This involves a genuine series of moves in which all the Os jump one
left, so shifting the '.' to the right of the pattern.

In both cases, p=0 and p>0, we're left with the board in the desired
form of (X^(n-1))((OX)^(p+1)).(O^(n-1)). 

A similar analysis applies to UnrollRight. To do a complete Unroll, we
keep on applying, repeatedly, UnrollLeft and UnrollRight until there's
only 1 X and 1 O left on the ends of the board.

Rolling works similarly by means of a RolliLeft and RollRight
procedures.

The main break in finding the solution to the n-frog game is working
out that (left) unrolling is
```
	(X^n).((OX)^p)(O^n) --> (X^(n-1))((OX)^(p+1)).(O^(n-1))
```
and (left) rolling is
```
	(O^n).((XO)^p)(X^n) --> (O^(n+1)).((XO)^(p-1))(X^(n+1))
```
From this syntactic analysis, it's a small jump to writing up the C
code which actually does the string transformations. My solution runs
in quadratic time (for the n-frog game, it takes n*(n+2) steps to get
to the end).


Finally, here --- in all its glory --- is a play of the 15-frog
problem:
```
pc456:sishtiaq ~/src/frog> a.out
Enter the number of frogs on one side ==> 15
Initial board = board=XXXXXXXXXXXXXXX.OOOOOOOOOOOOOOO, board_size=31, X_size=15, dot_pos=15, OX_size=0.
Number of swaps needed to finish=255.
board=XXXXXXXXXXXXXXX.OOOOOOOOOOOOOOO, board_size=31, X_size=15, dot_pos=15, OX_size=0.
board=XXXXXXXXXXXXXX.XOOOOOOOOOOOOOOO, board_size=31, X_size=15, dot_pos=14, OX_size=0.
board=XXXXXXXXXXXXXXOX.OOOOOOOOOOOOOO, board_size=31, X_size=14, dot_pos=16, OX_size=1.
board=XXXXXXXXXXXXXXOXO.OOOOOOOOOOOOO, board_size=31, X_size=14, dot_pos=17, OX_size=1.
board=XXXXXXXXXXXXXXO.OXOOOOOOOOOOOOO, board_size=31, X_size=14, dot_pos=15, OX_size=1.
board=XXXXXXXXXXXXX.OXOXOOOOOOOOOOOOO, board_size=31, X_size=14, dot_pos=13, OX_size=1.
board=XXXXXXXXXXXX.XOXOXOOOOOOOOOOOOO, board_size=31, X_size=13, dot_pos=12, OX_size=2.
board=XXXXXXXXXXXXOX.XOXOOOOOOOOOOOOO, board_size=31, X_size=13, dot_pos=14, OX_size=2.
board=XXXXXXXXXXXXOXOX.XOOOOOOOOOOOOO, board_size=31, X_size=13, dot_pos=16, OX_size=2.
board=XXXXXXXXXXXXOXOXOX.OOOOOOOOOOOO, board_size=31, X_size=13, dot_pos=18, OX_size=2.
board=XXXXXXXXXXXXOXOXOXO.OOOOOOOOOOO, board_size=31, X_size=12, dot_pos=19, OX_size=3.
board=XXXXXXXXXXXXOXOXO.OXOOOOOOOOOOO, board_size=31, X_size=12, dot_pos=17, OX_size=3.
board=XXXXXXXXXXXXOXO.OXOXOOOOOOOOOOO, board_size=31, X_size=12, dot_pos=15, OX_size=3.
board=XXXXXXXXXXXXO.OXOXOXOOOOOOOOOOO, board_size=31, X_size=12, dot_pos=13, OX_size=3.
board=XXXXXXXXXXX.OXOXOXOXOOOOOOOOOOO, board_size=31, X_size=12, dot_pos=11, OX_size=3.
board=XXXXXXXXXX.XOXOXOXOXOOOOOOOOOOO, board_size=31, X_size=11, dot_pos=10, OX_size=4.
board=XXXXXXXXXXOX.XOXOXOXOOOOOOOOOOO, board_size=31, X_size=11, dot_pos=12, OX_size=4.
board=XXXXXXXXXXOXOX.XOXOXOOOOOOOOOOO, board_size=31, X_size=11, dot_pos=14, OX_size=4.
board=XXXXXXXXXXOXOXOX.XOXOOOOOOOOOOO, board_size=31, X_size=11, dot_pos=16, OX_size=4.
board=XXXXXXXXXXOXOXOXOX.XOOOOOOOOOOO, board_size=31, X_size=11, dot_pos=18, OX_size=4.
board=XXXXXXXXXXOXOXOXOXOX.OOOOOOOOOO, board_size=31, X_size=11, dot_pos=20, OX_size=4.
board=XXXXXXXXXXOXOXOXOXOXO.OOOOOOOOO, board_size=31, X_size=10, dot_pos=21, OX_size=5.
board=XXXXXXXXXXOXOXOXOXO.OXOOOOOOOOO, board_size=31, X_size=10, dot_pos=19, OX_size=5.
board=XXXXXXXXXXOXOXOXO.OXOXOOOOOOOOO, board_size=31, X_size=10, dot_pos=17, OX_size=5.
board=XXXXXXXXXXOXOXO.OXOXOXOOOOOOOOO, board_size=31, X_size=10, dot_pos=15, OX_size=5.
board=XXXXXXXXXXOXO.OXOXOXOXOOOOOOOOO, board_size=31, X_size=10, dot_pos=13, OX_size=5.
board=XXXXXXXXXXO.OXOXOXOXOXOOOOOOOOO, board_size=31, X_size=10, dot_pos=11, OX_size=5.
board=XXXXXXXXX.OXOXOXOXOXOXOOOOOOOOO, board_size=31, X_size=10, dot_pos=9, OX_size=5.
board=XXXXXXXX.XOXOXOXOXOXOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=8, OX_size=6.
board=XXXXXXXXOX.XOXOXOXOXOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=10, OX_size=6.
board=XXXXXXXXOXOX.XOXOXOXOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=12, OX_size=6.
board=XXXXXXXXOXOXOX.XOXOXOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=14, OX_size=6.
board=XXXXXXXXOXOXOXOX.XOXOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=16, OX_size=6.
board=XXXXXXXXOXOXOXOXOX.XOXOOOOOOOOO, board_size=31, X_size=9, dot_pos=18, OX_size=6.
board=XXXXXXXXOXOXOXOXOXOX.XOOOOOOOOO, board_size=31, X_size=9, dot_pos=20, OX_size=6.
board=XXXXXXXXOXOXOXOXOXOXOX.OOOOOOOO, board_size=31, X_size=9, dot_pos=22, OX_size=6.
board=XXXXXXXXOXOXOXOXOXOXOXO.OOOOOOO, board_size=31, X_size=8, dot_pos=23, OX_size=7.
board=XXXXXXXXOXOXOXOXOXOXO.OXOOOOOOO, board_size=31, X_size=8, dot_pos=21, OX_size=7.
board=XXXXXXXXOXOXOXOXOXO.OXOXOOOOOOO, board_size=31, X_size=8, dot_pos=19, OX_size=7.
board=XXXXXXXXOXOXOXOXO.OXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=17, OX_size=7.
board=XXXXXXXXOXOXOXO.OXOXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=15, OX_size=7.
board=XXXXXXXXOXOXO.OXOXOXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=13, OX_size=7.
board=XXXXXXXXOXO.OXOXOXOXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=11, OX_size=7.
board=XXXXXXXXO.OXOXOXOXOXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=9, OX_size=7.
board=XXXXXXX.OXOXOXOXOXOXOXOXOOOOOOO, board_size=31, X_size=8, dot_pos=7, OX_size=7.
board=XXXXXX.XOXOXOXOXOXOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=6, OX_size=8.
board=XXXXXXOX.XOXOXOXOXOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=8, OX_size=8.
board=XXXXXXOXOX.XOXOXOXOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=10, OX_size=8.
board=XXXXXXOXOXOX.XOXOXOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=12, OX_size=8.
board=XXXXXXOXOXOXOX.XOXOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=14, OX_size=8.
board=XXXXXXOXOXOXOXOX.XOXOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=16, OX_size=8.
board=XXXXXXOXOXOXOXOXOX.XOXOXOOOOOOO, board_size=31, X_size=7, dot_pos=18, OX_size=8.
board=XXXXXXOXOXOXOXOXOXOX.XOXOOOOOOO, board_size=31, X_size=7, dot_pos=20, OX_size=8.
board=XXXXXXOXOXOXOXOXOXOXOX.XOOOOOOO, board_size=31, X_size=7, dot_pos=22, OX_size=8.
board=XXXXXXOXOXOXOXOXOXOXOXOX.OOOOOO, board_size=31, X_size=7, dot_pos=24, OX_size=8.
board=XXXXXXOXOXOXOXOXOXOXOXOXO.OOOOO, board_size=31, X_size=6, dot_pos=25, OX_size=9.
board=XXXXXXOXOXOXOXOXOXOXOXO.OXOOOOO, board_size=31, X_size=6, dot_pos=23, OX_size=9.
board=XXXXXXOXOXOXOXOXOXOXO.OXOXOOOOO, board_size=31, X_size=6, dot_pos=21, OX_size=9.
board=XXXXXXOXOXOXOXOXOXO.OXOXOXOOOOO, board_size=31, X_size=6, dot_pos=19, OX_size=9.
board=XXXXXXOXOXOXOXOXO.OXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=17, OX_size=9.
board=XXXXXXOXOXOXOXO.OXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=15, OX_size=9.
board=XXXXXXOXOXOXO.OXOXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=13, OX_size=9.
board=XXXXXXOXOXO.OXOXOXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=11, OX_size=9.
board=XXXXXXOXO.OXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=9, OX_size=9.
board=XXXXXXO.OXOXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=7, OX_size=9.
board=XXXXX.OXOXOXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=6, dot_pos=5, OX_size=9.
board=XXXX.XOXOXOXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=4, OX_size=10.
board=XXXXOX.XOXOXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=6, OX_size=10.
board=XXXXOXOX.XOXOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=8, OX_size=10.
board=XXXXOXOXOX.XOXOXOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=10, OX_size=10.
board=XXXXOXOXOXOX.XOXOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=12, OX_size=10.
board=XXXXOXOXOXOXOX.XOXOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=14, OX_size=10.
board=XXXXOXOXOXOXOXOX.XOXOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=16, OX_size=10.
board=XXXXOXOXOXOXOXOXOX.XOXOXOXOOOOO, board_size=31, X_size=5, dot_pos=18, OX_size=10.
board=XXXXOXOXOXOXOXOXOXOX.XOXOXOOOOO, board_size=31, X_size=5, dot_pos=20, OX_size=10.
board=XXXXOXOXOXOXOXOXOXOXOX.XOXOOOOO, board_size=31, X_size=5, dot_pos=22, OX_size=10.
board=XXXXOXOXOXOXOXOXOXOXOXOX.XOOOOO, board_size=31, X_size=5, dot_pos=24, OX_size=10.
board=XXXXOXOXOXOXOXOXOXOXOXOXOX.OOOO, board_size=31, X_size=5, dot_pos=26, OX_size=10.
board=XXXXOXOXOXOXOXOXOXOXOXOXOXO.OOO, board_size=31, X_size=4, dot_pos=27, OX_size=11.
board=XXXXOXOXOXOXOXOXOXOXOXOXO.OXOOO, board_size=31, X_size=4, dot_pos=25, OX_size=11.
board=XXXXOXOXOXOXOXOXOXOXOXO.OXOXOOO, board_size=31, X_size=4, dot_pos=23, OX_size=11.
board=XXXXOXOXOXOXOXOXOXOXO.OXOXOXOOO, board_size=31, X_size=4, dot_pos=21, OX_size=11.
board=XXXXOXOXOXOXOXOXOXO.OXOXOXOXOOO, board_size=31, X_size=4, dot_pos=19, OX_size=11.
board=XXXXOXOXOXOXOXOXO.OXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=17, OX_size=11.
board=XXXXOXOXOXOXOXO.OXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=15, OX_size=11.
board=XXXXOXOXOXOXO.OXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=13, OX_size=11.
board=XXXXOXOXOXO.OXOXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=11, OX_size=11.
board=XXXXOXOXO.OXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=9, OX_size=11.
board=XXXXOXO.OXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=7, OX_size=11.
board=XXXXO.OXOXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=5, OX_size=11.
board=XXX.OXOXOXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=4, dot_pos=3, OX_size=11.
board=XX.XOXOXOXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=2, OX_size=12.
board=XXOX.XOXOXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=4, OX_size=12.
board=XXOXOX.XOXOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=6, OX_size=12.
board=XXOXOXOX.XOXOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=8, OX_size=12.
board=XXOXOXOXOX.XOXOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=10, OX_size=12.
board=XXOXOXOXOXOX.XOXOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=12, OX_size=12.
board=XXOXOXOXOXOXOX.XOXOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=14, OX_size=12.
board=XXOXOXOXOXOXOXOX.XOXOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=16, OX_size=12.
board=XXOXOXOXOXOXOXOXOX.XOXOXOXOXOOO, board_size=31, X_size=3, dot_pos=18, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOX.XOXOXOXOOO, board_size=31, X_size=3, dot_pos=20, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOXOX.XOXOXOOO, board_size=31, X_size=3, dot_pos=22, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOXOXOX.XOXOOO, board_size=31, X_size=3, dot_pos=24, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOXOXOXOX.XOOO, board_size=31, X_size=3, dot_pos=26, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOXOXOXOXOX.OO, board_size=31, X_size=3, dot_pos=28, OX_size=12.
board=XXOXOXOXOXOXOXOXOXOXOXOXOXOXO.O, board_size=31, X_size=2, dot_pos=29, OX_size=13.
board=XXOXOXOXOXOXOXOXOXOXOXOXOXO.OXO, board_size=31, X_size=2, dot_pos=27, OX_size=13.
board=XXOXOXOXOXOXOXOXOXOXOXOXO.OXOXO, board_size=31, X_size=2, dot_pos=25, OX_size=13.
board=XXOXOXOXOXOXOXOXOXOXOXO.OXOXOXO, board_size=31, X_size=2, dot_pos=23, OX_size=13.
board=XXOXOXOXOXOXOXOXOXOXO.OXOXOXOXO, board_size=31, X_size=2, dot_pos=21, OX_size=13.
board=XXOXOXOXOXOXOXOXOXO.OXOXOXOXOXO, board_size=31, X_size=2, dot_pos=19, OX_size=13.
board=XXOXOXOXOXOXOXOXO.OXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=17, OX_size=13.
board=XXOXOXOXOXOXOXO.OXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=15, OX_size=13.
board=XXOXOXOXOXOXO.OXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=13, OX_size=13.
board=XXOXOXOXOXO.OXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=11, OX_size=13.
board=XXOXOXOXO.OXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=9, OX_size=13.
board=XXOXOXO.OXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=7, OX_size=13.
board=XXOXO.OXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=5, OX_size=13.
board=XXO.OXOXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=3, OX_size=13.
board=X.OXOXOXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=2, dot_pos=1, OX_size=13.
board=.XOXOXOXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=0, OX_size=14.
board=OX.XOXOXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=2, OX_size=14.
board=OXOX.XOXOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=4, OX_size=14.
board=OXOXOX.XOXOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=6, OX_size=14.
board=OXOXOXOX.XOXOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=8, OX_size=14.
board=OXOXOXOXOX.XOXOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=10, OX_size=14.
board=OXOXOXOXOXOX.XOXOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=12, OX_size=14.
board=OXOXOXOXOXOXOX.XOXOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=14, OX_size=14.
board=OXOXOXOXOXOXOXOX.XOXOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=16, OX_size=14.
board=OXOXOXOXOXOXOXOXOX.XOXOXOXOXOXO, board_size=31, X_size=1, dot_pos=18, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOX.XOXOXOXOXO, board_size=31, X_size=1, dot_pos=20, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOX.XOXOXOXO, board_size=31, X_size=1, dot_pos=22, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOX.XOXOXO, board_size=31, X_size=1, dot_pos=24, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXOX.XOXO, board_size=31, X_size=1, dot_pos=26, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXOXOX.XO, board_size=31, X_size=1, dot_pos=28, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXOXOXOX., board_size=31, X_size=1, dot_pos=30, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXOXOXO.X, board_size=31, X_size=1, dot_pos=29, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXOXO.OXX, board_size=31, X_size=1, dot_pos=27, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXOXO.OXOXX, board_size=31, X_size=1, dot_pos=25, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXOXO.OXOXOXX, board_size=31, X_size=1, dot_pos=23, OX_size=14.
board=OXOXOXOXOXOXOXOXOXOXO.OXOXOXOXX, board_size=31, X_size=1, dot_pos=21, OX_size=14.
board=OXOXOXOXOXOXOXOXOXO.OXOXOXOXOXX, board_size=31, X_size=1, dot_pos=19, OX_size=14.
board=OXOXOXOXOXOXOXOXO.OXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=17, OX_size=14.
board=OXOXOXOXOXOXOXO.OXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=15, OX_size=14.
board=OXOXOXOXOXOXO.OXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=13, OX_size=14.
board=OXOXOXOXOXO.OXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=11, OX_size=14.
board=OXOXOXOXO.OXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=9, OX_size=14.
board=OXOXOXO.OXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=7, OX_size=14.
board=OXOXO.OXOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=5, OX_size=14.
board=OXO.OXOXOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=3, OX_size=14.
board=O.OXOXOXOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=1, dot_pos=1, OX_size=14.
board=OO.XOXOXOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=2, OX_size=13.
board=OOOX.XOXOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=4, OX_size=13.
board=OOOXOX.XOXOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=6, OX_size=13.
board=OOOXOXOX.XOXOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=8, OX_size=13.
board=OOOXOXOXOX.XOXOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=10, OX_size=13.
board=OOOXOXOXOXOX.XOXOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=12, OX_size=13.
board=OOOXOXOXOXOXOX.XOXOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=14, OX_size=13.
board=OOOXOXOXOXOXOXOX.XOXOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=16, OX_size=13.
board=OOOXOXOXOXOXOXOXOX.XOXOXOXOXOXX, board_size=31, X_size=2, dot_pos=18, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOX.XOXOXOXOXX, board_size=31, X_size=2, dot_pos=20, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOXOX.XOXOXOXX, board_size=31, X_size=2, dot_pos=22, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOXOXOX.XOXOXX, board_size=31, X_size=2, dot_pos=24, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOXOXOXOX.XOXX, board_size=31, X_size=2, dot_pos=26, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOXOXOXOXOX.XX, board_size=31, X_size=2, dot_pos=28, OX_size=13.
board=OOOXOXOXOXOXOXOXOXOXOXOXOXO.XXX, board_size=31, X_size=3, dot_pos=27, OX_size=12.
board=OOOXOXOXOXOXOXOXOXOXOXOXO.OXXXX, board_size=31, X_size=3, dot_pos=25, OX_size=12.
board=OOOXOXOXOXOXOXOXOXOXOXO.OXOXXXX, board_size=31, X_size=3, dot_pos=23, OX_size=12.
board=OOOXOXOXOXOXOXOXOXOXO.OXOXOXXXX, board_size=31, X_size=3, dot_pos=21, OX_size=12.
board=OOOXOXOXOXOXOXOXOXO.OXOXOXOXXXX, board_size=31, X_size=3, dot_pos=19, OX_size=12.
board=OOOXOXOXOXOXOXOXO.OXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=17, OX_size=12.
board=OOOXOXOXOXOXOXO.OXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=15, OX_size=12.
board=OOOXOXOXOXOXO.OXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=13, OX_size=12.
board=OOOXOXOXOXO.OXOXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=11, OX_size=12.
board=OOOXOXOXO.OXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=9, OX_size=12.
board=OOOXOXO.OXOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=7, OX_size=12.
board=OOOXO.OXOXOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=5, OX_size=12.
board=OOO.OXOXOXOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=3, dot_pos=3, OX_size=12.
board=OOOO.XOXOXOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=4, OX_size=11.
board=OOOOOX.XOXOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=6, OX_size=11.
board=OOOOOXOX.XOXOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=8, OX_size=11.
board=OOOOOXOXOX.XOXOXOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=10, OX_size=11.
board=OOOOOXOXOXOX.XOXOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=12, OX_size=11.
board=OOOOOXOXOXOXOX.XOXOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=14, OX_size=11.
board=OOOOOXOXOXOXOXOX.XOXOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=16, OX_size=11.
board=OOOOOXOXOXOXOXOXOX.XOXOXOXOXXXX, board_size=31, X_size=4, dot_pos=18, OX_size=11.
board=OOOOOXOXOXOXOXOXOXOX.XOXOXOXXXX, board_size=31, X_size=4, dot_pos=20, OX_size=11.
board=OOOOOXOXOXOXOXOXOXOXOX.XOXOXXXX, board_size=31, X_size=4, dot_pos=22, OX_size=11.
board=OOOOOXOXOXOXOXOXOXOXOXOX.XOXXXX, board_size=31, X_size=4, dot_pos=24, OX_size=11.
board=OOOOOXOXOXOXOXOXOXOXOXOXOX.XXXX, board_size=31, X_size=4, dot_pos=26, OX_size=11.
board=OOOOOXOXOXOXOXOXOXOXOXOXO.XXXXX, board_size=31, X_size=5, dot_pos=25, OX_size=10.
board=OOOOOXOXOXOXOXOXOXOXOXO.OXXXXXX, board_size=31, X_size=5, dot_pos=23, OX_size=10.
board=OOOOOXOXOXOXOXOXOXOXO.OXOXXXXXX, board_size=31, X_size=5, dot_pos=21, OX_size=10.
board=OOOOOXOXOXOXOXOXOXO.OXOXOXXXXXX, board_size=31, X_size=5, dot_pos=19, OX_size=10.
board=OOOOOXOXOXOXOXOXO.OXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=17, OX_size=10.
board=OOOOOXOXOXOXOXO.OXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=15, OX_size=10.
board=OOOOOXOXOXOXO.OXOXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=13, OX_size=10.
board=OOOOOXOXOXO.OXOXOXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=11, OX_size=10.
board=OOOOOXOXO.OXOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=9, OX_size=10.
board=OOOOOXO.OXOXOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=7, OX_size=10.
board=OOOOO.OXOXOXOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=5, dot_pos=5, OX_size=10.
board=OOOOOO.XOXOXOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=6, OX_size=9.
board=OOOOOOOX.XOXOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=8, OX_size=9.
board=OOOOOOOXOX.XOXOXOXOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=10, OX_size=9.
board=OOOOOOOXOXOX.XOXOXOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=12, OX_size=9.
board=OOOOOOOXOXOXOX.XOXOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=14, OX_size=9.
board=OOOOOOOXOXOXOXOX.XOXOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=16, OX_size=9.
board=OOOOOOOXOXOXOXOXOX.XOXOXOXXXXXX, board_size=31, X_size=6, dot_pos=18, OX_size=9.
board=OOOOOOOXOXOXOXOXOXOX.XOXOXXXXXX, board_size=31, X_size=6, dot_pos=20, OX_size=9.
board=OOOOOOOXOXOXOXOXOXOXOX.XOXXXXXX, board_size=31, X_size=6, dot_pos=22, OX_size=9.
board=OOOOOOOXOXOXOXOXOXOXOXOX.XXXXXX, board_size=31, X_size=6, dot_pos=24, OX_size=9.
board=OOOOOOOXOXOXOXOXOXOXOXO.XXXXXXX, board_size=31, X_size=7, dot_pos=23, OX_size=8.
board=OOOOOOOXOXOXOXOXOXOXO.OXXXXXXXX, board_size=31, X_size=7, dot_pos=21, OX_size=8.
board=OOOOOOOXOXOXOXOXOXO.OXOXXXXXXXX, board_size=31, X_size=7, dot_pos=19, OX_size=8.
board=OOOOOOOXOXOXOXOXO.OXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=17, OX_size=8.
board=OOOOOOOXOXOXOXO.OXOXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=15, OX_size=8.
board=OOOOOOOXOXOXO.OXOXOXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=13, OX_size=8.
board=OOOOOOOXOXO.OXOXOXOXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=11, OX_size=8.
board=OOOOOOOXO.OXOXOXOXOXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=9, OX_size=8.
board=OOOOOOO.OXOXOXOXOXOXOXOXXXXXXXX, board_size=31, X_size=7, dot_pos=7, OX_size=8.
board=OOOOOOOO.XOXOXOXOXOXOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=8, OX_size=7.
board=OOOOOOOOOX.XOXOXOXOXOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=10, OX_size=7.
board=OOOOOOOOOXOX.XOXOXOXOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=12, OX_size=7.
board=OOOOOOOOOXOXOX.XOXOXOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=14, OX_size=7.
board=OOOOOOOOOXOXOXOX.XOXOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=16, OX_size=7.
board=OOOOOOOOOXOXOXOXOX.XOXOXXXXXXXX, board_size=31, X_size=8, dot_pos=18, OX_size=7.
board=OOOOOOOOOXOXOXOXOXOX.XOXXXXXXXX, board_size=31, X_size=8, dot_pos=20, OX_size=7.
board=OOOOOOOOOXOXOXOXOXOXOX.XXXXXXXX, board_size=31, X_size=8, dot_pos=22, OX_size=7.
board=OOOOOOOOOXOXOXOXOXOXO.XXXXXXXXX, board_size=31, X_size=9, dot_pos=21, OX_size=6.
board=OOOOOOOOOXOXOXOXOXO.OXXXXXXXXXX, board_size=31, X_size=9, dot_pos=19, OX_size=6.
board=OOOOOOOOOXOXOXOXO.OXOXXXXXXXXXX, board_size=31, X_size=9, dot_pos=17, OX_size=6.
board=OOOOOOOOOXOXOXO.OXOXOXXXXXXXXXX, board_size=31, X_size=9, dot_pos=15, OX_size=6.
board=OOOOOOOOOXOXO.OXOXOXOXXXXXXXXXX, board_size=31, X_size=9, dot_pos=13, OX_size=6.
board=OOOOOOOOOXO.OXOXOXOXOXXXXXXXXXX, board_size=31, X_size=9, dot_pos=11, OX_size=6.
board=OOOOOOOOO.OXOXOXOXOXOXXXXXXXXXX, board_size=31, X_size=9, dot_pos=9, OX_size=6.
board=OOOOOOOOOO.XOXOXOXOXOXXXXXXXXXX, board_size=31, X_size=10, dot_pos=10, OX_size=5.
board=OOOOOOOOOOOX.XOXOXOXOXXXXXXXXXX, board_size=31, X_size=10, dot_pos=12, OX_size=5.
board=OOOOOOOOOOOXOX.XOXOXOXXXXXXXXXX, board_size=31, X_size=10, dot_pos=14, OX_size=5.
board=OOOOOOOOOOOXOXOX.XOXOXXXXXXXXXX, board_size=31, X_size=10, dot_pos=16, OX_size=5.
board=OOOOOOOOOOOXOXOXOX.XOXXXXXXXXXX, board_size=31, X_size=10, dot_pos=18, OX_size=5.
board=OOOOOOOOOOOXOXOXOXOX.XXXXXXXXXX, board_size=31, X_size=10, dot_pos=20, OX_size=5.
board=OOOOOOOOOOOXOXOXOXO.XXXXXXXXXXX, board_size=31, X_size=11, dot_pos=19, OX_size=4.
board=OOOOOOOOOOOXOXOXO.OXXXXXXXXXXXX, board_size=31, X_size=11, dot_pos=17, OX_size=4.
board=OOOOOOOOOOOXOXO.OXOXXXXXXXXXXXX, board_size=31, X_size=11, dot_pos=15, OX_size=4.
board=OOOOOOOOOOOXO.OXOXOXXXXXXXXXXXX, board_size=31, X_size=11, dot_pos=13, OX_size=4.
board=OOOOOOOOOOO.OXOXOXOXXXXXXXXXXXX, board_size=31, X_size=11, dot_pos=11, OX_size=4.
board=OOOOOOOOOOOO.XOXOXOXXXXXXXXXXXX, board_size=31, X_size=12, dot_pos=12, OX_size=3.
board=OOOOOOOOOOOOOX.XOXOXXXXXXXXXXXX, board_size=31, X_size=12, dot_pos=14, OX_size=3.
board=OOOOOOOOOOOOOXOX.XOXXXXXXXXXXXX, board_size=31, X_size=12, dot_pos=16, OX_size=3.
board=OOOOOOOOOOOOOXOXOX.XXXXXXXXXXXX, board_size=31, X_size=12, dot_pos=18, OX_size=3.
board=OOOOOOOOOOOOOXOXO.XXXXXXXXXXXXX, board_size=31, X_size=13, dot_pos=17, OX_size=2.
board=OOOOOOOOOOOOOXO.OXXXXXXXXXXXXXX, board_size=31, X_size=13, dot_pos=15, OX_size=2.
board=OOOOOOOOOOOOO.OXOXXXXXXXXXXXXXX, board_size=31, X_size=13, dot_pos=13, OX_size=2.
board=OOOOOOOOOOOOOO.XOXXXXXXXXXXXXXX, board_size=31, X_size=14, dot_pos=14, OX_size=1.
board=OOOOOOOOOOOOOOOX.XXXXXXXXXXXXXX, board_size=31, X_size=14, dot_pos=16, OX_size=1.
board=OOOOOOOOOOOOOOO.XXXXXXXXXXXXXXX, board_size=31, X_size=15, dot_pos=15, OX_size=0.
```

It's curious how the space goes like a pendulum slowly out and then back. 
