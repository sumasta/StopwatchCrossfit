/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 * Copyright (c) 2018 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */



 // Include the correct display library
 // For a connection via I2C using Wire include
 #include "BLEHandler.h"
#include "SPIFlashSystem.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
 #include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
 // or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
 // For a connection via I2C using brzo_i2c (must be installed) include
 // #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
 // #include "SSD1306Brzo.h"
 // #include "SH1106Brzo.h"
 // For a connection via SPI include
 // #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
 // #include "SSD1306Spi.h"
 // #include "SH1106SPi.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images

#include "images.h"

// Use the corresponding display class:

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

#define		BUT_OK		23
#define		BUT_RIGHT	33
#define		BUT_LEFT	32

#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 5, 4);

OLEDDisplayUi ui     ( &display );

int menuSetPointer = 0;

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, getBatteryLevel());
}

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
	display->setTextAlignment(TEXT_ALIGN_LEFT);
	display->setFont(ArialMT_Plain_10);
	display->drawString(0, 0, getClock());
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void frameManualWOD(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  
  //display->setFont(ArialMT_Plain_10);
  //display->drawString(0 + x, 10 + y, "Arial 10");
  //display->setFont(ArialMT_Plain_16);
  //display->drawString(0 + x, 20 + y, "Arial 16");

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 19 + y, "MANUAL WOD");

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64 + x, 36 + y, "Basic Set Counter");
}

void frameCompetitionWOD(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	display->drawString(64 + x, 19 + y, "COMPETITION");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "Custom WOD Preset");
}

void frameBenchmarkWOD(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	display->drawString(64 + x, 19 + y, "BENCHMARK");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "Standard WOD Counter");
}

void frameRFT(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	display->drawString(64 + x, 19 + y, "RFT MODE");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "Round for Time");
}

void frameBack(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_24);
	display->drawString(64 + x, 15 + y, "BACK");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "Return to Previous Menu");
}

void frameAMRAP(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	display->drawString(64 + x, 19 + y, "AMRAP MODE");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "As Much Rep Possible");
}

void transmitXML(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	display->drawString(64 + x, 19 + y, "BLE-SEND");

	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_10);
	display->drawString(64 + x, 36 + y, "Transmit saved XML Data");
}



// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[5];// = { drawFrame1, frameManualWOD, frameCompetitionWOD, frameBenchmarkWOD };

// how many frames are there?


// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = {clockOverlay , msOverlay};
int overlaysCount = 2;

String timeNow;

long counterCheckCycle;
long timeCheckCycle=30000;

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;

/*
void writeFile(fs::FS &fs, const char * path, const char * message);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readFile(fs::FS &fs, const char * path);
*/

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(BUT_OK, INPUT_PULLUP);
  pinMode(BUT_RIGHT, INPUT_PULLUP);
  pinMode(BUT_LEFT, INPUT_PULLUP);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  beginSPIFFS();
 setupBLE();

  initFrames();

  refreshTime();


}

void loop() {
  int remainingTimeBudget = ui.update();
 
  if (remainingTimeBudget > 0) {
	  cycleCheck();

	  routine();
    delay(remainingTimeBudget);
  }  
}

int frameCount = 5;

void initFrames() {
	// The ESP is capable of rendering 60fps in 80Mhz mode
	// but that won't give you much time for anything else
	// run it in 160Mhz mode or just set it to 30 fps
	ui.setTargetFPS(90);

	// Customize the active and inactive symbol
	ui.setActiveSymbol(activeSymbol);
	ui.setInactiveSymbol(inactiveSymbol);

	// You can change this to
	// TOP, LEFT, BOTTOM, RIGHT
	ui.setIndicatorPosition(BOTTOM);

	// Defines where the first frame is located in the bar.
	ui.setIndicatorDirection(LEFT_RIGHT);

	frameCount = 5;
	frames[0] = {drawFrame1};
	frames[1] = {frameManualWOD};
	frames[2] = {frameCompetitionWOD};
	frames[3] = {frameBenchmarkWOD};
	frames[4] = {transmitXML };


	// You can change the transition that is used
	// SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
	ui.setFrameAnimation(SLIDE_RIGHT);
	ui.setFrames(frames, frameCount);

	// Add overlays
	ui.setOverlays(overlays, overlaysCount);

	ui.disableAutoTransition();

	// Initialising the UI will init the display too.
	ui.init();

	display.flipScreenVertically();
}

