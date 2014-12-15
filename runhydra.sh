#!/bin/bash
make clean
make -f Micfile
cp dumbstep1.dat /tmp
cp ../tot/trx5000 /tmp
micnativeloadex ./rkseq -a "-t /hydratmp/trx5000 -f /hydratmp/dumbstep1.dat"
rm /tmp/trx5000
rm /tmp/dumbstep1.dat
