#include "G0DumbStepSimulator.hh"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

extern "C" void ddg_start_trace(void) throw();
extern "C" void ddg_stop_trace(void) throw();
extern "C" void ddg_region_begin(void) throw();
extern "C" void ddg_region_end(void) throw();


// Global static pointer used to ensure a single instance of the class.
G0DumbStepSimulator* G0DumbStepSimulator::fInstance = 0;
double *G0DumbStepSimulator::fInputDataRota = 0;
int G0DumbStepSimulator::fNumInputs = 0;
double G0DumbStepSimulator::yOut[8];

// set 
void G0DumbStepSimulator::setFile(string fname)
{

  if ( fInputDataRota != 0 )
    free( fInputDataRota );

  ifstream inputFile(fname.c_str());  
  if ( !inputFile ) {
    cerr << "No such file " << fname << endl;
    exit(1);
  }
  
  inputFile >> fNumInputs;
  fInputDataRota = (double*)malloc( 17*fNumInputs*sizeof(double) );
  double *dataNext = fInputDataRota;
  for ( int j = 0; j < fNumInputs; j++ ) {
    inputFile >> dataNext[0] >> dataNext[1] >> dataNext[2]
	      >> dataNext[3] >> dataNext[4] >> dataNext[5];
    inputFile >> dataNext[8] >> dataNext[9] >> dataNext[10]
	      >> dataNext[11] >> dataNext[12] >> dataNext[13];
    inputFile >> dataNext[16];
    dataNext += 17;
  }
  inputFile.close();
}

// Constructor is private so that it can  not be called
G0DumbStepSimulator::G0DumbStepSimulator()
{
};
  
/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

G0DumbStepSimulator* G0DumbStepSimulator::instance()
{
  if (!fInstance)   // Only allow one instance of class to be generated.
    fInstance = new G0DumbStepSimulator;

  return fInstance;
}

const int nvar = 6;

const double fCof = -299.792458;

double c[24] = {
  .010, .010, .010,                                       // 3(0)
  .0001, .0001, .0001, .0001, .0001,                      // 5(1)
  .00001, .00001, .00001, .00001, .00001, .00001, .00001, // 7(2)
  .000001, .000001, .000001, .000001, .000001, .000001,
  .0000001, .0000001, .0000001                            // 9(3)
} ;                                            // total :   24

double yt[8];
double dydxt[8];
double dydxm[8];

void GetFieldValue( const double *yTrack,
		    double B[3]     )
{
   int i ;
   double a = 1.00 ;   // mm -> m 
   double x = a*yTrack[0], y = a*yTrack[1], z = a*yTrack[2] ;
   double x2 = x*x, y2 = y*y, z2 = z*z ;
   double x3 = x2*x, y3 = y2*y, z3 = z2*z ;
   double xy = x*y, xz = x*z, yz = y*z, xyz = x*y*z ;

   B[0] =  c[1]
          -2*c[3]*x + c[4]*z +c[6]*y -2*c[7]*x
          -6*c[8]*xz + c[9]*(z2-x2) -2*c[10]*xy + c[11]*yz - 2*c[12]*xz
          +c[13]*(y2-x2) - 6*c[14]*xy
          -4*c[15]*(3*x*z2-x3) +c[16]*(z3-3*x2*z) - 6*c[17]*xyz +c[18]*y*(z2-x2)
          -2*c[19]*(x*z2+x*y2-2*x3/3) + c[20]*z*(y2-x2) - 6*c[21]*xyz
          +c[22]*(y3-3*x2*y) - 4*c[23]*(3*x*y2-x3) ;
   
   B[1] =  c[2]
          +c[5]*z + c[6]*x + 2*c[7]*y
          +c[10]*(z2-x2) + c[11]*xz +2*c[12]*yz +2*c[13]*xy + 3*c[14]*(y2-x2)
	  +c[17]*(z3-3*x2*z) + c[18]*(x*z2-x3/3) +2*c[19]*y*(z2-x2)
          +2*c[20]*xyz
          +3*c[21]*z*(y2-x2) + c[22]*(3*x*y2-x3) + 4*c[23]*(y3-3*x2*y) ;
   
   B[2] =  c[0]
          +c[3]*z + c[4]*x + c[5]*y
          +3*c[8]*(z2-x2) + 2*c[9]*xz + 2*c[10]*yz + c[11]*xy + c[12]*(y2-x2)
          +4*c[15]*(z3-3*x2*z) + c[16]*(3*x*z2-x3) + 3*c[17]*(y*z2-x2*y)
          +2*c[18]*xyz
          +2*c[19]*z*(y2-x2) + c[20]*(x*y2-x3/3) + c[21]*(y3-3*x2*y) ;
   for(i=0;i<3;i++)
   {
      B[i] = 0.1*B[i] ;
   }
   return ;
}