void changeFramesManualWOD() {

#define HOMEMENU			0
#define MANUALWODMENU		1
#define COMPETITIONWODMENU	2
#define BENCHMARKWODMENU	3

	if (menuSetPointer == HOMEMENU && ui.getUiState()->currentFrame == 1) {

		//ui.setFrameAnimation(SLIDE_DOWN);
		//frames[3] = { frameRFT };
		//ui.switchToFrame(3);
		//ui.update();

		frames[0] = { frameRFT };
		frames[1] = { frameAMRAP };
		frames[2] = { frameBack };
		frameCount = 3;
		ui.setFrames(frames, frameCount);
		menuSetPointer = MANUALWODMENU;
		ui.setFrameAnimation(SLIDE_RIGHT);
	}
	else if (menuSetPointer == HOMEMENU && ui.getUiState()->currentFrame == 2) {
		notProgrammedBanner();
	}
	else if (menuSetPointer == HOMEMENU && ui.getUiState()->currentFrame == 3) {
		
		fileTest1();

	}
	else if (menuSetPointer == HOMEMENU && ui.getUiState()->currentFrame == 4) {
		fileTest2();

	}else if (menuSetPointer == MANUALWODMENU && ui.getUiState()->currentFrame == 0) { // manualWOD
		ui.disableIndicator();
		RFTSetTargetRound();
	}
	else if (menuSetPointer == MANUALWODMENU && ui.getUiState()->currentFrame == 1) {
		ui.disableIndicator();
		AMRAPSetTimeCap();
	}
	else if (menuSetPointer == MANUALWODMENU && ui.getUiState()->currentFrame == 2) {
		menuSetPointer = HOMEMENU;
		initFrames();
	}
	else if (menuSetPointer == HOMEMENU && ui.getUiState()->currentFrame == 0) {
		display.clear();
		display.display();
		deepSleep();
	}
	
}

void notProgrammedBanner() {
	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "SORRY!");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, "PROGRAM");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 40, "IS DISABLED");
	display.display();

	delay(2000);
	ESP.restart();
	
}

void RFTSetTargetRound() {
	int targetRound = 10;
	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "Add");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Sub");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Target Round");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, (String)targetRound);
	display.display();

	boolean blink = false;
	long timerBlink = millis();

	while (1) {
		cycleCheck();

		if (millis() - timerBlink > 500) {
			drawRoundToSet(targetRound, blink);
			blink = !blink;
			timerBlink = millis();
		}

		if (digitalRead(BUT_RIGHT) == LOW) {
			delay(200);
			while (digitalRead(BUT_RIGHT) == LOW) {};
			Serial.println("Incr");
			targetRound++; blink = false; timerBlink = millis();
			drawRoundToSet(targetRound, blink);
		}
		else if (digitalRead(BUT_LEFT) == LOW) {
			delay(200);
			while (digitalRead(BUT_RIGHT) == LOW) {};
			Serial.println("Decr");
			targetRound--; blink = false; timerBlink = millis();
			if (targetRound <= 0) {
				targetRound = 0;
			}
			drawRoundToSet(targetRound, blink);
		}
		else if (digitalRead(BUT_OK) == LOW) {
			delay(200);
			while (digitalRead(BUT_OK) == LOW) {};
			RFTSetTimeCap(targetRound);
		}

	}
}

void drawRoundToSet(int round, boolean blink) {
	
	String toRound;

	if (!blink) {
		toRound = (String)round;
	}
	else {
		toRound = "_";
	}
	
	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "Add");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Sub");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Target Round");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, toRound);
	display.display();
}

