//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: globals.hh 67970 2013-03-13 10:10:06Z gcosmo $
//
//
// Global Constants and typedefs
//
// History:
// 30.06.95 P.Kent - Created
// 16.02.96 G.Cosmo - Added inclusion of "templates.hh"
// 03.03.96 M.Maire - Added inclusion of "G4PhysicalConstants.hh"
// 08.11.96 G.Cosmo - Added cbrt() definition and G4ApplicationState enum type
// 29.11.96 G.Cosmo - Added typedef of HepBoolean to G4bool
// 22.10.97 M.Maire - Moved PhysicalConstants at the end of the file
// 04.12.97 G.Cosmo,E.Tcherniaev - Migrated to CLHEP
// 26.08.98 J.Allison,E.Tcherniaev - Introduced min/max/sqr/abs functions
// 22.09.98 G.Cosmo - Removed min/max/sqr/abs functions and replaced with
//                    inclusion of CLHEP/config/TemplateFunctions.h for CLHEP-1.3
// 15.12.99 G.Garcia - Included min, max definitions for NT with ISO standard
// 15.06.01 G.Cosmo - Removed cbrt() definition

#ifndef GLOBALS_HH
#define GLOBALS_HH

#ifndef FALSE
  #define FALSE 0
#endif
#ifndef TRUE
  #define TRUE 1
#endif

#include <algorithm>  // Retrieve definitions of min/max
#include <limits>

// Include base types
#include "G4Types.hh"

#define DBL_MAX   std::numeric_limits<double>::max()  // 1.7976931348623157e+308

#ifndef G4_SQR_DEFINED
  #define G4_SQR_DEFINED
  #ifdef sqr
    #undef sqr
  #endif

template <class T>
inline T sqr(const T& x)
{
  return x*x;
}
#endif

// Get definition of G4String
//#include "G4String.hh"

// Includes some additional definitions: sqr, G4SwapPtr, G4SwapObj.
//#include "templates.hh"

// Includes Physical Constants and System of Units
// #include "G4PhysicalConstants.hh"
// #include "G4SystemOfUnits.hh"
#endif /* GLOBALS_HH */

