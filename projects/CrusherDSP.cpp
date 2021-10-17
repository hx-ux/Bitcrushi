#include "CrusherDSP.h"
#include <cmath>

double CrusherDSP::processCrush(double inputValue, int bypass, int _step)
{
   int rate = 2;
  // float clamped = fmin(1, fmax(-1, inputValue));



  float r = floor(inputValue * (pow(rate, 2) - 1) + .5) / (pow(rate, 2) - 1);
  return r;
  //return r * 2 - 1;
 
}
