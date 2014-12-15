#include "G0TrackManager.hh"
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;

extern "C" void ddg_start_trace(void) throw();
extern "C" void ddg_stop_trace(void) throw();

extern "C" void ddg_region_begin(void) throw();
extern "C" void ddg_region_end(void) throw();

G0TrackManager::G0TrackManager( std::list<G0Track*> &trackList ) :
  fTrackList( trackList ), fTrackEndPoints(0)
{
  fNumTracks = fTrackList.size();
  fTrackEndPoints = (double*)malloc(3*fNumTracks*sizeof(double));
}

G0TrackManager::~G0TrackManager( )
{
  if ( fTrackEndPoints )
    free( fTrackEndPoints );
}

void G0TrackManager::getEndPoints( int &numTracks, const double *&endPointArray )
{
  numTracks = fNumTracks;
  endPointArray = fTrackEndPoints;
}

void G0TrackManager::processTracks( )
{
  G0Track *track;
  double   trackEndPoint[3];
  int      trackId;
  int curTrack = 0;
  int numTracks = fTrackList.size();
  ddg_start_trace();
  while ( !fTrackList.empty() ) {
    //ddg_region_begin();
    track = fTrackList.front();
    fTrackList.pop_front();
#ifdef __VERBAL__
    cout << endl << endl << "PROCESSING TRACK " << curTrack++
	 << " OF " << numTracks << endl << endl;
#endif
    track->process();
    track->getEndPoint(trackEndPoint);
    trackId = track->getId();
    fTrackEndPoints[3*trackId]   = trackEndPoint[0];
    fTrackEndPoints[3*trackId+1] = trackEndPoint[1];
    fTrackEndPoints[3*trackId+2] = trackEndPoint[2];
    //ddg_region_end();
  }
  ddg_stop_trace();
}

