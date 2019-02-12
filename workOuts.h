// workOuts.h

#ifndef _WORKOUTS_h
#define _WORKOUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void assignData(int selector);
void setWorkoutsName(int index, String name);
String getWorkoutsName(int index);
void setSequence(int index, int seq);
int getSequence(int index);
void setConfigType(int index, boolean type);
boolean getConfigType(int index);
void setRound(int index, int rounds);
int getRounds(int index);
void setHardCap(int index, boolean type);
boolean getHardcap(int index);
void setSplit(int index, boolean type);
boolean getSplit(int index);
void setFulltime(int index, boolean type);
boolean getFulltime(int index);
void setMovementName(int wodIndex, int moveIndex, String name);
String getMovementName(int wodIndex, int moveIndex);
void setMovementReps(int wodIndex, int moveIndex, int reps);
int getMovementReps(int wodIndex, int moveIndex);
void setMovementSplit(int wodIndex, int moveIndex, boolean split);
boolean getMovementSplit(int wodIndex, int moveIndex);

#endif

