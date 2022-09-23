/* This is a cellular automation simulator.  The rules are very
 * simple.  The board is a grid.  Each grid square can hold at most
 * one cell.  For a cell to survive from generation to generation it
 * needs two or three neighbors.  Too many neighbors causes the cell
 * to starve.  Too few causes an incurable case of ennui.  All is not
 * lost.  If a grid square has exactly three neighbors, a new cell is
 * born, which is kind of strange when you think about it.
 *
 * There are lots of variations on this game.  Technically, this is Conway's
 * Game of Life and is a B3/S23 game.  That is, cells survive with two or three
 * neighbors and are born with exactly 3 neighbors.  This game can also be
 * extended into the real domain or more dimenstions.  We, however, with stick
 * with Conway's original automation.
 *
 * Below is a very simple implementation.  This is your chance to apply some of
 * the loop optimization techniques we've been talking about in class: 
 *  1) Remove loop inefficiencies
 *  2) Reduce procedure calls
 *  3) Reduce unnecessary memory references
 *  4) Loop unrolling
 * There are, of course, more optimizations that you can apply.  For example,
 * moving sequentially through memory is more efficient than jumping around.
 *  

 * C stores 2D arrays Row Major Order.  That is to say that cell
 * A[0][WIDTH-1] is right next to A[1][0] in memory.  If you notice the nested
 * loops in evolve (below) I'm incrementing the column index, i, more quickly
 * than the row index, j.  Try reversing these and see what happens.
 *
 * The experimental set-up is implemented in a library called liblife.a  Since
 * it has solutions, it is providing it as a binary in:
 *
 *   /home/akters/lib/liblife.a
 *
 * The provided make file links to this library.
 *
 * There are three ways to run the test program: with a window (full
 * GUI), silently, or lazy GUI. Without a window (silent), the program
 * runs the test cases against the provided evolution methods as
 * quickly as possible.  After about 500 generations, it stops and
 * reports the average performance of each method.
 *
 * The following flags are available
 *
 * silent mode (default)
 * -lg : lazy GUI
 * -fg : full GUI
 *
 * See how fast you can make your code.
 *
 * You are free to add as many test methods as you want.  You are
 * encouraged to do this.  Copy the code, give it a new name, and
 * add it to the library's suite of methods.  This way you can really
 * see if you are making improvements.  Have fun.
 *
 */

#include "liblife.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "optimized.c"

int generation = 0;
int printLazy = 0;

/* Simple Life evolution.  These functions implement Conway's game of
 * life rules.  Your millage may vary, void where prohibited.
 */
static int min(int x, int y) {
   return x < y ? x : y;
}
static int max(int x, int y) {
   return x < y ? y : x;
}

/* Neighbors: Given the board b, this function counts the number of
 * neighbors of cell-(i,j).
 *
 * This implementation over-counts the neighborhood.  The loop counts the
 * cell-(i,j) and then subtracts off one if it is alive. 
 *
 * The use of min and max ensure that the neighborhood is truncated at the edges
 * of the board.  
 */
static int neighbors (board b, int i, int j)
{
   int n = 0;
   int i_left = max(0,i-1);
   int i_right  = min(HEIGHT, i+2);
   int j_left = max(0,j-1);
   int j_right  = min(WIDTH, j+2);
   int ii, jj;

   for (ii = i_left; ii < i_right; ++ii) {
      for (jj = j_left; jj < j_right; ++jj) {
         n += b[ii][jj];
      }
   }

   return n - b[i][j];
}

/* lazyPrint: A not-too-elegant print function that prints the
 * upper-left 10 x 10 cells of the simulation board, and sleeps for 1
 * second.
 */
void lazyPrint(board prv){

  for (int i=0; i<10; i++){
    for (int j=0; j<10; j++){
      printf("%d",prv[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  // sleep 1 seconds
  unsigned int time_to_sleep = 1; 
  while(time_to_sleep){
    time_to_sleep = sleep(time_to_sleep);
  }
}

/* Evolve: This is a very simple evolution function.  It applies the
 * rules of Conway's Game of Live as written.  There are a lot of
 * improvements that you can make.  Good luck beating Ada, she is
 * really good.
 */
void evolve(board prv, board nxt){
  
   int i, j;
   int n;

   printf("\rGeneration %d\n", generation++);
   if (printLazy == 1){
     lazyPrint(prv);
   }
   
   for (j=0; j < WIDTH; ++j) {
      for (i = 0; i < HEIGHT; ++i) {
         n = neighbors(prv, i, j);
         if (prv[i][j] && (n == 3 || n == 2))
            nxt[i][j] = true;
         else if (!prv[i][j] && (n == 3)) 
            nxt[i][j] = true;
         else
            nxt[i][j] = false;
      }
   }
}


/* The program takes one optional arugment: -fg (full GUI) or -lg
 * (lazy GUI).  The default (no option) tells the program to skip the
 * GUI components (in other words run silently), while lg tells the
 * program to print to the screen the left-most 10 x 10 cells of the
 * board using a "text-only" (hence lazy) GUI. The silent mode is much
 * more stable than the fg mode and what will be used measure your
 * code's performance.
 */
int main(int argc, char* argv[]){

  // mode 1 : invalid number of arguments specified
  // mode 2 : -fg full GUI mode
  // mode 3 : -lg (lazy gui) console print GUI mode
  // mode 4 : silent (default)
  
  if (argc > 2 ){
    fprintf(stderr, "Usage: %s [-fg or -lg]\n\t-fg Full GUID\n\t-lg Lazy GUI Mode\n", argv[0]);
    return EXIT_FAILURE; 
  } else if ((argc == 2 && 0 == strcmp (argv[1], "-lg"))) {
    printf("Running in -lg (Lazy GUI) mode\n");
    printLazy += 1;
    add_method("Optimized", &myEvolve);
    add_method("Simple", &evolve);
    run_game(1);
  } else if ((argc == 2 && 0 == strcmp (argv[1], "-fg"))) {
    printf("Running in GUI mode\n");
    add_method("Optimized", &myEvolve);
    add_method("Simple", &evolve);
    run_game(0);
  } else if ((argc == 2 && (1 != strcmp (argv[1], "-fg") || 1 != strcmp (argv[1], "-lg")))) {
    fprintf(stderr, "Usage: %s [-fg or -lg]\n\t-fg Full GUID\n\t-lg Lazy GUI Mode\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    printf("Running in silent (no GUI window) mode\n");
    add_method("Optimized", &myEvolve);
    add_method("Simple", &evolve);
    run_game(1);
  }              
  return 0;
}





