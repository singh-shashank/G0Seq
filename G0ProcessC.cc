#include "G0ProcessC.hh"
#include <iostream>

using namespace std;

G0ProcessC::~G0ProcessC()
{
}

void G0ProcessC::doIt(double yOut[3])
{
  //cout << "do C" << endl;
  yOut[0] = 0.0;
  yOut[1] = 0.0;
  yOut[2] = 0.0;
  sleep(0.03);
}
