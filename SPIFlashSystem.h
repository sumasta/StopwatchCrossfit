// SPIFlashSystem.h

#ifndef _SPIFLASHSYSTEM_h
#define _SPIFLASHSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void beginSPIFFS(void);
void fileTest1(void);
void fileTest2(void);


#endif

