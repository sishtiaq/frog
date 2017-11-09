
#include <stdio.h>
#include <stdlib.h>


#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif

#define DEBUG FALSE // TRUE

/* Eg, index  0|  |d|        |b
       char   X^k  . OX^n O^k   */
typedef struct game
{
  char *board;
  int board_size;// b
  int X_size; // k
  int dot_pos;// d
  int OX_size;// n
} game;


void game_trace(game *g)
{
  printf("board=%s, board_size=%d, X_size=%d, dot_pos=%d, OX_size=%d.\n",
         g->board,g->board_size,g->X_size,g->dot_pos,g->OX_size);

}


void game_swap(game *g, int p, int q)
/* swap g->board[p] with g->board[q].
 * update dot_pos (either p or q *has* to be dot_pos).
 */
{
  char t;
  
  if ((g->dot_pos)==p)
  {
    g->dot_pos=q;
  }
  else
  {
    g->dot_pos=p;
  }

  t = g->board[p];
  g->board[p] = g->board[q];
  g->board[q] = t;

}

  
void game_init(game *g, int n)
{
  int i;

  g->board_size = 2*n+1;
  g->board = (char *) malloc(g->board_size + 1 * sizeof(char) );
  
  g->X_size=n;
  g->dot_pos = n;
  g->OX_size = 0;
  
  for (i=0; i<n; i++)
  {
    g->board[i] = 'X';
  }
  g->board[i] = '.';
  i++;
  for (i=i; i<(g->board_size); i++)
  {
    g->board[i] = 'O';
  }

  
}

void game_delete(game *g)
{
  free(g->board);
}


void game_right_unroll_lemma(game *g, int l)
{
/* X((OX)^l)O. ->*  .((OX)^(l+1)) */
  if (DEBUG) printf("game_right_unroll_lemma\n");
  if (l==0)
  {
    // XO.
    game_swap(g,(g->dot_pos)-2,g->dot_pos);
    game_trace(g);
    // .OX
  }
  else if (l>0)
  {
    // X((OX)^(l-1))OXO.
    game_swap(g,(g->dot_pos)-2,g->dot_pos);
    game_trace(g);
    // X((OX)^(l-1))O.OX
    l--;
    game_right_unroll_lemma(g,l);
    // .((OX)^l)XO = .(OX)^(l+1)
  }
  
}

void game_right_unroll(game *g)
{
  // X^k(OX)^n.O^k
  if (DEBUG) printf("game_right_unroll\n");

  if ((g->OX_size)==0)
  {
    // (X^k-1)X.O(O^k-1)
    game_swap(g,(g->dot_pos)-1,g->dot_pos);
    game_trace(g);
    // (X^k-1).XO(O^k-1)
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    (g->X_size)--;
    (g->OX_size)++;
    // (X^k-1)OX.(O^k-1)
    game_trace(g);
  }
  else if ((g->OX_size)>0)
  {
    // (X^k-1)X(OX)^n.O(O^k-1)
    game_swap(g,g->dot_pos,(g->dot_pos)+1);
    game_trace(g);    
    // (X^k-1)X(OX)^nO.(O^k-1)
    game_right_unroll_lemma(g,g->OX_size);
    // (X^k-1).(OX^(n+1))(O^k-1)
    (g->X_size)--;
    (g->OX_size)++;
    //game_trace(g,TRUE);
  }  
}

void game_left_unroll_lemma(game *g, int l)
/* .X((OX)^l)O  ->*  (OX)^(l+1). */
{
  if (DEBUG) printf("game_left_unroll_lemma\n");  
  if (l==0)
  { 
    // .XO
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    // OX.
  }
  else if (l>0)
  {
    // .XOX(OX^l-1)O
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);    
    // OX.X(OX^(l-1))O
    l--;
    game_left_unroll_lemma(g,l);
    // OX((OX)^(l+1)). = OX^(l+2).
  }
  
}

