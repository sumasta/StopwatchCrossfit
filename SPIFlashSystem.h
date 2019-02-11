// SPIFlashSystem.h

#ifndef _SPIFLASHSYSTEM_h
#define _SPIFLASHSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
    #include "FS.h"
#else
	#include "WProgram.h"
#endif

void beginSPIFFS(void);
void fileTest1(void);
void fileTest2(void);
void scanXML(fs::FS &fs, const char * path);


#endif

