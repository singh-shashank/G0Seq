#include "G0ProcessB.hh"
#include  "G0DumbStepSimulator.hh"
#include <iostream>
#include <cstdio>

using namespace std;

G0ProcessB::G0ProcessB() : fRKDataIndex(0)
{
}

G0ProcessB::G0ProcessB(int rkIndex) : fRKDataIndex(rkIndex)
{
}

G0ProcessB::~G0ProcessB()
{
}

void G0ProcessB::doIt(double yOut[3])
{
  //cout << "do B" << endl;
  doItLevelOne(yOut);
}

void G0ProcessB::doItLevelOne(double yOut[3])
{
  doItLevelTwo(yOut);
}

void G0ProcessB::doItLevelTwo(double yOut[3])
{
  doItLevelThree(yOut);
}

void G0ProcessB::doItLevelThree(double yOut[3])
{
  G0DumbStepSimulator *gsSimulator = G0DumbStepSimulator::instance();
  gsSimulator->takeStep(fRKDataIndex, yOut);
}

