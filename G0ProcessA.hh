#ifndef G0PROCESSA_H
#define G0PROCESSA_H

#include "G0VProcess.hh"

class G0ProcessA : public G0VProcess
{
 public:
  virtual ~G0ProcessA();
  virtual void doIt(double yOut[3]);
};

#endif