void RFTSetTimeCap(int targetRound) {

	int time[4];

	time[0] = 1;
	time[1] = 0;
	time[2] = 0;
	time[3] = 0;

	int pointerSet = 0;
#define MAXPOINTER	6

	String timeToSet = (String)time[0] + (String)time[1] +":"+ (String)time[2] + (String)time[3];

	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Min : Sec");

	
	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "GO");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Back");

	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, 49, "TimeCap");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, timeToSet);
	display.display();
	
	Serial.println("RFT Mode");

	boolean blink = false;
	long timerBlink = millis();

	while (1) {

			cycleCheck();

			if (millis() - timerBlink > 500) {
				drawTimeToSet(time, pointerSet, blink);
				blink = !blink;
				timerBlink = millis();
			}

			if (digitalRead(BUT_RIGHT) == LOW) {
				delay(200);
				while (digitalRead(BUT_RIGHT) == LOW) {};
				Serial.println("PointerUP");
				pointerSet++;
				if (pointerSet >= MAXPOINTER) {
					pointerSet = 0;
				}
			}
			else if (digitalRead(BUT_LEFT) == LOW) {
				delay(200);
				while (digitalRead(BUT_RIGHT) == LOW) {};
				Serial.println("PointerDOWN");
				pointerSet--;
				if (pointerSet < 0) {
					pointerSet = MAXPOINTER-1;
				}
			}
			else if (digitalRead(BUT_OK) == LOW) {
				delay(200);
				while (digitalRead(BUT_OK) == LOW) {};
				
				time[pointerSet]++;
				if (time[pointerSet] > 9)time[pointerSet] = 0;
				drawTimeToSet(time, pointerSet, false);
				
				if (pointerSet == 4) {
					startRFT(time, targetRound);
				}
				else if (pointerSet == 5) {
					RFTSetTargetRound();
				}

			}
		}
	
}

void AMRAPSetTimeCap() {

	int time[4];

	time[0] = 1;
	time[1] = 0;
	time[2] = 0;
	time[3] = 0;

	int pointerSet = 0;
#define MAXPOINTER	6

	String timeToSet = (String)time[0] + (String)time[1] + ":" + (String)time[2] + (String)time[3];

	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "As Much Rep");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Min : Sec");


	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "GO");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Back");

	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, 49, "TimeCap");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, timeToSet);
	display.display();

	Serial.println("AMRAP Mode");

	boolean blink = false;
	long timerBlink = millis();

	while (1) {

		cycleCheck();

		if (millis() - timerBlink > 500) {
			drawTimeToSetAMRAP(time, pointerSet, blink);
			blink = !blink;
			timerBlink = millis();
		}

		if (digitalRead(BUT_RIGHT) == LOW) {
			delay(200);
			while (digitalRead(BUT_RIGHT) == LOW) {};
			Serial.println("PointerUP");
			pointerSet++;
			if (pointerSet >= MAXPOINTER) {
				pointerSet = 0;
			}
		}
		else if (digitalRead(BUT_LEFT) == LOW) {
			delay(200);
			while (digitalRead(BUT_RIGHT) == LOW) {};
			Serial.println("PointerDOWN");
			pointerSet--;
			if (pointerSet < 0) {
				pointerSet = MAXPOINTER - 1;
			}
		}
		else if (digitalRead(BUT_OK) == LOW) {
			delay(200);
			while (digitalRead(BUT_OK) == LOW) {};

			time[pointerSet]++;
			if (time[pointerSet] > 9)time[pointerSet] = 0;
			drawTimeToSetAMRAP(time, pointerSet, false);

			if (pointerSet == 4) {
				startAMRAP(time);
			}
			else if (pointerSet == 5) {
				ESP.restart();
			}

		}
	}

}

