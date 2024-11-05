// Libraries & Inclusions
#include "Arduino.h"
#include <String.h>
#include <DisplayOurValues.h>
//_________________________________________________________________________________________________________________________________

// Global Variables & Instances
const int front_ir_pin = 26;
const int in_ir_pin = 27;
const int out_ir_pin = 28;
const int down_ir_pin = 29;
const int encoderClk = 16;
const int encoderDt = 17;
const int encoderSw = 3; //18; (switched to 3 (the start button pin) because there is no encoder button on the V1 PCB)
//const int startPin = 3;
const int stopPin = 2;

int badValue = 0;
bool clickedTwice = false;
int itemIncrements = 0;
// used for software debouncing
//const int reasonableTime = 14000;
//const int swReasonableTime = 330;
//int pastTime;
//int swPastTime;

enum turnStates {
	CW = 0,
	CCW = 1
};
int state = CW;

DisplayOurValues disp = DisplayOurValues();
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  setup1();
}

void loop() {
  loop1();
}

void setup1() {
  
  Serial.begin(115200);
  pinMode(in_ir_pin, INPUT);
  pinMode(out_ir_pin, INPUT);
  pinMode(front_ir_pin, INPUT);
  pinMode(down_ir_pin, INPUT);
  pinMode(encoderSw, INPUT);
  pinMode(encoderDt, INPUT);
  pinMode(encoderClk, INPUT);
// pinMode(startPin, INPUT);
  pinMode(stopPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderSw), swInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderClk), clkInterrupt, CHANGE);
 // attachInterrupt(digitalPinToInterrupt(startPin), startInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(stopPin), stopInterrupt, CHANGE);
  interrupts();
  disp.setup();
  //disp.setUpdateScreen(true);

}

void loop1() {
  //This if statement is currently redundant, but if we put it here, this core would basically be idle (aside from interrupts) except for when it needs to rewrite the screen
  //Would have to write a getUpdateScreen function
  /**if (disp.getUpdateScreen() == true) {
    disp.displayMenu();
  };*/

  //disp.setMenuType(DisplayOurValues::menuTypes::DISPLAYIR);
  //disp.displayIRValues();
  //disp.displayMenu();
	//disp.displayMenu();
  disp.displayValue("HELLO", false);
  //disp.displayIRValues();
}
//_________________________________________________________________________________________________________________________________

