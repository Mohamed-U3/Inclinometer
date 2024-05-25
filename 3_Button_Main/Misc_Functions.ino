//////////////////////
//    Functions     //
//////////////////////
void flashLED(void) {
  flash = 0;
  analogWrite(ledPin, map(flash, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
  EEPROM.write(0, brightness);                                //write counter to address 0
  delay(500);

  flash = 20;
  analogWrite(ledPin, map(flash, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
  EEPROM.write(0, brightness);                                //write counter to address 0
  delay(2000);

  flash = 0;
  analogWrite(ledPin, map(flash, 0, maxBrightness, 0, 255));  //this code maps the max brightness constant to the max LED brightness
  EEPROM.write(0, brightness);                                //write counter to address 0
}

void autoPowerOff() {
  if (!digitalRead(buttonCentre)) {
    powerButtonHoldTimer = millis();
  } else if (millis() > (powerOffHoldTime * 1000) + powerButtonHoldTimer) {
    drawHeader("Powering Down");
    display.display();
    delay(2000);
    digitalWrite(powerPin, 0);
  }
  if (autoPowerOffTime > 0) {
    if (millis() > (autoPowerOffTime * 1000) + lastInteractionTime) {
      drawHeader("Auto Power Down");
      display.display();
      delay(2000);
      digitalWrite(powerPin, 0);
    }
  }
}