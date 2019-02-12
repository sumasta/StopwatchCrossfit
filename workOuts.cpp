
#include "workOuts.h"

class movements {
public:
	String name;
	int reps;
	boolean split;
};

class workOuts {
public:
	String name;
	int sequence;

	// 0=FT : 1=AMRAP
	boolean configTypes;

	// True - Reset the timer at the beginning of this section.
	// False - Do not reset the clock, keep it running from the previous section.
	boolean configResetClock;

	// (FT) The number of rounds (loops) of that section to complete
	int rounds;

	//True - if the athlete doesn’t complete the allocated reps before the timecap then the workout ends
	//False - if the athlete doesn’t  complete the allocated reps before the timecap, he can still continue with the next section of the WoD.
	boolean hardCap;

	// True - save the split time for that section/movement, save a split time for each round.
	//False - no need to save the split time for that section.
	boolean split;

	//True - save the split time for that section/movement, save a split time for each round.
	//False - no need to save the split time for that section.
	boolean fullTime;

	void chengeName(String nameNew) {
		this->name = nameNew;
	}
};

workOuts myWorkouts[10];
movements myMovements[10][10];

void setWorkoutsName(int index, String name) {
	myWorkouts[index].name = name;
}

String getWorkoutsName(int index) {
	return myWorkouts[index].name;
}

void setSequence(int index, int seq) {
	myWorkouts[index].sequence = seq;
}

int getSequence(int index) {
	return myWorkouts[index].sequence;
}

void setConfigType(int index, boolean type) {
	myWorkouts[index].configTypes = type;
}

boolean getConfigType(int index) {
	return myWorkouts[index].configTypes;
}

void setRound(int index, int rounds) {
	myWorkouts[index].rounds = rounds;
}

int getRounds(int index) {
	return myWorkouts[index].rounds;
}

void setHardCap(int index, boolean type) {
	myWorkouts[index].hardCap = type;
}

boolean getHardcap(int index) {
	return myWorkouts[index].hardCap;
}

void setSplit(int index, boolean type) {
	myWorkouts[index].split = type;
}

boolean getSplit(int index) {
	return myWorkouts[index].split;
}

void setFulltime(int index, boolean type) {
	myWorkouts[index].fullTime = type;
}

boolean getFulltime(int index) {
	return myWorkouts[index].fullTime;
}

void setMovementName(int wodIndex, int moveIndex, String name) {
	myMovements[wodIndex][moveIndex].name = name;
}

String getMovementName(int wodIndex, int moveIndex) {
	return myMovements[wodIndex][moveIndex].name;
}

void setMovementReps(int wodIndex, int moveIndex, int reps) {
	myMovements[wodIndex][moveIndex].reps = reps;
}

int getMovementReps(int wodIndex, int moveIndex) {
	return myMovements[wodIndex][moveIndex].reps;
}

void setMovementSplit(int wodIndex, int moveIndex, boolean split) {
	myMovements[wodIndex][moveIndex].split = split;
}

boolean getMovementSplit(int wodIndex, int moveIndex) {
	return myMovements[wodIndex][moveIndex].split;
}


void assignData(int selector) {
	myWorkouts[0].name = "Murph";
	myWorkouts[0].sequence = true;
	myWorkouts[0].configTypes = false;
	myWorkouts[0].configResetClock = false;
	myWorkouts[0].rounds = 99;
	myWorkouts[0].hardCap = true;
	myWorkouts[0].split = true;
	myWorkouts[0].fullTime = false;

	myMovements[0][0].name = "Pull-ups";
	myMovements[0][0].reps = 20;
	myMovements[0][0].split = true;
}


