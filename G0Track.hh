#ifndef G0TRACK_H
#define G0TRACK_H

#include "G0VProcess.hh"
#include <list>

class G0Track
{
 public:
  G0Track();
  G0Track(int id);
  ~G0Track();
  void clear();
  void addProcess( G0VProcess *proc );
  void process();
  void getEndPoint(double ept[3]);
  int  getId() { return fId; }

 private:
  std::list<G0VProcess*> fProcessList;
  int    fId;
  double fEndPoint[3];

};

#endif
