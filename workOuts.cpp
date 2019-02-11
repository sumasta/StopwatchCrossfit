// 
// 
// 

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


