/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and clears it.
 
 A good test for this is to try it with a GPS receiver that sends out
 NMEA 0183 sentences.
 
 NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
 other ATmega32U4 based boards.
 
 created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 https://www.arduino.cc/en/Tutorial/BuiltInExamples/SerialEvent
 */
boolean newCommand = false;
boolean gripperStatus = false;
int LEDPIN = 6;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //digitalWrite(LEDPIN, HIGH);

  if(newCommand == true){
    if(gripperStatus==false){
      digitalWrite(LEDPIN, LOW);
    }
    if(gripperStatus==true){
      digitalWrite(LEDPIN, HIGH);
    } 
    newCommand = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
 routine is run between each time loop() runs, so using delay inside loop can
 delay response. Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    //Serial.print(inChar);
    switch (inChar) {
    case 'a':
      gripperStatus=true;
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case 'b':
      gripperStatus=false;
      digitalWrite(LED_BUILTIN, LOW);
      break;
    default:
      digitalWrite(LED_BUILTIN, HIGH);
      break;

    }
    newCommand=true;
  }

}