void startAMRAP(int time[]) {

	int counterReps=0;

	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "As Much Rep");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 16, "Reps Elapsed");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 26, "Within 05:23");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 38, "Round Time Remaining");


	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(32, 15, (String)counterReps);
	display.display();

	int counter = 1;

	int totalTimeInSeconds = time[0] * 60 * 10;
	totalTimeInSeconds += time[1] * 60;
	totalTimeInSeconds += time[2] * 10;
	totalTimeInSeconds += time[3];

	Serial.print("Total Seconds: ");
	Serial.println(totalTimeInSeconds);

	double totalTimeInMs = totalTimeInSeconds * 1000;

	long timeStamp = millis();
	long timeNow;
	long passTimer;

	int progress = 0;
	int secElapse = 0;
	int minElapse = 0;
	double dprogress = 0;

	boolean finishRound = false;

	while (progress<100) {

		if (digitalRead(BUT_RIGHT) == LOW) {
			debounce(BUT_RIGHT);
			counterReps++;
			drawRTFCountingAMRAP(counterReps, secElapse, minElapse);
			dprogress = ((millis() - timeStamp) / totalTimeInMs);
			progress = dprogress * 100;
			display.drawProgressBar(0, 50, 120, 10, progress);
			display.display();
		}

		if (millis() - passTimer > 1000) {

			secElapse++;
			if (secElapse > 59) {
				minElapse++;
				secElapse = 0;
			}
			drawRTFCountingAMRAP(counterReps, secElapse, minElapse);

			dprogress = ((millis() - timeStamp) / totalTimeInMs);
			progress = dprogress * 100;
			display.drawProgressBar(0, 50, 120, 10, progress);
			display.display();

			passTimer = millis();
		}

		delay(5);
	}

		display.clear();
		String remainingReps = (String)(counterReps) + " Reps";
		String youDid = "Performed within Time-cap";

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_16);
		display.drawString(64, 0, "TIME-OUT!");

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_24);
		display.drawString(64, 15, remainingReps);

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_10);
		display.drawString(64, 40, youDid);


	display.display();

	while ((digitalRead(BUT_OK) == HIGH) && (digitalRead(BUT_RIGHT) == HIGH) && (digitalRead(BUT_LEFT) == HIGH)) {
		delay(20);
	}

	delay(500);
	repeatDialog(false);

	while (1) {
		if (digitalRead(BUT_RIGHT) == LOW) {
			display.clear();
			display.display();
			debounce(BUT_RIGHT);
			delay(1000);
			startAMRAP(time);
		}
		else if (digitalRead(BUT_LEFT) == LOW) {
			delay(50);
			ESP.restart();
		}
		delay(20);
	}

}