// USER FUNCTIONS
// this function tells the OLED to change accordingly when an encoder turn occurs
void clkInterrupt() {
  // the if(clickedTwice) statement is for the V1 PCB which was counting every encoder turn as 2 encoder turns
  if (clickedTwice) {

    // read the SCL and SDA lines, and retrieve the menu item currently selected and menu type
    int clkVal = digitalRead(encoderClk); //A
    int dtVal = digitalRead(encoderDt); //B
    int ind = disp.getMenuIndex();
    DisplayOurValues::menuTypes menu = disp.getMenuType();

    // were used for software debouncing - DELETE THESE IF THEY ARE NOT EVENTUALLY USED
    //int presentTime = micros();
    //int delta = presentTime-pastTime;

    // used for software debouncing
    // if (delta >= reasonableTime) {
      // if there have been two consecutive turns in the direction we weren't expecting, we are most likely turning the opposite direction, so switch the direction
      if (badValue >= 2) {
        state = !state;
      };
      
      // if we were turning clockwise before this (from the turner's perspective)...
      if (state == CW){
        // if the SCL and SDA values are different (which they should be if we are going clockwise)...
        if (clkVal ^ dtVal) {
          // depending on the menu, respond accordingly
          switch (menu) {
            case DisplayOurValues::menuTypes::STARTPTLETS:
            case DisplayOurValues::menuTypes::STARTPTNUMS:
            case DisplayOurValues::menuTypes::ENDPTLETS:
            case DisplayOurValues::menuTypes::ENDPTNUMS:
            case DisplayOurValues::menuTypes::SELDISPLAYVALUE:
              { 
                // this code is here because there is a possibility for the item value to go beyond the number of allowed values for that menu when turning clockwise
                // (think about taking one too many steps forward and falling off a dock)
                // if that happens, this code resets the item to be the first allowed item for that menu

                // get the menu item of the menu item we will be selecting after the turn is executed
                String currentItem = disp.getMenuItem(menu, ind+1);
                // get the limits on the bounds of the values displayed for the current menus
                int limits = disp.getNumRowsOrCols();

                // For wraparound purposes
                String endItem;
                if (menu == DisplayOurValues::menuTypes::STARTPTLETS || menu == DisplayOurValues::menuTypes::ENDPTLETS) {
                  // these menus display letters, so find what the last allowed letter in them should be by starting at A and incrementing over their limits
                  endItem = "A";
                } else if (menu == DisplayOurValues::menuTypes::SELDISPLAYVALUE) { // is this the correct menu for here??
                  // this menu displays numbers, so find what the last allowed number in them should be by starting at 0 and incrementing over their limits
                  endItem = "0";
                } else { // I feel like this should be for seldisplayvalue and the previous case should be for the nums menus....
                  endItem = "1";
                };
                // iterate over their limits to find their last allowed values
                for (int i = 0; i < limits - 1; i++) {
                  endItem[0]++;
                };

                // if the item that will be selected after the turn is executed is the last allowed value in the menu...
                if (currentItem == endItem) {
                  // reassign the currentItem value to the first allowed value in the menu 9since we are turning CW) depending on menu type
                  if (menu == DisplayOurValues::menuTypes::STARTPTLETS || menu == DisplayOurValues::menuTypes::ENDPTLETS) {
                    currentItem = "A";
                  } else if (menu == DisplayOurValues::menuTypes::SELDISPLAYVALUE) {
                    currentItem = "0";
                  } else {
                    currentItem = "1";
                  };
                  // reset how many times the menu item has been incremented, since we have looped back around to its first allowed value
                  itemIncrements = 0;
                // if we aren't at the last allowed item in the menu...
                } else {
                  // increment the menu item to display the next allowed value
                  currentItem[0]++;
                  // increment the number of item increments tracker variable as well (since we incremented the menu item)
                  itemIncrements++;
                };
                // set the next item to be displayed (after the turn is executed) with the new item value
                disp.setMenuItem(menu, ind+1, currentItem);
              }
              break;
            case  DisplayOurValues::menuTypes::DISPLAYIR:
              // NOT CORRECT YET
              Serial.println("in right place");
              Serial.println(analogRead(down_ir_pin));
              break;
            // if we aren't in any of these special menus...
            default:
              // just increment the index so the next item in the menu list gets selected
              ind++;
              break;
          };
          // reset the number of "bad turn values"
          badValue = 0;
          // assert the direction
          state = CW;
        // if we were turning clockwise but the SCL and SDA lines AREN'T different, then we got a bad turn value
        } else {
          // increment the number of bad turn values
          badValue++;
        };
      // if we were going counterclockwise...
      } else {
        // if the SCL and SDA values are the same...
        if (clkVal ^! dtVal) {
          // depending on the menu, respond accordingly
          switch (menu) {
            case DisplayOurValues::menuTypes::STARTPTLETS:
            case DisplayOurValues::menuTypes::STARTPTNUMS:
            case DisplayOurValues::menuTypes::ENDPTLETS:
            case DisplayOurValues::menuTypes::ENDPTNUMS:
            case DisplayOurValues::menuTypes::SELDISPLAYVALUE:
              { 
                // this code is here because there is a possibility for the item value to go below the number of allowed values for that menu
                // (think about taking one too many steps backward and falling off a dock)
                // if that happens, this code resets the item to be the last allowed item for that menu

                // get the menu item of the menu item we will be selecting after the turn is executed
                String currentItem = disp.getMenuItem(menu, ind+1);
                // get the limits on the bounds of the values displayed for the current menus
                int limits = disp.getNumRowsOrCols();

                // if we are at the first allowed value, reassign the currentItem value depending on the menu type and currentItem value
                if (currentItem == "A" || (currentItem == "1" && menu != DisplayOurValues::menuTypes::SELDISPLAYVALUE) || (currentItem == "0" && menu == DisplayOurValues::menuTypes::SELDISPLAYVALUE)) {
                  // reassign the currentItem with the final allowed value for the menu by incrementing over its limits
                  for (int i = 0; i < limits - 1; i++) {
                    currentItem[0]++;
                  };
                  // reset the number of times the value has been incremented
                  itemIncrements = 0;
                // if we AREN'T at the first allowed value...
                } else {
                  // decrement the item value
                  currentItem[0]--;
                  // increment the number of times this menu item has been decremented
                  itemIncrements++;
                };
                // set the next item to be displayed (after the turn is executed) with the new item value 
                disp.setMenuItem(menu, ind+1, currentItem);
              }
              break;
            case  DisplayOurValues::menuTypes::DISPLAYIR:
              // NOT CORRECT YET
              Serial.println("in right place");
              Serial.println(analogRead(down_ir_pin));
              break;
            // if we aren't in any of these special menus...
            default:
              // just decrement the index so the previous item in the menu list gets selected
              ind--;
              break;
          };
          // reset the number of "bad turn values"
          badValue = 0;
          // assert the direction
          state = CCW;
        // if we were turning counterclockwise but the SCL and SDA values AREN'T the same, then we got a bad turn value
        } else {
          // increment the number of bad turn values
          badValue++;
        };
      };
    // }
    
    // used for software debouncing
    //pastTime = presentTime;
    // set the menu index of the item that will be selected after the turn is executed
    disp.setMenuIndex(ind);
    // set the updateScreen flag so that the OLED updates what it is displaying to reflect the newly-selected item
    disp.setUpdateScreen(true);

    // the clickedTwice assignment statement and the following else are for the V1 PCB which was counting every encoder turn as 2 encoder turns
    clickedTwice = false;
    } else {
      clickedTwice = true;
  }
}

