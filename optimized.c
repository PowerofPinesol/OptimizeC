#include "liblife.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int gen = 0;
int print = 0;

// Unrolled loop which removes repeated jump and INC/ADD calls in assembly code
// Now sets each cell individually. Has the same semantics as evolve in Simple solution.
int neighbor (board b, int i, int j)
{
  int n = 0;
  int left, right, up, down;

  // precompute for quick reuse from registers/cache
  left = j - 1;
  right = j + 1;
  up = i - 1;
  down = i + 1; 

  // handle edge case up
  if(up >= 0) {
    n += b[up][j];
    if (left >= 0) n += b[up][left];
    if (right < WIDTH) n += b[up][right];
  }

  // handle edge case down
  if(down < HEIGHT) {
    n += b[down][j];
    if (left >= 0) n += b[down][left];
    if (right < WIDTH) n += b[down][right];
  }

  // left-right edge cases
  if (left >= 0) n += b[i][left];
  if (right < WIDTH) n += b[i][right];

  return n;
}

void myEvolve(board prv, board nxt){
   int i, j;
   int n;
   
   // Reduced unneccessary memory calls, removed loop inefficiencies here as well
   // Checks for most common cases first, assumes that nxt is instantiated with zeros/false
   // Reordered the loop in order to have higher CPU cache hits
   for (i=0; i < HEIGHT; i++) {
      for (j= 0; j < WIDTH; j++) {
        n = neighbor(prv, i, j);    // Tried removing PROC call, ended up with slightly slower speeds. Compilers, how do they work?
        int value = prv[i][j];      // Flatten/cache value

        // Only access memory for purpose of setting for singular condition
        if((n == 3 || (value && n == 2))) {
          nxt[i][j] = true;
        }
      }
   }
}