#include "G0ProcessA.hh"
#include <iostream>

using namespace std;

G0ProcessA::~G0ProcessA()
{
}

void G0ProcessA::doIt(double yOut[3])
{
  //cout << "do A" << endl;
  yOut[0] = 0.0;
  yOut[1] = 0.0;
  yOut[2] = 0.0;
  sleep(0.02);
}
