#include "CrusherDSP.h"
#include <cmath>

double CrusherDSP::processCrush(double inputValue, int bypass, int _step)
{


  int rate = 2;
  /*  float clamped = fmin(1, fmax(-1, inputValue));
    float normal = (inputValue + 1) / 2;*/

  float round = floor(inputValue * (pow(rate, _step) - 1) + .5) / (pow(rate, _step) - 1);
  return round * 2 - 1;
  /*   buffer++;*/
   //}
  // VERSION 2


  return 0.0;
}
