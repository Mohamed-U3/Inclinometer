//  =============================
//    Checks for button presses
//  =============================
/*void checkButtonDown() {
  //    Check button Down Button)
  //  ==================
  //downchange = false;
  buttonDown_state = digitalRead(buttonDown);
  // compare the buttonState to its previous state
  if (buttonDown_state != buttonDown_lastState) {
    if (buttonDown_state == LOW) {  // if the state has changed, increment the counter
      //menuPageCounter++; // Now handled in checkPage()
      if (millis() > debounceDelay + debounceTimer){
        downchange = true;
        lastInteractionTime = millis();
      }
      delay(125);  // Delay a little bit to avoid bouncing
    }
    buttonDown_lastState = buttonDown_state;  // save the current state as the last state
    debounceTimer = millis();
  }
}*/

bool checkButtonCentre()
{
  //    Check button Centre Button)
  //  ==================
  //centreChange = false;
  buttonCentre_state = digitalRead(buttonCentre);
  // compare the buttonState to its previous state
  if (buttonCentre_state != buttonCentre_lastState)
  {
    if (buttonCentre_state == HIGH)
    {  // if the state has changed, increment the counter
      if (millis() > debounceDelay + debounceTimer)
      {
        centreChange = true;
        lastInteractionTime = millis();
      }
      //delay(125);  // Delay a little bit to avoid bouncing
    }
    buttonCentre_lastState = buttonCentre_state;  // save the current state as the last state
    debounceTimer = millis();
  }
}
/*
void checkButtonUp() {
  //    Check button Up Button)
  //  ==================
  //upchange = false;
  buttonUp_state = digitalRead(buttonUp);
  // compare the buttonState to its previous state
  if (buttonUp_state != buttonUp_lastState) {
    if (buttonUp_state == LOW) {  // if the state has changed, increment the counter
      if (millis() > debounceDelay + debounceTimer){
        upchange = true;
        lastInteractionTime = millis();
      }
      //menuPageCounter--; // Now handled in checkPage()
      //delay(125);  // Delay a little bit to avoid bouncing
    }
    buttonUp_lastState = buttonUp_state;  // save the current state as the last state
    debounceTimer = millis();
  }
}*/

void checkButtonLeft()
{
  //    Check button Left Button)
  //  ==================
  //leftchange = false;
  if(GlobalUpsideDownFlag == true)
  {
    buttonLeft_state = digitalRead(buttonRight);
  }
  else
  {
    buttonLeft_state = digitalRead(buttonLeft);
  }
  // compare the buttonState to its previous state
  if (buttonLeft_state != buttonLeft_lastState)
  {
    if (buttonLeft_state == LOW)
    {  // if the state has changed, increment the counter
      if (millis() > debounceDelay + debounceTimer)
      {
        leftchange = true;
        lastInteractionTime = millis();
      }
      //delay(125);  // Delay a little bit to avoid bouncing
    }
    buttonLeft_lastState = buttonLeft_state;  // save the current state as the last state
    debounceTimer = millis();
  }
}

void checkButtonRight()
{
  //    Check button Right Button)
  //  ==================
  //rightchange = false;
  if(GlobalUpsideDownFlag == true)
  {
    buttonRight_state = digitalRead(buttonLeft);
  }
  else
  {
    buttonRight_state = digitalRead(buttonRight);
  }
  // compare the buttonState to its previous state
  if (buttonRight_state != buttonRight_lastState)
  {
    if (buttonRight_state == LOW)
    {  // if the state has changed, increment the counter
      if (millis() > debounceDelay + debounceTimer)
      {
        rightchange = true;
        lastInteractionTime = millis();
      }
      //delay(125);  // Delay a little bit to avoid bouncing
    }
    buttonRight_lastState = buttonRight_state;  // save the current state as the last state
    debounceTimer = millis();
  }
}
