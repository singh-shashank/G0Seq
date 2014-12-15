#ifndef G0TRACKMANAGER_H
#define G0TRACKMANAGER_H

#include <pthread.h>
#include "G0Track.hh"

class G0TrackManager
{
 public:
  G0TrackManager( std::list<G0Track*> &trackList );
  ~G0TrackManager();
  void processTracks();
  void getEndPoints( int &numTracks, const double *&endPointArray );
 private:
  std::list<G0Track*>     &fTrackList;
  int                      fNumTracks;
  double                  *fTrackEndPoints;
};

#endif