void startRFT(int time[], int targetRound) {


	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 16, "Round to Go");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 26, "Within 05:23");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 38, "Round Time Remaining");


	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(32, 15, (String)targetRound);
	display.display();

	int counter = 1;

	int totalTimeInSeconds = time[0] * 60 * 10;
	totalTimeInSeconds += time[1] * 60;
	totalTimeInSeconds += time[2] * 10;
	totalTimeInSeconds += time[3];

	Serial.print("Total Seconds: ");
	Serial.println(totalTimeInSeconds);

	double totalTimeInMs = totalTimeInSeconds * 1000;

	long timeStamp = millis();
	long timeNow;
	long passTimer;

	int progress = 0;
	int secElapse = 0;
	int minElapse = 0;
	double dprogress = 0;

	boolean finishRound = false;
	int initialTarget = targetRound;

	while (progress<100) {
	
		if (digitalRead(BUT_RIGHT) == LOW) {
			debounce(BUT_RIGHT);
			targetRound--;
			drawRTFCounting(targetRound, secElapse, minElapse);
			dprogress = ((millis() - timeStamp) / totalTimeInMs);
			progress = dprogress * 100;
			display.drawProgressBar(0, 50, 120, 10, progress);
			display.display();
			if (targetRound == 0) {
				finishRound = true;
				break;
			}
		}

		if (millis() - passTimer > 1000) {

			secElapse++;
			if (secElapse > 59) {
				minElapse++;
				secElapse = 0;
			}
			drawRTFCounting(targetRound, secElapse,minElapse);
			
			dprogress = ((millis() - timeStamp) / totalTimeInMs);
			progress = dprogress * 100;
			display.drawProgressBar(0, 50, 120, 10, progress);
			display.display();

			passTimer = millis();
		}

		
		delay(5);
	}

	
	
	long finish = millis();

	if (finishRound) {

		String totalElapseTime;
		int sec = (finish - timeStamp)/1000;
		int remSec = (finish - timeStamp)%1000;
		int minute = sec / 60;

		Serial.print("Total MS");
		Serial.println(finish - timeStamp);

		Serial.print("Min: ");
		Serial.println(minute);

		Serial.print("Sec: ");
		Serial.println(sec);

		Serial.print("Millisec: ");
		Serial.println(remSec);

		if (minute < 10) {
			totalElapseTime += "0" + (String)minute;
		}
		else {
			totalElapseTime += (String)minute;
		}

		if (sec < 10) {
			totalElapseTime += ":0" + (String)sec;
		}
		else {
			totalElapseTime += ":" + (String)sec;
		}

		totalElapseTime += "." + (String)remSec;

		display.clear();

    	display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_16);
		display.drawString(64, 0, "CONRATS!");

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_24);
		display.drawString(64, 15, totalElapseTime);

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_16);
		display.drawString(64, 40, "Finish Time");

	} else {

		display.clear();
		String remainingReps=(String)(initialTarget-targetRound)+" Reps";
		String youDid = "Out of " + (String)(initialTarget) + " Target";

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_16);
		display.drawString(64, 0, "TIME-OUT!");

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_24);
		display.drawString(64, 15, remainingReps);

		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.setFont(ArialMT_Plain_16);
		display.drawString(64, 40, youDid);


	}
	
	display.display();

	while ((digitalRead(BUT_OK) == HIGH) && (digitalRead(BUT_RIGHT) == HIGH) && (digitalRead(BUT_LEFT) == HIGH)) {
		delay(20);
	}

	delay(500);
	repeatDialog(true);

	while (1) {
		if (digitalRead(BUT_RIGHT) == LOW) {
			display.clear();
			display.display();
			debounce(BUT_RIGHT);
			delay(1000);
			startRFT(time, initialTarget);
		}
		else if (digitalRead(BUT_LEFT) == LOW) {
			delay(50);
			ESP.restart();
		}
		delay(20);
	}

}

void repeatDialog(boolean type) {

	display.clear();
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	if (type) {
		display.drawString(64, 0, "Round For Time");
	}
	else {
		display.drawString(64, 0, "As Much Rep");
	}
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, "Repeat?");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "YES");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "HOME");

	display.display();
}

void drawRTFCounting(int targetRound, int secElapse, int minElapse) {

	String timeElapse;

	if (minElapse < 10) {
		timeElapse = timeElapse + "0" + (String)minElapse;
	}
	else {
		timeElapse =timeElapse +(String)minElapse;
	}

	if (secElapse < 10) {
		timeElapse += ":0" + (String)secElapse;
	}
	else {
		timeElapse += ":"+(String)secElapse;
	}

	timeElapse = "Elapsed " + timeElapse;

	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 16, "Round to Go");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 26, timeElapse);

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 38, "Round Time Remaining");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(30, 15, (String)targetRound);
}

void drawRTFCountingAMRAP(int targetRound, int secElapse, int minElapse) {

	String timeElapse;

	if (minElapse < 10) {
		timeElapse = timeElapse + "0" + (String)minElapse;
	}
	else {
		timeElapse = timeElapse + (String)minElapse;
	}

	if (secElapse < 10) {
		timeElapse += ":0" + (String)secElapse;
	}
	else {
		timeElapse += ":" + (String)secElapse;
	}

	timeElapse = "Elapsed " + timeElapse;

	display.clear();

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "As Much Reps");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 16, "Reps Performed");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(80, 26, timeElapse);

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 38, "Round Time Remaining");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(30, 15, (String)targetRound);
}

