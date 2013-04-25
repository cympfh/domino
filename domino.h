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

// [0,WIDTH] -> [-1,1]
inline float relX (float x) {
    float half_width = WIDTH/2.0;
    return (x - half_width) / half_width;
}
// [0,HEIGHT] -> [-1,1]
inline float relY (float y) {
    float half_height = HEIGHT/2.0;
    return (y - half_height) / half_height;
}

bool intersect(int, int);
int ccw(float,float,float,float,float,float);
void debug(void);
