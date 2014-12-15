#include "G0Track.hh"
#include "G0VProcess.hh"
#include <iostream>

using namespace std;
 
extern "C" void ddg_start_trace(void) throw();
extern "C" void ddg_stop_trace(void) throw();
extern "C" void ddg_region_begin(void) throw();
extern "C" void ddg_region_end(void) throw();

G0Track::G0Track() : fId(0)
{
  fEndPoint[0] = 0.0;
  fEndPoint[1] = 0.0;
  fEndPoint[2] = 0.0;
}

G0Track::G0Track(int id) : fId(id)
{
  fEndPoint[0] = 0.0;
  fEndPoint[1] = 0.0;
  fEndPoint[2] = 0.0;
}

G0Track::~G0Track()
{
  clear();
}

void G0Track::clear()
{
  while (! fProcessList.empty() ) {
    G0VProcess *proc = fProcessList.front();
    fProcessList.pop_front();
    delete proc;
  }
}

void G0Track::getEndPoint(double endPoint[3])
{
  endPoint[0] = fEndPoint[0];
  endPoint[1] = fEndPoint[1];
  endPoint[2] = fEndPoint[2];
}

void G0Track::addProcess( G0VProcess *proc )
{
  fProcessList.push_back(proc);
}

void G0Track::process( )
{
  G0VProcess *proc;
  double yOut[3];
  //ddg_start_trace();
  int cnt=0;
  for ( list<G0VProcess*>::iterator pit = fProcessList.begin();
	pit != fProcessList.end(); pit++, cnt++) {
    ddg_region_begin();
    (*pit)->doIt(yOut);
#ifdef __VERBAL__
  cout << "Update track " << fId << " with " << yOut[2] << endl;
#endif
    fEndPoint[0] += yOut[0];
    fEndPoint[1] += yOut[1];
    fEndPoint[2] += yOut[2];
    ddg_region_end();
  }
  cout << "cnt: " << cnt << "\n";
  //ddg_stop_trace();
}
