#include <math.h>

float about_cos (float x) {
  const
  float ret = cos(x),
        iki = 3e-1;
  return (fabs(ret) < iki) ? 0.0 : ret;
}

