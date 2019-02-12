// 
// 
// 

#include "SPIFlashSystem.h"

#include "FS.h"
#include "SPIFFS.h"
#include "Arduino.h"
#include "workOuts.h"

/* You only need to format SPIFFS the first time you run a
test or else use the SPIFFS plugin to create a partition
https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void beginSPIFFS() {
	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed");
		return;
	}
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root) {
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file) {
		if (file.isDirectory()) {
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		}
		else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}

void readFile(fs::FS &fs, const char * path) {
	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if (!file || file.isDirectory()) {
		Serial.println("- failed to open file for reading");
		return;
	}

	Serial.println("- read from file:");
	while (file.available()) {
		Serial.write(file.read());
		//SerialBT.write(file.read());
	}
}

void writeFile(fs::FS &fs, const char * path, const char * message) {

	//setupBLE();
	Serial.printf("Writing file: %s\r\n", path);

	File file = fs.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("- failed to open file for writing");
		return;
	}

	boolean start = true;
	long timeStamp;
	long counter;

	while (1) {


		if (Serial.available() > 0) {
			if (start) {
				start = false;
				timeStamp = millis();
			}
			counter++;
			file.write(Serial.read());
		}

		if (counter>1061167000) {
			counter = 0;
			Serial.print("Time: ");
			Serial.println(millis() - timeStamp);
		}

		if (digitalRead(23) == LOW) {
			file.close();
			delay(100);
			readFile(SPIFFS, "/hello.txt");
			Serial.println("DONE");
			Serial.println();
			Serial.println();
			Serial.println(counter);
			Serial.println();
			Serial.println();
			ESP.restart();
		}
	}

	/*
	if (file.print(message)) {
	Serial.println("- file written");
	} else {
	Serial.println("- frite failed");
	}
	*/

}

void appendFile(fs::FS &fs, const char * path, const char * message) {
	Serial.printf("Appending to file: %s\r\n", path);

	File file = fs.open(path, FILE_APPEND);
	if (!file) {
		Serial.println("- failed to open file for appending");
		return;
	}
	if (file.print(message)) {
		Serial.println("- message appended");
	}
	else {
		Serial.println("- append failed");
	}
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
	Serial.printf("Renaming file %s to %s\r\n", path1, path2);
	if (fs.rename(path1, path2)) {
		Serial.println("- file renamed");
	}
	else {
		Serial.println("- rename failed");
	}
}

void deleteFile(fs::FS &fs, const char * path) {
	Serial.printf("Deleting file: %s\r\n", path);
	if (fs.remove(path)) {
		Serial.println("- file deleted");
	}
	else {
		Serial.println("- delete failed");
	}
}

void testFileIO(fs::FS &fs, const char * path) {
	Serial.printf("Testing file I/O with %s\r\n", path);

	static uint8_t buf[512];
	size_t len = 0;
	File file = fs.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("- failed to open file for writing");
		return;
	}

	size_t i;
	Serial.print("- writing");
	uint32_t start = millis();
	for (i = 0; i<2048; i++) {
		if ((i & 0x001F) == 0x001F) {
			Serial.print(".");
		}
		file.write(buf, 512);
	}
	Serial.println("");
	uint32_t end = millis() - start;
	Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
	file.close();

	file = fs.open(path);
	start = millis();
	end = start;
	i = 0;
	if (file && !file.isDirectory()) {
		len = file.size();
		size_t flen = len;
		start = millis();
		Serial.print("- reading");
		while (len) {
			size_t toRead = len;
			if (toRead > 512) {
				toRead = 512;
			}
			file.read(buf, toRead);
			if ((i++ & 0x001F) == 0x001F) {
				Serial.print(".");
			}
			len -= toRead;
		}
		Serial.println("");
		end = millis() - start;
		Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
		file.close();
	}
	else {
		Serial.println("- failed to open file for reading");
	}
}

