/** Junior Lesson 5 Student Sketch: I2C Communication, OLED Display, and Collecting Data
* This sketch uses the DisplayOurValues library to write a message on the OLED display
* and to display the IR values on the OLED display.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <String.h>
#include <DisplayOurValues.h>
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values of the IR sensors
//***********************************************************************************************************************
const int downIrPin = /** insert the down IR sensor GPIO pin number here*/ ; // THESE ARE OLD PIN NUMBERS SO THEY WILL NEED UPDATED WHEN THE NEW PCBs ARRIVE
const int frontIrPin = /** insert the front IR sensor GPIO pin number here*/ ; 
const int outerIrPin = /** insert the outer IR sensor GPIO pin number here*/ ;
const int innerIrPin = /** insert the inner IR sensor GPIO pin number here*/ ;
//***********************************************************************************************************************

// these hold important values the adaSSD1306 library functions need to use
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// make an instance of the class adaSSD1306
// IMPORTANT NOTE: Comment out the following line when programming for Section 3.
//Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// make an instance of the class DisplayOurValues
// IMPORTANT NOTE: Comment out the following line when programming for Section 2.
DisplayOurValues display = DisplayOurValues();
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:

  // set the modes of the GPIO pins you will use
  pinMode(downIrPin, INPUT);
  pinMode(frontIrPin, INPUT);
  pinMode(outerIrPin, INPUT);
  pinMode(innerIrPin, INPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // IMPORTANT NOTE: Comment out the following if statement when programming for Section 3.
  // if(!adaSSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }

  // run the setup function for the DisplayOurValues library
  // IMPORTANT NOTE: Comment out the following line when programming for Section 2.
  display.setup();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Section 2
  // IMPORTANT NOTE: Comment out the following Section 2 code when programming for Section 3.
  // establish some default settings for the display
  // adaSSD1306.clearDisplay();  // clear what was previously on the screen
  // adaSSD1306.setTextSize(1);      // set the size of the text to display. An input of 1 indicates the normal 1:1 pixel scale
  // adaSSD1306.setTextColor(SSD1306_WHITE); // print white text
  // adaSSD1306.setCursor(0, 0);     // start writing at the top-left corner (xy coordinates (0, 0)). Since this gets set every loop, every subsequent message will be written in the same place

  // Section 2.1
  // print the message "Hello, World!" to the OLED display using adaSSD1306 functions
  //***********************************************************************************************************************
  // display "Hello, World!" on the OLED using adaSSD1306 functions
  //***********************************************************************************************************************

  // Section 2.2
  // read the IR sensors, and print the ADC values of the IR sensors to the OLED display using adaSSD1306 functions
  //***********************************************************************************************************************
  // read the IR sensors' ADC values

  // display the IR ADC values on the OLED display all at once using adaSSD1306 functions
  //***********************************************************************************************************************

  // Section 3
  // IMPORTANT NOTE: Comment out the following Section 3 code when programming for Section 2.
  // Section 3.1
  // print the message "Hello, World!" to the OLED display using DisplayOurValues functions
  //***********************************************************************************************************************
  // display "Hello, World!" on the OLED using DisplayOurValues functions
  //***********************************************************************************************************************

  // Section 3.2
  // IMPORTANT NOTE: Comment out the following Section 3.2 statements when programming for Section 3.1
  // display the ADC values of the IR sensors on the OLED display using DisplayOurValues functions
  //***********************************************************************************************************************
  // display the IR ADC values on the OLED all at once using DisplayOurValues functions
  //***********************************************************************************************************************  
}
//_________________________________________________________________________________________________________________________________ 
