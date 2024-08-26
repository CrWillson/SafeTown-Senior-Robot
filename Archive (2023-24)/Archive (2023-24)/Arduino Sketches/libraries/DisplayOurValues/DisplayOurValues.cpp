  /*
  DisplayOurValues.cpp - Library for displaying system values.
  Created by Larissa Robinson, October 10, 2023.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "DisplayOurValues.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// variables and instances
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//NOTE: Size 1 font seems to be 7 pixels tall

// Constructor
DisplayOurValues::DisplayOurValues()
{
	setup();
}

// setup function. Must be run in any sketch that writes to the OLED - taken from Adafruit
void DisplayOurValues::setup(void) {
	 // initialize serial monitor
	 Serial.begin(9600);

	 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	 if(!adaSSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
	   Serial.println(F("SSD1306 allocation failed"));
	   for(;;); // Don't proceed, loop forever
	 }
}

// function to test if basic functions for the OLED are working - taken from Adafruit
void DisplayOurValues::testScreen(void) {
	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	adaSSD1306.display();
	delay(2000); // Pause for 2 seconds

	// Clear the buffer
	adaSSD1306.clearDisplay();

	// Draw a single pixel in white
	adaSSD1306.drawPixel(10, 10, SSD1306_WHITE);

	// Show the display buffer on the screen. You MUST call display() after
	// drawing commands to make them visible on screen!
	adaSSD1306.display();
	delay(2000);
	// adaSSD1306.display() is NOT necessary after every single drawing command,
	// unless that's what you want...rather, you can batch up a bunch of
	// drawing operations and then update the screen all at once by calling
	// adaSSD1306.display(). These examples demonstrate both approaches...

	// call testdrawchar() to test drawing a character
	testdrawchar();

	// call testdrawstyles() to test drawing a style
	testdrawstyles();

	// Invert and then restore display, pausing in-between
	adaSSD1306.invertDisplay(true);
	delay(1000);
	adaSSD1306.invertDisplay(false);
	delay(1000);
}

// function to test drawing a character on the OLED
void DisplayOurValues::testdrawchar(void) {
	// clear the OLED buffer
	adaSSD1306.clearDisplay();

	// choose text settings
	adaSSD1306.setTextSize(1);      // Normal 1:1 pixel scale
	adaSSD1306.setTextColor(SSD1306_WHITE); // Draw white text
	adaSSD1306.setCursor(0, 0);     // Start at top-left corner
	adaSSD1306.cp437(true);         // Use full 256 char 'Code Page 437' font

	// write the character to the OLED buffer
	// Not all the characters will fit on the display. This is normal.
	// The library will draw what it can and the rest will be clipped.
	for(int16_t i=0; i<256; i++) {
	if(i == '\n') adaSSD1306.write(' ');
	else          adaSSD1306.write(i);
	}

	// post the buffer to the OLED (write to the screen), and then delay before doing anything else
	adaSSD1306.display();
	delay(2000);
}

// function to test drawing styles
void DisplayOurValues::testdrawstyles(void) {
	// clear the OLED buffer
	adaSSD1306.clearDisplay();

	// choose text settings
	adaSSD1306.setTextSize(1);             // Normal 1:1 pixel scale
	adaSSD1306.setTextColor(SSD1306_WHITE);        // Draw white text
	adaSSD1306.setCursor(0,0);             // Start at top-left corner

	// post a message to display to the OLED buffer
	adaSSD1306.println(F("Hello, world!"));

	// change the text color, and post another message to display to the OLED buffer
	adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
	adaSSD1306.println(3.141592);

	// change the text size, revert the text color, and post a third message to display to the OLED buffer
	adaSSD1306.setTextSize(2);             // Draw 2X-scale text
	adaSSD1306.setTextColor(SSD1306_WHITE);
	adaSSD1306.print(F("0x")); 
	adaSSD1306.println(0xDEADBEEF, HEX);

	// post the OLED buffer (that is now holding all three messages) to the OLED (write to the screen), and then delay before doing anything else
	adaSSD1306.display();
	delay(2000);
}

// display a single value on the OLED, choosing the text color
void DisplayOurValues::displayValue(String input, bool invert) {

	// clear the OLED buffer
	adaSSD1306.clearDisplay();
	
	// choose text settings
	adaSSD1306.setTextSize(1);             
	adaSSD1306.setCursor(0,0);  
	
	// set the text color
	if (invert) {
		adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
	} else {
		adaSSD1306.setTextColor(SSD1306_WHITE);        // Draw white text
	};
	
	// post the message to display to the OLED buffer
	adaSSD1306.println(input);
	
	// post the OLED buffer to the OLED (write to the screen)
	adaSSD1306.display();
}

// display the IR sensor ADC values on the OLED
void DisplayOurValues::displayIRValues() {
	
	// clear the OLED buffer
	adaSSD1306.clearDisplay();
	
	// choose text settings
	adaSSD1306.setTextSize(1);             
	adaSSD1306.setCursor(0,0); 
	
	// read the IR sensors
	int down = analogRead(down_ir_pin);
	int front = analogRead(front_ir_pin);
	int inner = analogRead(in_ir_pin);
	int outer = analogRead(out_ir_pin);
	
	// post the IR ADC values to the OLED buffer
	adaSSD1306.print("Down IR: ");
	adaSSD1306.println(down);
	adaSSD1306.print("Front IR; ");
	adaSSD1306.println(front);
	adaSSD1306.print("Inner Left IR: ");
	adaSSD1306.println(inner);
	adaSSD1306.print("Outer Left IR: ");
	adaSSD1306.println(outer);

	// post the OLED buffer (which now holds the IR ADC values) to the OLED (write to the screen)
	adaSSD1306.display();
}

// function to route to the correct menu given a menu item was clicked
void DisplayOurValues::goToMenu() {
	
	// get the menu type and menu index for the current menu type
	menuTypes menu = getMenuType();
	int index = getMenuIndex();
	
	// given the current menu type and index, route to the next appropriate menu
	// This switch statement is hardcoded (dislike)
	switch (menu) {
		// if in the MAIN menu...
		case MAIN:
			switch (index) {
				// if the current index (the MAIN menu item selected) is 1...
				case 1:
					// go to the NAV menu
					// Because of the way setMenuIndex() is written, setMenuType must always be called before setMenuIndex in cases like this
					setMenuType(NAV);
					break;
				// if the current index is 2...
				case 2:
					// go to the SELDISPLAYVALUE menu
					setMenuType(SELDISPLAYVALUE);
					break;
				// if the current index is 3...
				case 3:
					// go to the DISPLAYIR menu
					setMenuType(DISPLAYIR);
					break;
				// if the current index is any other value...
				default:
					break;
			};
			// set the index of the next menu to 0
			setMenuIndex(0);
			break;
		case NAV:
			switch (index) {
				case 1:
					setMenuType(STARTPTLETS);
					// set the next menu index to be 1 so that when you start turning on the next menu, the number displayed immediately starts changing
					setMenuIndex(1);
					break;
				case 2:
					setMenuType(ENDPTLETS);
					// set the next menu index to be 1 so that when you start turning on the next menu, the number displayed immediately starts changing
					setMenuIndex(1);
					break;
				case 3:
					setMenuType(MAIN);
					// reset the next menu index because MAIN is not a number selection menu
					setMenuIndex(0);
					break;
				default:
					// reset the next menu index because these menus are not a number selection menu
					setMenuIndex(0);
					break;
			};
			break;
		case SELDISPLAYVALUE:
			//NOT CORRECT YET
			{
				String temp;
				String temp2;
				switch (index) {
					case 0:
						// the user wants to input a second digit. Initialize the array item for a second digit
						setMenuItem(menu, 2, "0");
						
						// increment the index to be pointing at the second digit
						setMenuIndex(index+1);
						break;
					case 1:
						// the user selected a second digit. Route to DISPLAYVALUE menu
						setMenuType(DISPLAYVALUE);
						setMenuIndex(0);
						
						// get the pin number selected as a string
						temp = getMenuItem(SELDISPLAYVALUE, 1);
						temp2 = getMenuItem(SELDISPLAYVALUE, 2);
						temp = temp + temp2;
						
						// store the selected pin number in pinToDisplay
						pinToDisplay = atoi((temp).c_str());
						break;
					default:
						// otherwise, we are returning to the MAIN menu, so reset the array items back to zero before returning
						setMenuItem(SELDISPLAYVALUE, 1, "0");
						setMenuItem(SELDISPLAYVALUE, 2, "");
						setMenuType(MAIN);
						break;
				};
			}
			break;
		case DISPLAYVALUE:
			//NOT CORRECT YET
			// we are exiting the DISPLAYVALUE menu, so reset the SELDISPLAYVALUE menu array items (which were used in displaying the DISPLAYVALUE menu)
			setMenuItem(SELDISPLAYVALUE, 1, "0");
			setMenuItem(SELDISPLAYVALUE, 2, "");
			setMenuType(MAIN);
			setMenuIndex(0);
			break;
		case STARTPTLETS:
			switch (index) {
				case 0:
					// if the index is zero, the menu name is selected, so clicking it should do nothing
					break;
				default:
					setMenuType(STARTPTNUMS);
					break;
			};
			setMenuIndex(1);
			break;
		case DISPLAYIR:
			// NOT CORRECT YET
			setMenuType(MAIN);
			setMenuIndex(0);
			break;
		case STARTPTNUMS:
			{
				// get the start value selected as a string
				String row = getMenuItem(STARTPTLETS, 1);
				String col = getMenuItem(menu, 1);
				String select = "Selected: ";
				String startPt = select + row + col;
				switch (index) {
					case 0:
						// if the index is zero, the menu name is selected, so clicking it should do nothing
						break;
					default:
						// route to the CONFIRMSELECTION menu, so set array item 1 of the CONFIRMSELECTION menu to hold the value selected
						setMenuItem(CONFIRMSELECTION, 1, startPt);
						setMenuType(CONFIRMSELECTION);	
						setMenuIndex(0);		
						break;
				};
			}
			break;
		case ENDPTLETS:
			switch (index) {
				case 0:
					// if the index is zero, the menu name is selected, so clicking it should do nothing
					break;
				default:
					setMenuType(ENDPTNUMS);
					break;
			};
			setMenuIndex(1);
			break;
		case ENDPTNUMS:
			{
				// get the start value selected as a string
				String row = getMenuItem(ENDPTLETS, 1);
				String col = getMenuItem(menu, 1);
				String select = "Selected: ";
				String endPt = select + row + col;
				switch (index) {
					case 0:
						// if the index is zero, the menu name is selected, so clicking it should do nothing
						break;
					default:
						// route to the CONFIRMSELECTION menu, so set array item 1 of the CONFIRMSELECTION menu to hold the value selected
						setMenuItem(CONFIRMSELECTION, 1, endPt);
						setMenuType(CONFIRMSELECTION);
						setMenuIndex(0);				
						break;
				};
			}
			break;
		default: //CONFIRMSELECTION
			// reset the starting/ending point selection menu array values to their defaults
			setMenuItem(STARTPTLETS, 1, "A");
			setMenuItem(STARTPTNUMS, 1, "1");
			setMenuItem(ENDPTLETS, 1, "A");
			setMenuItem(ENDPTNUMS, 1, "1");
			switch (index) {
				case 2:
					// if the selected point is confirmed, go back to the MAIN menu
					setMenuType(MAIN);
					setMenuIndex(0);
					break;
				case 3:
					// if the selected point is incorrect, go back to the STARTPTLETS menu to select a new point
					setMenuType(STARTPTLETS);
					setMenuIndex(1);
					break;
				default:
					// if they click anywhere else, do nothing
					break;
			};
			break;
	};
}

// get the current menu type
DisplayOurValues::menuTypes DisplayOurValues::getMenuType() {
	return menuType;
}

// set the menu type (which will change it to the current menu type)
void DisplayOurValues::setMenuType(menuTypes type) {
	menuType = type;
}

// get the current menu index
int DisplayOurValues::getMenuIndex() {
	// get the current menu type
	menuTypes menu = getMenuType();
	
	// return the menu index based on the menu type
	//hardcoded
	switch (menu) {
		case MAIN:
			return mainIndex;
			break;
		case NAV:
			return navIndex;
			break;
		case SELDISPLAYVALUE:
			return displayValueIndex;
			break;
		case STARTPTLETS:
			return startPtLetsIndex;
			break;
		case STARTPTNUMS:
			return startPtNumsIndex;
			break;
		case ENDPTLETS:
			return endPtLetsIndex;
			break;
		case ENDPTNUMS:
			return endPtNumsIndex;
			break;
		default:
			return confirmSelectionIndex;
			break;
	};
}

// set the current menu index
void DisplayOurValues::setMenuIndex(int index) {
	// get the current menu type
	menuTypes menu = getMenuType();
	
	// set the current menu index
	//hardcoded
	switch (menu) {
		case MAIN:
			//hardcoded
			// if the index is greater than the index of the last item in the menua array, wrap the index back around to zero
			if (index >= mainLength) {
				index = 0;
			} else if (index < 0) {
				// if the index is less than zero, wrap the index around to point to the last item in the menu array
				index = mainLength-1;
			};
			
			// set the menu index
			mainIndex = index;
			break;
		case NAV:
			//hardcoded
			if (index >= navLength) {
				index = 0;
			} else if (index < 0) {
				index = navLength-1;
			};
			navIndex = index;
			break;
		//RENAME THE VARIABLE IN THE NEXT CASE
		case SELDISPLAYVALUE:
			//hardcoded
			if (index >= displayValueMenuLength) {
				index = 0;
			} else if (index < 0) {
				index = displayValueMenuLength-1;
			};
			displayValueIndex = index;
			break;
		case STARTPTLETS:
			if (index >= selPtLetsLength) {
				index = 0;
			} else if (index < 0) {
				index = selPtLetsLength-1;
			};
			startPtLetsIndex = index;
			break;
		case STARTPTNUMS:
			if (index >= selPtNumsLength) {
				index = 0;
			} else if (index < 0) {
				index = selPtNumsLength-1;
			};
			startPtNumsIndex = index;
			break;
		case ENDPTLETS:
			if (index >= selPtLetsLength) {
				index = 0;
			} else if (index < 0) {
				index = selPtLetsLength-1;
			};
			endPtLetsIndex = index;
			break;
		case ENDPTNUMS:
			if (index >= selPtNumsLength) {
				index = 0;
			} else if (index < 0) {
				index = selPtNumsLength-1;
			};
			endPtNumsIndex = index;
			break;
		default:
			if (index >= confirmSelectionLength) {
				index = 0;
			} else if (index < 0) {
				index = confirmSelectionLength-1;
			};
			confirmSelectionIndex = index;
			break;
	};
	
}

// display the current menu's array items on the OLED
void DisplayOurValues::displayMenu() {
	// if the updateScreen flag is true (otherwise don't waste time/power rewriting the screen)
	if (updateScreen == true) {
		
		// get the current menu's attributes, including the index of the currently-selected menu item (index)
		menuTypes menu = getMenuType();
		int length = getMenuLength();
		int index = getMenuIndex();
		
		// the screen can only display 4 rows of text if the text size is 1, so determine the current screen range given the currently-selected menu item
		// (the current screen range changes if the encoder is turned to items off the screen -
		// it adjusts to always show from the item above the selected item to however many after it can be displayed on the screen)
		int startIndex = index >= numRowsOnScreen-1 ? index-1 : 0;
		int endIndex = index >= numRowsOnScreen ? startIndex + 1 : length-1;
	
		//hardcoded
		// set the starting pixel location (x-coordinate) given the menu type (for spacing purposes)
		int startPx;
		if (menu == MAIN) {
			startPx = 55;
		} else if (menu == NAV) {
			startPx = 40;
		} else if (menu == CONFIRMSELECTION) {
			startPx = 25;
		} else if (menu == DISPLAYIR) {
			startPx = 0;
		} else {
			startPx = 17;
		};
	
		// clear the OLED buffer, & choose text settings
		adaSSD1306.clearDisplay();
		adaSSD1306.setTextSize(1);             
		adaSSD1306.setCursor(startPx,0);  
		
		// print the values of the current menu
		if (menu != DISPLAYVALUE && menu != DISPLAYIR) {
			
			// iterate over the length of the menu array
			for (int i = 0; i < length; i++) {
				
				// determine if the next item to be printed is in the current screen range
				bool inPrintRange = i >= startIndex && i <= endIndex;
				
				// if we aren't in certain menus and if the selected item's index matches the iteration variable value, invert the color of the selected item
				if (i == index && menu != STARTPTLETS && menu != ENDPTLETS && menu != STARTPTNUMS && menu != ENDPTNUMS && menu != SELDISPLAYVALUE) {
					adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
				} else {
					// if this isn't the selected item, don't invert its color
					adaSSD1306.setTextColor(SSD1306_WHITE);
				};
				
				// if we are on the first menu array item or if we are in the current screen range, print the current menu item
				if (i == 0 || inPrintRange) {
			
					// if the current menu type is SELDISPLAYVALUE and we aren't on the first item, don't print a newline between every subsequent menu item
					if (menu == SELDISPLAYVALUE && i >= 1) {
						adaSSD1306.print(getMenuItem(menu, i));
					} else {
						// if the current menu type is anything else, print a newline between every menu item
						adaSSD1306.println(getMenuItem(menu, i));
					};
				};
			};
		// if the current menu type is DISPLAYVALUE, print the value of the selected pin (which is not stored in the menu's array)
		} else if (menu == DISPLAYVALUE){
			// read the value of the selected pin
			// WHAT IF THIS IS A DIGITAL PIN????
			int pinVal = analogRead(pinToDisplay);
			adaSSD1306.println(pinVal);
		} else {
			// if the current menu type is DISPLAYIR, call the displayIRValues() function to display their values (which are not stored in the menu's array)
			adaSSD1306.setTextColor(SSD1306_WHITE);
			displayIRValues();
		};
		
		// post the OLED buffer to the OLED
		adaSSD1306.display();
		
		// reset the updateScreen flag so the screen doesn't get rewritten when it doesn't have to be
		updateScreen = false;
	};
}

// get the currently-selected item of the current menu
String DisplayOurValues::getMenuItem(menuTypes menu, int index) {
	//hardcoded
	// return the currently-selected menu item of the current menu
	switch (menu) {
		case MAIN:
			return mainMenu[index];
		case NAV:
			return navMenu[index];
		case STARTPTLETS:
			return startPtLetters[index];
		case STARTPTNUMS:
			return startPtNums[index];
		case ENDPTLETS:
			return endPtLetters[index];
		case ENDPTNUMS:
			return endPtNums[index];
		case CONFIRMSELECTION:
			return confirmSelection[index];
		case SELDISPLAYVALUE:
			return displayValueMenu[index];
		case DISPLAYIR:
			return displayIRMenu[index];
		default:
			return "";
	};
}

// set an item in the current menu
void DisplayOurValues::setMenuItem(menuTypes menu, int index, String val) {
	//hardcoded
	// set the item at the chosen index in the chosen menu with the chosen value
	switch (menu) {
		case STARTPTLETS:
			startPtLetters[index] = val;
			break;
		case ENDPTLETS:
			endPtLetters[index] = val;
			break;
		case STARTPTNUMS:
			startPtNums[index] = val;
			break;
		case ENDPTNUMS:
			endPtNums[index] = val;
			break;
		case CONFIRMSELECTION:
			confirmSelection[index] = val;
			break;
		case SELDISPLAYVALUE:
			displayValueMenu[index] = val;
		case DISPLAYIR:
			displayIRMenu[index] = val;
		default:
			break;
	};
}

// get the length of the current menu
int DisplayOurValues::getMenuLength() {
	// get the current menu type
	menuTypes menu = getMenuType();
	
	//hardcoded
	// return the length of the current menu
	switch (menu) {
		case MAIN:
			return mainLength;
		case NAV:
			return navLength;
		case STARTPTLETS:
		case ENDPTLETS:
			return selPtLetsLength;
		case STARTPTNUMS:
		case ENDPTNUMS:
			return selPtNumsLength;
		case CONFIRMSELECTION:
			return confirmSelectionLength;
		case SELDISPLAYVALUE:
			return displayValueMenuLength;
		case DISPLAYIR:
			return displayIRMenuLength;
		default:
			return 1;
	};
}

// set the updateScreen variable
void DisplayOurValues::setUpdateScreen(bool update) {
	// set this value if the OLED needs to be updated
	updateScreen = update;
}

// get the number of rows or columns given the current menu type
int DisplayOurValues::getNumRowsOrCols() {
	// get the current menu type
	menuTypes menu = getMenuType();
	
	//hardcoded
	// return the number of rows or columns appropriate to the menu
	switch (menu) {
		// return the number of rows in the town
		case STARTPTLETS:
		case ENDPTLETS:
			return numRows;
		// return the number of columns in the town
		case STARTPTNUMS:
		case ENDPTNUMS:
			return numCols;
		// return the value beyond which ASCII values for digits don't exist (for encoder library wraparound purposes)
		case SELDISPLAYVALUE:
			return 10;
		default:
			return 0;
	};
}