void fileTest1(void) {
	listDir(SPIFFS, "/", 0);
	writeFile(SPIFFS, "/hello.txt", "Hello ");

}


String XMLSearchPhrase[14] = {"wod name",
							"section name",
							"config type",
							"<resetclock>",
							"<rounds>",
							"<timecap>",
							"<hardcap>",
							"<split>",
							"<fulltime>",
							"<movement seq>",
							"<name>",
							"<reps>",
							"<split>"
							};

boolean XMLSearchScanType[14] = { 1,1,1,0,0,0,0,0,0,1,0,0,0 };

int counterWod = 0;
int counterMovements = 0;

void scanXML(fs::FS &fs, const char * path) {

	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if (!file || file.isDirectory()) {
		Serial.println("- failed to open file for reading");
		return;
	}

	long counterLF = 0;
	Serial.println("- read from file:");

	String bufferPerLine;

	while (file.available()) {
		char in = file.read();

		bufferPerLine+=in;

		// #########################################################

		if (in == 13) {
			counterLF++;

			if (StringContains(bufferPerLine, XMLSearchPhrase[counterWod])!=-1) {
				Serial.print("Found!: ");
				Serial.print(XMLSearchPhrase[counterWod]);
				Serial.print(" = ");

				//Serial.println(cropStringQuotationMarks(bufferPerLine));



				switch (counterWod) {
				case 0: // WOD NAME
					if (StringContains(bufferPerLine, XMLSearchPhrase[counterWod]) != -1) {
						setWorkoutsName(counterWod, cropStringQuotationMarks(bufferPerLine));
					};
					break;
				case 1: // SECTION NAME

					break;
				case 2: // CONFIG TYPE

					break;
				case 3: // RESET CLOCK

					break;
				case 4: // ROUNDS

					break;
				case 5: // TIMECAP

					break;
				case 6: // HARDCAP

					break;
				case 7: // SPLIT

					break;
				case 8: // FULLTIME

					break;
				case 9: // MOVEMENT SEQUENCE

					break;
				case 10: // NAME

					break;
				case 11: // REPS

					break;
				case 12: // SPLIT

					break;		
				}

				/*
				if (XMLSearchScanType[counterWod]) {
					setWorkoutsName(counterWod, cropStringQuotationMarks(bufferPerLine));
					Serial.println(getWorkoutsName(counterWod));
				}
				else {
					setWorkoutsName(counterWod, cropStringQuotationBrackets(bufferPerLine));
					Serial.println(getWorkoutsName(counterWod));
				}
				*/

			}
			bufferPerLine = "";
		}

		// ##########################################################
	}

	Serial.print("Total Lines: ");
	Serial.println(counterLF);

}

void fileTest2(void) {
	//setupBLE();
	scanXML(SPIFFS, "/hello.txt");
}

int StringContains(String s, String search) {
	int max = s.length() - search.length();
	int lgsearch = search.length();

	for (int i = 0; i <= max; i++) {
		if (s.substring(i, i + lgsearch) == search) return i;
	}

	return -1;
}

String cropStringQuotationMarks(String text) {
	int startIndex, endIndex;
	int length = text.length();
	boolean foundFirst = false;

	for (int i = 0; i < length; i++) {

		if (text.charAt(i) == '"' && foundFirst) {
			endIndex = i;
			break;
		}
		if (text.charAt(i) == '"' && !foundFirst) {
			startIndex = i+1;
			foundFirst = true;
		}
	}

	return text.substring(startIndex, endIndex);
}

String cropStringQuotationBrackets(String text) {
	int startIndex, endIndex;
	int length = text.length();
	
	boolean foundFirst = false;

	for (int i = 0; i < length; i++) {

		if (text.charAt(i) == '<' && foundFirst) {
			endIndex = i;
			break;
		}
		if (text.charAt(i) == '>' && !foundFirst) {
			startIndex = i + 1;
			foundFirst = true;
		}
	}

	return text.substring(startIndex, endIndex);
}