void drawTimeToSetAMRAP(int time[], byte blinkDigit, boolean blink) {


	display.clear();

	String timeToSet = (String)time[0] + (String)time[1] + ":" + (String)time[2] + (String)time[3];

	if (blinkDigit >= 2)blinkDigit++;

	if (blinkDigit < 5) {
		if (blink)timeToSet.setCharAt(blinkDigit, '_');
	}
	else {
		if (blinkDigit == 5) {
			display.fillCircle(110, 40, 5);
		}
		else if (blinkDigit == 6) {
			display.fillCircle(5, 40, 5);
		}
	}


	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "As Much Rep");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Min : Sec");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "GO");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Back");

	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, 49, "TimeCap");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, timeToSet);
	display.display();
}

void drawTimeToSet(int time[], byte blinkDigit, boolean blink) {


	display.clear();

	String timeToSet = (String)time[0] + (String)time[1] + ":" + (String)time[2] + (String)time[3];

	if (blinkDigit >= 2)blinkDigit++;

	if (blinkDigit < 5) {
		if (blink)timeToSet.setCharAt(blinkDigit, '_');
	}
	else {
		if (blinkDigit == 5) {
			display.fillCircle(110, 40, 5);
		}
		else if (blinkDigit == 6) {
			display.fillCircle(5, 40, 5);
		}
	}


	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 0, "Round For Time");

	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 39, "Min : Sec");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(128, 46, "GO");

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 46, "Back");

	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, 49, "TimeCap");
	
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 15, timeToSet);
	display.display();
}

void cycleCheck() {
	if (millis() - counterCheckCycle > timeCheckCycle) {
		refreshTime();
	}
}

void routine() {
	if (digitalRead(BUT_OK) == LOW) {
		Serial.println("BUT OK");
		debounce(BUT_OK);
		changeFramesManualWOD();

	}
	else if (digitalRead(BUT_RIGHT) == LOW) {
		Serial.println("BUT RIGHT");
		//Serial.println(ui.getUiState()->currentFrame);
		debounce(BUT_RIGHT);
		ui.nextFrame();
	}
	else if (digitalRead(BUT_LEFT) == LOW) {
		Serial.println("BUT LEFT");
		debounce(BUT_LEFT);
		ui.previousFrame();
	}
}

void debounce(int button) {
	while (digitalRead(button) == LOW) {

	}
}

#include <driver/adc.h>

String getBatteryLevel() {
	/*
	pinMode(7, INPUT);
	// function check battery here
	
	adc1_config_width(ADC_WIDTH_BIT_10);   //Range 0-1023 
	adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);  //ADC_ATTEN_DB_11 = 0-3,6V
	int vbat= adc1_get_raw(ADC1_CHANNEL_7); //Read analog

	//int vbat = analogRead(7);
	//vbat *= 2;

	String out = (String)vbat;
	*/
	return "100%";
}

String getClock() {

	return timeNow;
}


void refreshTime() {

	String strHour, strMinute;

	if (hour() < 10) {
		strHour = "0"+ (String)hour();
	}
	else {
		strHour = (String)hour();
	}

	if (minute() < 10) {
		strMinute = "0"+(String)minute();
	}
	else {
		strMinute = (String)minute();
	}

	timeNow = strHour + ':' + strMinute;// +':' + getStrDigit(minute());// +":" + getStrDigit(second());

}

void digitalClockDisplay() {
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(" ");
	Serial.print(month());
	Serial.print(" ");
	Serial.print(year());
	Serial.println();
}

String getStrDigit(int digit) {
	String text;
	if (digit < 10) text = "0";

	text += (String)digit;
	return text;
}

void printDigits(int digits) {
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if (digits < 10)
		Serial.print('0');
	Serial.print(digits);
}


void print_wakeup_reason() {
	esp_sleep_wakeup_cause_t wakeup_reason;

	wakeup_reason = esp_sleep_get_wakeup_cause();

	switch (wakeup_reason)
	{
	case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
	case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
	case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
	case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
	case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
	default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
	}
}

void deepSleep() {
	display.displayOff();

	esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); //1 = High, 0 = Low

												  //If you were to use ext1, you would use it like
												  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

												  //Go to sleep now
	Serial.println("Going to sleep now");
	esp_deep_sleep_start();

}