void game_left_unroll(game *g)
{
  // X^k.{OX}^n{O}^k
  if (DEBUG) printf("game_left_unroll\n");

  game_swap(g,(g->dot_pos)-1,g->dot_pos);
  game_trace(g);

  // (X^k-1).X{OX}^n{O}^k  
  if ((g->OX_size)==0)
  {
    // (X^k-1).XO(O^k-1)
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    (g->X_size)--;
    (g->OX_size)++;
    // (X^k-1)OX.(O^k-1)
    game_trace(g);
  }
  else if ((g->OX_size)>0)
  {
    // (X^k-1).XOX(OX^n-1)O(O^k-1)
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    // (X^k-1)OX.X(OX^n-1)O(O^k-1)    
    game_left_unroll_lemma(g,(g->OX_size)-1);
    // (X^k-1)OX(OX^n).(O^k-1)        
    (g->X_size)--;
    (g->OX_size)++;
    //game_trace(g,TRUE);    
  }
}

void game_unroll(game *g)
{
  // X^k.(OX)^pO^k
  if (DEBUG) printf("game_unroll\n");
  
  if ((g->dot_pos) == (g->X_size))
    game_left_unroll(g);
  else 
  {
    // X^k(OX)^p.O^k
    if ( (g->dot_pos) == ((g->X_size)+((g->OX_size)*2)) )
      game_right_unroll(g);
    else
    {
      fprintf(stderr,
              "board_unroll: board isn't of the form X^k.{OX}^nO^k or X^k{OX}^n.O^k\n");
      fprintf(stderr,
              "board=%s, board_size=%d, X_size=%d, dot_pos=%d, OX_size=%d\n",
              g->board,g->board_size,g->X_size,g->dot_pos,g->OX_size);
      fprintf(stderr,"Exiting ...\n");
      exit(-1);
    }
  }
}


void game_right_roll_lemma(game *g, int l)
// (XO)^l. ->*   .(OX)^l
{
  if (DEBUG) printf("game_right_roll_lemma\n");
  
  // (XO)^l.
  if (l==1)
  {
    // (XO).
    game_swap(g,(g->dot_pos)-2,g->dot_pos);
    // .OX
    game_trace(g);    
  }
  else if (l>1)
  {
    // XO((XO)^(l-1)).
    l--;
    game_right_roll_lemma(g,l);
    // XO.((OX)^(l-1))
    game_swap(g,(g->dot_pos)-2,g->dot_pos);
    // .OX((OX)^(l-1))    
    // = .((OX)^l)
    game_trace(g);
  }
  
}

void game_right_roll(game *g)
// (O^k)((XO)^p).X^k ->* O^(k+1).((XO)^(p-1))(X^(k+1)
{
  if (DEBUG) printf("game_right_roll\n");

  // (O^k)((XO)^p).X^k  
  if ((g->OX_size)==1)
  {
    // (O^k)(XO).X^k
    game_swap(g,(g->dot_pos)-2,g->dot_pos);
    game_trace(g);
    // (O^k).OXX^k
    game_swap(g,(g->dot_pos)-1,g->dot_pos);
    // (O^k)O.XX^k    
    (g->X_size)++;
    (g->OX_size)--;
    // (O^(k+1)).(X^(k+1))
    game_trace(g);
  }
  else if ((g->OX_size)>1)
  {
    // (O^k)(XO)^p.X^k
    game_right_roll_lemma(g,(g->OX_size));
    // (O^k).(OX)^pX^k
    // (O^k).OX((OX)^(p-1))X^k    
    game_swap(g,g->dot_pos,(g->dot_pos)+1);
    // (O^k)O.X((OX)^(p-1))X^k        
    (g->X_size)++;
    (g->OX_size)--;    
    // = (O^(k+1)).((XO)^(p-1))(X^(k+1))
    game_trace(g);

  }
  
}

void game_left_roll_lemma(game *g, int l)
// .(XO)^l ->* (OX)^l.
{
  if (DEBUG) printf("game_left_roll_lemma \n");  

  if (l==1)
  {
    // .XO 
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    // OX.
  }
  else if (l>1)
  {
    // .XO((XO)^(l-1))
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    l--;
    // OX.((XO)^(l-1))    
    game_left_roll_lemma(g,l);
    // OX((OX)^(l-1)). = (OX^l).
  }
  
    
}

