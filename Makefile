OBJS = G0VProcess.bc G0ProcessA.bc G0ProcessB.bc G0ProcessC.bc G0Track.bc G0TrackManager.bc \
       G0DumbStepSimulator.bc g0seq.bc
G4INSTALL=/home/shashank/project/geant4_ddg/geant4.10.00.p02
INCLUDE=-I$(G4INSTALL)/source/externals/clhep/include
CC = clang++
DEBUG = -g
##G0SEQ_CFLAGS = $(CFLAGS) -march=native -D_RK4VECT -O3 -Wall -c $(DEBUG) $(INCLUDE)
G0SEQ_CFLAGS = $(CFLAGS) -Wall -c -flto $(INCLUDE) -O0
LFLAGS = -Wall -lpthread $(DEBUG) -O0
#LIBS= -L/opt/software/intel/composerxe2013/vtune_amplifier_xe_2013/lib64 -littnotify
_RK4VECT=1
EXEBASE=g0seq
DDG_DIR=/home/shashank/ddg_dbg_orion

g0seq.instr : g0seq.instr.bc
	clang++ -L/opt/software/gcc/4.6.1/lib64/ -L$(DDG_DIR)/lib -lddg-rt $< -o $@

$(EXEBASE).instr.bc: $(EXEBASE).opt.bc
	opt -load $(DDG_DIR)/lib/ddg-instr.so -loop-simplify -split-lpads -instrument-ddg -instrument-indvars -o $@ $<

$(EXEBASE).opt.bc: $(EXEBASE).combined.bc
	cp $< $@
	#opt -O3 $< -o $@

g0seq.combined.bc : $(OBJS)
	llvm-link $(OBJS) -o $@	

g0seq.bc : g0seq.cc
	$(CC) $(G0SEQ_CFLAGS) g0seq.cc -o $@

G0ProcessA.bc : G0ProcessA.hh G0VProcess.hh G0ProcessA.cc
	$(CC) $(G0SEQ_CFLAGS) G0ProcessA.cc -o $@

G0ProcessB.bc : G0ProcessB.hh G0VProcess.hh G0ProcessB.cc
	$(CC) $(G0SEQ_CFLAGS) G0ProcessB.cc -o $@

G0ProcessC.bc : G0ProcessC.hh G0VProcess.hh G0ProcessC.cc
	$(CC) $(G0SEQ_CFLAGS) G0ProcessC.cc -o $@

G0VProcess.bc : G0VProcess.hh G0VProcess.cc
	$(CC) $(G0SEQ_CFLAGS) G0VProcess.cc -o $@

G0Track.bc : G0Track.hh G0VProcess.hh G0Track.cc 
	$(CC) $(G0SEQ_CFLAGS) G0Track.cc -o $@

G0TrackManager.bc : G0TrackManager.hh G0Track.hh G0TrackManager.cc 
	$(CC) $(G0SEQ_CFLAGS) G0TrackManager.cc -o $@

G0DumbStepSimulator.bc : G0DumbStepSimulator.cc G0DumbStepSimulator.hh
	$(CC) $(G0SEQ_CFLAGS) G0DumbStepSimulator.cc -o $@


clean :
	\rm *.bc *.instr

llvmclean :
	rm -f *.db *.trace
