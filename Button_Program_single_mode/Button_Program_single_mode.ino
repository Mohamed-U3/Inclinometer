//  ==============
//    User Setup
//  ==============

//NOTE: for any power or auto power on code, check "1_Button_Program_Test_v3.ino" file

String codeTitle = ("3D Archery LED");
String codeVersion = ("PROTO V37 SM");  // Update this number when changes are made.
                                           // Number will be displayed on start up screen.

//bool printToSerialMonitor = false;  // Print debug info to serial monitor?


//  =============
//    Libraries
//  =============

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include "MAX17043.h"
//#include <TimerOne.h>
//https://github.com/PaulStoffregen/TimerOne/blob/master/examples/Interrupt/Interrupt.ino

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//#include <AutoPower.h>

Adafruit_MPU6050 mpu;

//  ========
//    OLED
//  ========
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDR 0x3C
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
// Set OLED resolution
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

//  ==========
//    Inputs
//  ==========
const int buttonUp = A0;       //  Down button
const int buttonDown = A1;     //  Up button
const int buttonCentre = 2;    // Confirm button
const int buttonLeft = A2;     //  Up button
const int buttonRight = A3;    // Confirm button
const int ledPin = 3;          // the number of the LED pin
const int maxBrightness = 21;  //this can be any number - it's the number of steps between dimmest and brightest.
const int powerPin = 7;
//int PwrPin = 7;

//Added for angle screen
int flashcount = 0;
int iii = 10;
double endt, startt, duration;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int minVal = 265;
int maxVal = 402;
double z = 0;
bool GlobalUpsideDownFlag = false;
int xAng, yAng;
const int MPU_addr = 0x68;

//  ============================================
//    Variables - these will change during use
//  ============================================
long lastInteractionTime = 0;
long autoPowerOffTime = 180;
long powerButtonHoldTimer = 0;
long debounceTimer = 0;
int debounceDelay = 50; //Increase delay if multiple button presses are registering in a single press of the button.

bool mode = 0;  //0=Practice 1=Comp

bool screensetup = true;
bool powersetup = true;

int flash = 0;

int menuPageCounter = 1;
int menuNum = 1;
boolean startpage = true;
int subMenuCounter = 1;
//bool breakMenuLoop = false;

//bool buttonActive = false;
//bool longPressActive = false;

//int buttonUp_state = 0;
//int buttonUp_lastState = 0;

//int buttonDown_state = 0;
//int buttonDown_lastState = 0;

int buttonCentre_state = 0;
int buttonCentre_lastState = 0;

int buttonLeft_state = 0;
int buttonLeft_lastState = 0;

int buttonRight_state = 0;
int buttonRight_lastState = 0;

//String pageTitle = "";
//int pageLayout = 0;
//int pageContentTextSize;

//String infoToDisplay = "";
//int cursorLocation_X;
//int cursorLocation_Y;

int brightness = 0;
int interval = 1;
int flag = 0;

//unsigned long lastDebounceTime = 0;
//unsigned long debounceDelay = 10;

float percentage, volts;

bool centreChange, looping, leftchange, rightchange; // upchange, downchange,

//  =========================================
//    Non-variables - these will not change
//  =========================================
#define powerOffHoldTime 5
//  =========================================
//                AutoPower
//  =========================================
// Note: last parameter is optional (defaults to 2 seconds if omitted)
//AutoPower power(powerPin, buttonCentre, 2);   // Hold button 2 seconds to power off

void setup()
{
  pinMode(powerPin, "OUTPUT");
  digitalWrite(powerPin, 1);
  // Start serial comms
  Serial.begin(9600);
  //Serial.println("Line 169: setup function");
  //assign counter the value of address 0
  brightness = EEPROM.read(0);
  //autoPowerOffTime = EEPROM.read(10);
  
  //write a 0 to address 0. This allows for consecutive resets to reset the counter
  EEPROM.write(0, 0);

  // Initialize Fuel Guage
  FuelGauge.begin();

  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  // Define pinModes
  //pinMode(buttonUp, INPUT_PULLUP);      // up button input
  //pinMode(buttonDown, INPUT_PULLUP);    // down button input
  pinMode(buttonCentre, INPUT_PULLUP);  // Menu button Input
  pinMode(buttonLeft, INPUT_PULLUP);    // Left button Input
  pinMode(buttonRight, INPUT_PULLUP);   // Right button Input
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  setupScreens();  //This triggers the two startup screens after the power switch

  //Interrupt setup
  //Timer1.initialize(500);             //sets software timing interrupt to occur every 1ms
  //Timer1.attachInterrupt(checkPage);  // blinkLED to run every 0.15 seconds
  delay(3000);                        //moved from menu case 1
  mainMenu();                         //start the menu options
  lastInteractionTime = millis();
  //power.setTimeout(autoPowerOffTime);
}

void loop() 
{
  if (flag == 1)//checks if flag == 1 from the checkPage func
  {
    //Timer1.stop();  //stop the interrupt
    mainMenu();     //goes to menu 1, change brightness first
    flag = 0;       //so flag works fresh the next time
    //Timer1.restart();
  }
  checkPage();
  flag = 1;
//  autoPowerOff();
  //power.updatePower();                // Call if setTimeout() used
}  // End of Loop