// this function tells the OLED to change accordingly when the encoder button gets pushed
void swInterrupt() {
  // read the value of the encoder button
  int swVal = digitalRead(encoderSw);
  // used for software debouncing
  //int swPresentTime = millis();
	//int swDelta = swPresentTime-swPastTime;

  // used for software debouncing
  //if (swDelta >= 300) {
    // if the encoder button value is LOW (if it actually is being pushed)
    if (swVal == 0) { // is this necessary?...
      // tell the OLED screen to go to the appropriate menu
      disp.goToMenu();
    };
 // };
 
  // used for software debouncing
  //swPastTime = swPresentTime;
  // set the updateScreen flag to tell the OLED to update what it is displaying accordingly
  disp.setUpdateScreen(true);
}

// this function tells the OLED to change accordingly when the start button gets pushed
void startInterrupt() {
  // NOT CORRECT YET
  int ind = disp.getMenuIndex();
  DisplayOurValues::menuTypes menu = disp.getMenuType();
  switch (menu) {
    case DisplayOurValues::menuTypes::SELDISPLAYVALUE:
      disp.setMenuItem(menu, ind++, "0");
      disp.setMenuIndex(ind++);
      break;
    default:
      break;
  }
}

// this function tells the OLED to change accordingly when the stop button gets pushed
void stopInterrupt() {
  // if the stop button was pushed, go back to the MAIN menu and reset the item that is selected
  disp.setMenuType(DisplayOurValues::menuTypes::MAIN);
  disp.setMenuIndex(0);
  disp.setUpdateScreen(true);
}
//_________________________________________________________________________________________________________________________________
