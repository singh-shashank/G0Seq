#ifndef G0PROCESSC_H
#define G0PROCESSC_H

#include "G0VProcess.hh"

class G0ProcessC : public G0VProcess
{
 public:
  virtual ~G0ProcessC();
  virtual void doIt(double yOut[3]);
};

#endif
