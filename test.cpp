#include <GL/glut.h>
#include "domino.h"
#include "gl.h"

void keyboard (unsigned char key, int x, int y) {
  float step_x = 1.0;
  static int theta_i = 60;
  switch (key) {
    case 'q': exit(0);
    case ' ': add_domino(); break;
    case 'h': cursor_move(-step_x); break;
    case 'l': cursor_move(+step_x); break;
    case 'H': cursor_move(-step_x*5); break;
    case 'L': cursor_move(+step_x*5); break;
    case 'j': theta_i += 10; cursor_set_theta(theta_i); break;
    case 'J': theta_i += 2; cursor_set_theta(theta_i); break;
    case 'k': theta_i -= 10; cursor_set_theta(theta_i); break;
    case 'K': theta_i -= 2; cursor_set_theta(theta_i); break;
    case '@': animation_toggle(); break;
    case 'd': debug(); break;
  }
  if (theta_i < 0) theta_i = 0;
  if (theta_i > 120) theta_i = 120;
  glutPostRedisplay();
}

int main (int argc, char*argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow("domino");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  init_cursor();
  glutMainLoop();
  return 0;
}
