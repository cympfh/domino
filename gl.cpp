#include <GL/glut.h>
#include <string.h>
#include "domino.h"

void drawString (char *s, float x, float y, float z) {
  void* font = GLUT_BITMAP_HELVETICA_18;
  glRasterPos3f(x, y, z);
  for (uint i=0; i<strlen(s); glutBitmapCharacter(font, s[i++]));
}

float relX (float x) {
    float half_width = WIDTH/2.0;
    return (x - half_width) / half_width;
}
float relY (float y) {
    float half_height = HEIGHT/2.0;
    return (y - half_height) / half_height;
}
