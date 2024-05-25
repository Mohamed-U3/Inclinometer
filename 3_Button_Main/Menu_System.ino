void mainMenu() {
  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  menuNum = menuPageCounter;
  interrupts();

  switch (menuNum) {
    case 0:
      checkButtonLeft();
      checkButtonRight();
      drawHeader(F("Mode Select"));

      display.setCursor(5, 20);   //  Sets coursor position
      display.print(F("Training"));  // set info
      display.setCursor(80, 20);  //  Sets coursor position
      display.println(F("Comp"));    // set info

      if (leftchange || rightchange){//(digitalRead(buttonLeft) == 0 || digitalRead(buttonRight) == 0) {  // if the state has changed, change selection
        mode = !mode;
        leftchange = false;
        rightchange = false;
        delay(125);  // Delay a little bit to avoid bouncing
      }

      //Highlight Selection
      if (mode == 0) {
        display.drawLine(5, 30, 50, 30, WHITE);  // draw underline
      } else {
        display.drawLine(80, 30, 103, 30, WHITE);  // draw underline
      }

      display.display();
      //checkPage();
      //}
      break;

    case 1:  // Menu Page 1, LED control
      checkButtonLeft();
      checkButtonRight();
      drawHeader(F("LED Brightness"));

      display.setCursor(5, 20);     //  Sets coursor position
      display.println(brightness);  // set info

      //// Check if button (Left) Pressed ////
      // compare the buttonState to its previous state
      if (leftchange){//(digitalRead(buttonLeft) == 0) {  // if the state has changed, increment the counter
        brightness = brightness - interval;
        leftchange = false;
      }
      //analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
      //EEPROM.write(0, brightness);                                     //write counter to address 0
      //buttonLeft_lastState = buttonLeft_state;                         // save the current state as the last state

      //// Check if button (Up) Pressed ////
      if (rightchange){//(digitalRead(buttonRight) == 0) {  // if the state has changed, increment the counter
        brightness = brightness + interval;
        rightchange = false;
      }
                                          //write counter to address 0
      //buttonRight_lastState = buttonRight_state;                       // save the current state as the last state

      if (brightness > 20) {
        brightness = 20;
      }
      if (brightness <= 0) {
        brightness = 0;
      }
      analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
      EEPROM.write(0, brightness); 

      display.display();  // update display
                          //checkPage();
      //}
      break;

    case 2:  // Menu Page 2, mpu initialisation
      drawHeader(F("MPU6050 test"));
      display.setCursor(5, 15);                 //  Sets coursor position
      display.print(F("Press button to"));
      display.setCursor(5, 23);  //  Sets coursor position
      display.print(F("start"));
      display.display();
      checkButtonLeft();
      checkButtonRight();
      if (leftchange || rightchange){//(digitalRead(buttonCentre) == 0) {
        leftchange = false;
        rightchange = false;
        // Try to initialize!
        delay(125);
         // MPU 6050 code
        drawHeader(F("MPU6050 test"));                 // set info
        display.setCursor(5, 20); 
        if (!mpu.begin()) {
          display.println(F("Error"));  // set info
          display.display();
        } else {
          display.print(F("MPU6050 Found!"));  // set info
          display.display();
          delay(1000);

          looping = true;
          while (looping) {
            flashcount = 0;
            //flash the screen twice
            while (flashcount < 2) {
              flashLED();
              flashcount++;
            }

            //reset the LED brightness back to original setting
            analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
            drawHeader(F("MPU6050 test"));
            display.setCursor(5, 20);                 //  Sets coursor position
            display.println(F("Draw Bow to Target"));    // set info
            display.display();
            delay(1000);

            //start 10 second counter to set angle
            startt = millis();
            endt = 0;
            duration = 10000;  //10000 ms = 10 seconds
            Wire.beginTransmission(MPU_addr);
            Wire.write(0x6B);
            Wire.write(0);
            Wire.endTransmission(true);
            int value = 0;
            while ((endt - startt) <= duration) {
              Wire.beginTransmission(MPU_addr);
              Wire.write(0x3B);
              Wire.endTransmission(false);
              Wire.requestFrom(MPU_addr, 14, true);
              AcX = Wire.read() << 8 | Wire.read();
              AcY = Wire.read() << 8 | Wire.read();
              xAng = map(AcX, minVal, maxVal, -90, 90);
              yAng = map(AcY, minVal, maxVal, -90, 90);
              z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
              if(z>180){
                z = z-360;
              }
              Serial.print(F("Current angle: "));
              //Serial.print("Numerical value:" + z);
              z = z - 90;
              value = round(z);
              value = abs(value);
              //if(value < 0){
              //  value = value * -1;
              //}
              Serial.println(String(value));
              delay(5000);

              drawHeader(F("MPU6050 test"));
              display.setCursor(5, 20);         //  Sets coursor position
              display.display();

              delay(250);  //controls text change speed

              endt = millis();
            }

            //flashScreen();
            flashLED();  //flash the LED

            //reset brightness of LED
            analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
            EEPROM.write(0, brightness);                                     //write counter to address 0

            delay(125);
            drawHeader(F("MPU6050 test"));
            display.setCursor(5, 20);                       //  Sets coursor position
            display.print(F("Target Angle: "));
            display.println(String(value));  // set info
            display.display();
            delay(2000);

            //if(!checkButtonCentre()){ //returns true if changed
            looping = false;
            //} //else centre button has been pushed, so reset and repeat the process
          }
        }
      }
      break;


    /*case 3:  // Menu Page 3, power off selector
      //// Page setup ////

      //Default auto power off is off
      if (digitalRead(buttonLeft) == 0 && autoPowerOffTime > 0) {
        autoPowerOffTime = autoPowerOffTime - 30;
        EEPROM.write(10, autoPowerOffTime); //write autoPowerOffTime to address 0
        //power.setTimeout(autoPowerOffTime);
      }
      if (digitalRead(buttonRight) == 0 && autoPowerOffTime < 300) {  // limit timer to max of 5 minutes
        autoPowerOffTime = autoPowerOffTime + 30;
        EEPROM.write(10, autoPowerOffTime); //write autoPowerOffTime to address 0
        //power.setTimeout(autoPowerOffTime);
      }

      drawHeader(F("Auto power off"));
      display.setCursor(5, 20);
      if(autoPowerOffTime == 0){                       //  Sets coursor position
        display.println(F("Disabled"));
      } else{
        float i = float(autoPowerOffTime)/60;
        display.print(i);
        display.print(" Min");
      }
      display.display();                        // update display

      break;
    */
    case 3:  // Menu Page 4, battery %
      percentage = FuelGauge.percent();  //update percentage variable
      drawHeader(F("Battery percentage:"));
      display.setCursor(5, 20);                 //  Sets coursor position
      display.print(percentage);                // set info
      display.setCursor(65, 20);                //  Sets coursor position
      display.println("%");                     // set info
      display.display();                        // update display
      delay(150);
      break;

    case 4:  // Menu Page 6, back to mode select
      drawHeader(F("Back"));
      display.setCursor(5, 15);                 //  Sets coursor position
      display.print(F("Return to mode"));
      display.setCursor(5, 23);  //  Sets coursor position
      display.print(F("select"));
      display.display();  // update display
      delay(150);
      break;
  }
}