//inline
void RightHandSide( const  double y[],
		    double dydx[]   )
{
  double B[3];   
  double PositionAndTime[4];

  // Position
  PositionAndTime[0] = y[0];
  PositionAndTime[1] = y[1];
  PositionAndTime[2] = y[2];
  // Global Time
  PositionAndTime[3] = y[7];  // See G4MagneticFieldTrack::LoadFromArray

  /*
  cout << "PAT: " << PositionAndTime[0] << " " 
       << PositionAndTime[1] << " " << PositionAndTime[2] << endl;
  */
  GetFieldValue(PositionAndTime, B) ;

  /*
  cout << "B:   " << B[0] << " " << B[1] << " " << B[2] << endl;
  */

  double momentum_mag_square = y[3]*y[3] + y[4]*y[4] + y[5]*y[5];
  double inv_momentum_magnitude = 1.0 / std::sqrt( momentum_mag_square );
  
  double cof = fCof*inv_momentum_magnitude;

  dydx[0] = y[3]*inv_momentum_magnitude;       //  (d/ds)x = Vx/V
  dydx[1] = y[4]*inv_momentum_magnitude;       //  (d/ds)y = Vy/V
  dydx[2] = y[5]*inv_momentum_magnitude;       //  (d/ds)z = Vz/V

  dydx[3] = cof*(y[4]*B[2] - y[5]*B[1]) ;   // Ax = a*(Vy*Bz - Vz*By)
  dydx[4] = cof*(y[5]*B[0] - y[3]*B[2]) ;   // Ay = a*(Vz*Bx - Vx*Bz)
  dydx[5] = cof*(y[3]*B[1] - y[4]*B[0]) ;   // Az = a*(Vx*By - Vy*Bx)

  return ;
}



void dumbStepper( const double  yIn[],
		  const double  dydx[],
		  double  h,
		  double  yOut[])
{

  //ddg_region_begin();
  int i;
  double  hh = h*0.5 , h6 = h/6.0  ;

  // Initialise time to t0, needed when it is not updated by the integration.
  //        [ Note: Only for time dependent fields (usually electric) 
  //                  is it neccessary to integrate the time.] 
  yt[7]   = yIn[7]; 
  yOut[7] = yIn[7];

  for(i=0;i<nvar;i++)
  {
    yt[i] = yIn[i] + hh*dydx[i] ;             // 1st Step K1=h*dydx
  }

  RightHandSide(yt,dydxt) ;                   // 2nd Step K2=h*dydxt

  for(i=0;i<nvar;i++)
  { 
    yt[i] = yIn[i] + hh*dydxt[i] ;
  }
  RightHandSide(yt,dydxm) ;                   // 3rd Step K3=h*dydxm

  for(i=0;i<nvar;i++)
  {
    yt[i]   = yIn[i] + h*dydxm[i] ;
    dydxm[i] += dydxt[i] ;                    // now dydxm=(K2+K3)/h
  }
  RightHandSide(yt,dydxt) ;                   // 4th Step K4=h*dydxt

  /*
  cout << "yt: " << yt[0] << " " << yt[1] << " " << yt[2]
       << " " << yt[3] << " " << yt[4] << " " << yt[5] << endl;
  cout << "dydxt: " << dydxt[0] << " " << dydxt[1] << " " << dydxt[2]
       << " " << dydxt[3] << " " << dydxt[4] << " " << dydxt[5] << endl;
  */

  // Final RK4 output
  //+K1/6+K4/6+(K2+K3)/3
  for(i=0;i<nvar;i++)
  {
    yOut[i] = yIn[i]+h6*(dydx[i]+dydxt[i]+2.0*dydxm[i]); //+K1/6+K4/6+(K2+K3)/3
  }

  /*
  cout << "yOut: " << yOut[0] << " " << yOut[1] << " " << yOut[2]
       << " " << yOut[3] << " " << yOut[4] << " " << yOut[5] << endl;
  */
  //NormalisePolarizationVector ( yOut );
  //ddg_region_end();
}

void G0DumbStepSimulator::takeStep( int rkDataIndex, double yResult[3] )
{
  if ( fInputDataRota == 0 ) {
    cerr << "G0DumbStepSimulator File Not Set" << endl;
    exit(1);
  }
  if ( rkDataIndex < 0 ) {
    cerr << "negative data index not allowed" << endl;
    exit(1);
  }

  // in case the data index is too big
  rkDataIndex = rkDataIndex % fNumInputs;

  //cout << "Taking one step with input group " << rkDataIndex
  //     << " out of " << fNumInputs << endl;

  double *y = fInputDataRota + 17*rkDataIndex;
  double *dydx = fInputDataRota + 17*rkDataIndex + 8;
  double x = fInputDataRota[17*rkDataIndex + 16];
  dumbStepper( y, dydx, x, yOut );
  yResult[0] = yOut[0];
  yResult[1] = yOut[1];
  yResult[2] = yOut[2];
}

