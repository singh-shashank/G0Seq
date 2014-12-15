#ifndef G0PROCESSB_H
#define G0PROCESSB_H

#include "G0VProcess.hh"

class G0ProcessB : public G0VProcess
{
 public:
  G0ProcessB();
  G0ProcessB(int rkIndex);
  virtual ~G0ProcessB();
  virtual void doIt(double yOut[3]);
 private:
  void doItLevelOne(double yOut[3]);
  void doItLevelTwo(double yOut[3]);
  void doItLevelThree(double yOut[3]);

  int fRKDataIndex;
};

#endif
