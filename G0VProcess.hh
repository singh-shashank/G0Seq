#ifndef G0VPROCESS_H
#define G0VPROCESS_H

#include <string>

class G0VProcess
{
 public:
  virtual ~G0VProcess();
  inline void SetName(const std::string theName);
  virtual void doIt(double yOut[3]) = 0;

 private:
  std::string fName;
};

#endif
