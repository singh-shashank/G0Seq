#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G0DumbStepSimulator.hh"
#include "G0VProcess.hh"
#include "G0ProcessA.hh"
#include "G0ProcessB.hh"
#include "G0ProcessC.hh"
#include "G0Track.hh"
#include "G0TrackManager.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/time.h>
#ifdef __USEITT__
#include <ittnotify.h>
#endif

using namespace std;

const int repetitions = 1000;
//const int repetitions = 1;

//------------------------------------------------------
// general timer function using unix system call
// dlowell ANL-MCS
//------------------------------------------------------
double getclock() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (tp.tv_sec + tp.tv_usec*1.0e-6);
}

//============================================================
// main function
//
// Usage:
//   ./rkproto <filename>
// -OR-
//   ./rkproto <filename> v4
//============================================================
int main( int argc, char *argv[] )
{
  char *trackFname = NULL;
  char *testFname = NULL;
  char *dumbstepFname = NULL;
  int c;
  bool fDumpTestFile = false;

  opterr = 0;
  while ((c = getopt (argc, argv, "ht:f:o")) != -1)
    switch (c)
      {
      case 'h':
	cout << "Usage: ./rktot -t <trackFname> -f <dumbstepFname> [-o]" << endl;
	return 1;
      case 't':
	trackFname = optarg;
        break;
      case 'o':
	fDumpTestFile = true;
        break;
      case 'f':
        dumbstepFname = optarg;
        break;
      case '?':
        if (optopt == 'f')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

  //==============================================
  // Set up the singleton class G0DumbStepSimulator
  // which is used to feed 15-tuples of input data
  // during track processing
  //==============================================
  //============================================================
  // Construct the objects of interest
  //============================================================
#ifdef __VERBAL__
  cout << "Done constructing objects for DumbStepper" << endl;
#endif
  string fName;
  istringstream ss(dumbstepFname);
  if (!(ss >> fName)) {
    cerr << "Invalid file name " << dumbstepFname << endl;
    exit(1);
  }
  G0DumbStepSimulator::setFile(fName);

  //========================================
  // Build up the tracks from the track file
  //========================================
  ifstream trackFile(trackFname);

  if ( !trackFile ) {
    cerr << "No such file " << trackFname << endl;
    exit(1);
  }

  // Got a valid track file
  char ch;
  int rkIndex, trackIndex = 0;
  G0Track *track = 0;
  list<G0Track*> trackList;
  double processStart, processEnd, processElapsed;

  track = new G0Track(trackIndex++);
  while ((ch = trackFile.get()) != EOF) {

    if ( ch == 'A' ) {
      G0ProcessA *procA = new G0ProcessA();
      track->addProcess(procA);
    }
    else if ( ch == 'B' ) {
      trackFile >> rkIndex;
      G0ProcessB *procB = new G0ProcessB(rkIndex);
      track->addProcess(procB);
    }
    else if ( ch == 'C' ) {
      G0ProcessC *procC = new G0ProcessC();
      track->addProcess(procC);
    }
    else if ( ch == '\n' ) {
      trackList.push_back(track);
      track = new G0Track(trackIndex++);
    }
  }
  delete track;

  G0TrackManager trackManager( trackList );
#ifdef __USEITT__
  __itt_resume();
#endif
  processStart = getclock();
  trackManager.processTracks();
  processEnd = getclock();
#ifdef __USEITT__
  __itt_pause();
#endif
  processElapsed = processEnd - processStart;
  printf("Non-vectorized elapsed time %lf sec\n", processElapsed);

  int numTracks;
  const double *trackEndPoints;
  trackManager.getEndPoints( numTracks, trackEndPoints );
#ifdef __VERBAL__
  cout << "Processed " << numTracks << " tracks" << endl;
  for ( int j = 0; j < numTracks; j++ )
    cout << "ept[" << j << "] = (" << trackEndPoints[3*j] << ", "
	 << trackEndPoints[3*j+1] << ", "
	 << trackEndPoints[3*j+2] << ")" << endl;
#endif

  if ( fDumpTestFile ) {
    ofstream outputFile("output.dat");
    outputFile << numTracks << endl;
    for ( int j = 0; j < numTracks; j++ )
      outputFile << trackEndPoints[3*j] << " "
		 << trackEndPoints[3*j+1] << " "
		 << trackEndPoints[3*j+2] << endl;
  }

  while ( !trackList.empty() ) {
    track = trackList.front();
    trackList.pop_front();
    delete track;
  }


}
