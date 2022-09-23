#if !defined(_9c098646_d92a_11e3_9d6a_f0def131db66)
#define _9c098646_d92a_11e3_9d6a_f0def131db66

#include <stdbool.h>

#define HEIGHT 1024
#define WIDTH 1024
typedef bool board[HEIGHT][WIDTH];

typedef void (*evolve_t)(board p, board n);

void run_game(bool no_window);
void add_method(char* name, evolve_t evolve);

#endif /*_9c098646_d92a_11e3_9d6a_f0def131db66*/
