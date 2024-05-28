void mainMenu()
{
  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  menuNum = menuPageCounter;
  interrupts();

  switch (menuNum)
  {
    case 0:  // Menu Page 1, LED control
      checkButtonLeft();
      checkButtonRight();
      drawHeader(F("LED Brightness"));

      display.setCursor(5, 20);     //  Sets coursor position
      display.println(brightness);  // set info

      //// Check if button (Left) Pressed ////
      // compare the buttonState to its previous state
      if (leftchange)
      { //(digitalRead(buttonLeft) == 0) {  // if the state has changed, increment the counter
        brightness = brightness - interval;
        leftchange = false;
      }
      //analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
      //EEPROM.write(0, brightness);                                     //write counter to address 0
      //buttonLeft_lastState = buttonLeft_state;                         // save the current state as the last state

      //// Check if button (Up) Pressed ////
      if (rightchange)
      { //(digitalRead(buttonRight) == 0) {  // if the state has changed, increment the counter
        brightness = brightness + interval;
        rightchange = false;
      }
      //write counter to address 0
      //buttonRight_lastState = buttonRight_state;                       // save the current state as the last state

      if (brightness > 20)
      {
        brightness = 20;
      }
      if (brightness <= 0)
      {
        brightness = 0;
      }
      analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
      EEPROM.write(0, brightness);

      display.display();  // update display
      //checkPage();
      //}
      break;

    case 1:  // Menu Page 2, mpu initialisation
      drawHeader(F("Target Angle"));
      display.setCursor(5, 15);                 //  Sets coursor position
      display.print(F("Press button to"));
      display.setCursor(5, 23);  //  Sets coursor position
      display.print(F("start"));
      display.display();
      checkButtonLeft();
      checkButtonRight();
      if (leftchange || rightchange)
      {
        leftchange = false;
        rightchange = false;
        // Try to initialize!
        delay(125);
        // MPU 6050 code
        drawHeader(F("Target Angle"));         // set info
        display.setCursor(5, 20);
        if (!mpu.begin())
        {
          display.println(F("Module Error"));  // set info
          display.display();
        }
        else
        {
          display.print(F("Module Active!"));  // set info
          display.display();
          delay(1000);

          looping = true;
          while (looping)
          {
            flashcount = 0;
            //flash the screen twice
            while (flashcount < 2)
            {
              flashLED();
              flashcount++;
            }

            //reset the LED brightness back to original setting
            analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
            drawHeader(F("Target Angle"));
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
              if (z > 180) {
                z = z - 360;
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

              drawHeader(F("Target Angle"));
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

    case 2:  // Menu Page 4, battery %
      percentage = FuelGauge.percent();  //update percentage variable
      drawHeader(F("Battery percentage:"));
      display.setCursor(5, 20);                 //  Sets coursor position
      display.print(percentage);                // set info
      display.setCursor(65, 20);                //  Sets coursor position
      display.println("%");                     // set info
      display.display();                        // update display
      delay(150);
      break;

  }
}

void checkPage()
{
  //Button Checking
  //checkButtonDown();
  //checkButtonUp();
  checkButtonCentre();
  checkButtonLeft();
  checkButtonRight();

  if (centreChange == true)
  { // check for centre button press
    if (menuPageCounter == 0)
    { // check if we are on the mode select screen
      centreChange = false;
      leftchange = false;
      rightchange = false;
      menuPageCounter = 1;
      flag = 1;
    }
    else
    {
      // Copied from down button
      if (menuPageCounter != 0)
      {
        menuPageCounter++;
        centreChange = false;
        leftchange = false;
        rightchange = false;
        if (menuPageCounter >= 3)
        { // if page number goes over 5
          menuPageCounter = 0;       // reset to 0
        }
        if (menuPageCounter == 2 && mode)
        {
          centreChange = false;
          leftchange = false;
          rightchange = false;
          menuPageCounter = 2;
        }
        // end of copied from down
        flag = 1;
      }
    }
  }
  else if (leftchange || rightchange)
  {
    //Serial.println("Left or right pressed");
    flag = 1;
  }
}

void checkM()
{
  if (!mpu.begin())
  {
    display.println(F("Module Error"));  // set info
    display.display();
  }
  else
  {
    //    flashcount = 0;
    //    //flash the screen twice
    //    while (flashcount < 2)
    //    {
    //      flashLED();
    //      flashcount++;
    //    }
    //
    //    //reset the LED brightness back to original setting
    //    analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
    //
    //    //flashScreen();
    //    flashLED();  //flash the LED
    //
    //    //reset brightness of LED
    //    analogWrite(ledPin, map(brightness, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
    //    EEPROM.write(0, brightness);                                     //write counter to address 0
    //
    //    delay(125);
    int val = MPU_Angle();
    drawHeader(F("Real Time Angle"));
    display.setCursor(5, 20);                       //  Sets coursor position
    display.print(F("Target Angle: "));
    display.print(String(val));  // set info
    display.display();
    //    delay(2000);
  }
}

int MPU_Angle()
{
  int value = 0;
  if (!mpu.begin())
  {
    display.println(F("Module Error"));  // set info
    display.display();
  }
  else
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    xAng = map(AcX, minVal, maxVal, -90, 90);
    yAng = map(AcY, minVal, maxVal, -90, 90);
    z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
    z = z + 90;
    if (z > 360) z = z - 360;
    //    Serial.print(F("Current angle: "));
    //    Serial.println(z);
    value = round(z);
    value = abs(value);
    //    Serial.println(value);

    if (value > 180)
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

  return value;
}