void game_left_roll(game *g)
// (O^k).((XO)^p)X^k ->* O^(k+1)((XO)^(p-1)).(X^(k+1)
{
  if (DEBUG) printf("game_left_roll\n");

  // O^k.((XO)^p)X^k
  if ((g->OX_size)==1)
  {
    // O^k.(XO)X^k    
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    // O^kOX.X^k        
    game_swap(g,(g->dot_pos)-1,g->dot_pos);
    // O^kO.XX^k
    (g->X_size)++;
    (g->OX_size)--;
    game_trace(g);
  }
  else if ((g->OX_size)>1)
  {
    // O^k.XO((XO)^(p-1))X^k    
    game_swap(g,g->dot_pos,(g->dot_pos)+2);
    game_trace(g);
    // O^kOX.((XO)^(p-1))X^k
    game_left_roll_lemma(g,(g->OX_size)-1);
    // O^kOX((OX)^(p-1)).X^k    
    game_swap(g,(g->dot_pos)-1,g->dot_pos);
    // (O^k+1)((XO)^(p-1)).XX^k    
    (g->X_size)++;
    (g->OX_size)--;    
    game_trace(g);

  }
  
}


void game_roll(game *g)
{
  if (DEBUG) printf("game_roll\n");
  
  // O^k.((XO)^p)K^k
  if ((g->dot_pos) == (g->X_size))
    game_left_roll(g);
  else
  {
  // O^k((XO)^p).K^k    
    if ( (g->dot_pos) == ((g->X_size)+((g->OX_size)*2)) )
      game_right_roll(g);
    else
    {
      fprintf(stderr,
              "board_roll: board isn't of the form X^k.{XO}^nO^k or X^k{XO}^n.O^k\n");
      fprintf(stderr,
              "board=%s, board_size=%d, X_size=%d, dot_pos=%d, OX_size=%d\n",
              g->board,g->board_size,g->X_size,g->dot_pos,g->OX_size);
      fprintf(stderr,"Exiting ...\n");
      exit(-2);
    }
  }
  
}


int main()
{
  game g;
  int i;
  int not_ok = TRUE;
  int steps = 0;
  
  while (not_ok)
  {
    printf("Enter the number of frogs on one side ==> ");
    scanf("%d",&i);
    if (i>0) not_ok=FALSE;
  }

  // X_size = i (what the user said, like)
  // board_size = 2*i+1 (i Ks + i Os + 1 .)
  // dot_pos = i (cos board is indexed from 0)
  // OX_size = 0 (no swaps yet)
  game_init(&g,i);

  printf("Initial board = ");
  game_trace(&g);
  printf("Number of swaps needed to finish=%d.\n",i*(i+2));

  if (DEBUG) printf("Unrolling ... \n");
  game_trace(&g);
  // X^k.O^k
  while (g.X_size>0)
  {
    game_unroll(&g);
  }
  // .(OX)^(k/2) or (OX)^(k/2).

  if (DEBUG) printf("Minor swap ...\n");
  if ((g.dot_pos) == 0)
  {
    // .(OX)^(k/2)     
    game_swap(&g,g.dot_pos,(g.dot_pos)+1);
  }
  else
  {
    // (OX)^(k/2).
    game_swap(&g,(g.board_size)-1,(g.board_size)-2);
  }
  (g.X_size)++;
  (g.OX_size)--;
  game_trace(&g);
  // O.(XO)^(k/2)X or O(XO)^(k/2).X  

  if (DEBUG) printf("And rolling ...\n");
  while (g.X_size<i)
  {
    // O^k.(XO)^nX^k or O^k(XO)^n.X^k
    game_roll(&g);
    // O^(k+1).(XO)^(n-1)(X^(k+1)) or O^(k+1)(XO)^(n-1).(X^(k+1))
  }

  game_delete(&g);
  
  
}









  
