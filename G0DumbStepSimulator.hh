#ifndef G0DUMBSTEPSIMULATOR_H
#define G0DUMBSTEPSIMULATOR_H

#include <string>

using namespace std;

class G0DumbStepSimulator{
 public:
  static G0DumbStepSimulator* instance();
  static void setFile(string fname);

  void takeStep(int rkDataIndex, double yResult[3]);
  
 private:
  G0DumbStepSimulator();  // Private so that it can  not be called
  G0DumbStepSimulator(G0DumbStepSimulator const&);             // copy constructor is private
  G0DumbStepSimulator& operator=(G0DumbStepSimulator const&);  // assignment operator is private
  static G0DumbStepSimulator* fInstance;
  static double *fInputDataRota;
  static int    fNumInputs;
  static double yOut[8];
};

#endif
