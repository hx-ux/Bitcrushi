
#ifndef _CrusherDSP // if this class hasn't been defined, the program can define it
#define _CrusherDSP// by using this if statement you prevent the class to be called more
//#include "ICruscher.h"
#pragma once


class CrusherDSP
{
public:

  double processCrush(double inputValue, int bypass, int step);

private:
};

#endif