void checkPage() {
  //Button Checking
  //checkButtonDown();
  //checkButtonUp();
  checkButtonCentre();
  checkButtonLeft();
  checkButtonRight();

  // Constrains menuPageCounter between 1 and 6
  /*if (upchange == true) {  // check for button up press
    //Serial.println(menuPageCounter);
    if (menuPageCounter != 0) {
      menuPageCounter--;
      upchange = false;
      downchange = false;
      centreChange = false;
      leftchange = false;
      rightchange = false;
      if (menuPageCounter < 1) {  // if page number goes lower than 0
        menuPageCounter = 4;      // reset to 4
      }
      if (menuPageCounter == 2 && mode) {
        menuPageCounter = 1;
        upchange = false;
        downchange = false;
        centreChange = false;
        leftchange = false;
        rightchange = false;
      }
      flag = 1;
      //breakMenuLoop = true;
    }
  } else if (downchange == true) {  // check for down button press
    //Serial.println(menuPageCounter);
    if (menuPageCounter != 0) {
      menuPageCounter++;
      upchange = false;
      downchange = false;
      centreChange = false;
      leftchange = false;
      rightchange = false;
      if (menuPageCounter >= 5) {  // if page number goes over 5
        menuPageCounter = 1;       // reset to 0
      }
      if (menuPageCounter == 2 && mode) {
        upchange = false;
        downchange = false;
        centreChange = false;
        leftchange = false;
        rightchange = false;
        menuPageCounter = 3;
        //Serial.println("Skipping 2");
      }
      //breakMenuLoop = true;
      flag = 1;  //force to start the void loop;
    }
  } else */ if (centreChange == true) {  // check for centre button press
    //Serial.println(menuPageCounter);
    if (menuPageCounter == 0) {  // check if we are on the mode select screen
      //upchange = false;
      //downchange = false;
      centreChange = false;
      leftchange = false;
      rightchange = false;
      menuPageCounter = 1;
      //breakMenuLoop = true;
      flag = 1;
    } else if (menuPageCounter == 4) {  // check if we are on the back to mode select screen
      //upchange = false;
      //downchange = false;
      centreChange = false;
      leftchange = false;
      rightchange = false;
      menuPageCounter = 0;
      flag = 1;
    }
    else{
      // Coppied from down button
      if (menuPageCounter != 0) {
      menuPageCounter++;
      //upchange = false;
      //downchange = false;
      centreChange = false;
      leftchange = false;
      rightchange = false;
      if (menuPageCounter >= 5) {  // if page number goes over 5
        menuPageCounter = 1;       // reset to 0
      }
      if (menuPageCounter == 2 && mode) {
        //upchange = false;
        //downchange = false;
        centreChange = false;
        leftchange = false;
        rightchange = false;
        menuPageCounter = 3;
        //Serial.println("Skipping 2");
      }
      // end of coppied from down
      flag = 1;
    }
    }

  } else if (leftchange || rightchange) {
    //Serial.println("Left or right pressed");
    flag = 1;
  }
  
}

void checkMPU()
{
  if (!mpu.begin())
  {
    display.println(F("Error in MPU6050"));  // set info
    display.display();
  }
  else
  {
    //start 10 second counter to set angle
    startt = millis();
    endt = 0;
    duration = 50;  //50 ms
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    int value = 0;
    while ((endt - startt) <= duration)
    {
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr, 14, true);
      AcX = Wire.read() << 8 | Wire.read();
      AcY = Wire.read() << 8 | Wire.read();
      xAng = map(AcX, minVal, maxVal, -90, 90);
      yAng = map(AcY, minVal, maxVal, -90, 90);
      z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
      if (z > 180) {
        z = z - 360;
      }
      Serial.print(F("Current angle: "));
      //Serial.print("Numerical value:" + z);
      z = z - 90;
      value = round(z);
      value = abs(value);
      
      endt = millis();
    }

    if(value> 180)
    {
      display.setRotation(2); //rotates text on OLED 1=90 degrees, 2=180 degrees
      GlobalUpsideDownFlag = true;
    }
    else
    {
      display.setRotation(0); //rotates text on OLED 1=90 degrees, 2=180 degrees
      GlobalUpsideDownFlag = false;
    }
  }
}
