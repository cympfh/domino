#include <vector>
#define obj world[idx]

typedef unsigned int uint;

typedef struct {
  float x, y, theta,
        x1, y1, x2, y2,
        vx,vy,omega;
} domino;

// window size
const
int WIDTH = 600,
    HEIGHT= 200,
    GROUND = 20,

    domino_height = 50,
    domino_height_2 = domino_height / 2,
    domino_m = 10,
    domino_I = domino_m * domino_height * domino_height / 12;

const
float g = 0.98,
   	  dt = 1e-4;

void update_endpoints(int);
void move_world(void);
void add_domino(void);

void init_cursor(void);
void cursor_move(float);
void cursor_set_theta(int);

void display(void);
void animation_toggle(void);
void debug(void);
