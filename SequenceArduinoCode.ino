int sensorValue;      //variables used for sensor calibration
int sensorLow = 1023;
int sensorHigh = 0;

const int CalPin = 13;  //the built in LED on pin 13 is used to indicate sensor calibration

const int buttonPin = 2;  //buttion and led pin numbers are set here
const int ledPin1 =  3;
const int ledPin2 =  4;
const int ledPin3 =  5;
const int ledPin4 =  6;

int ledState1 = LOW; // led and button states are initialised here
int ledState2 = LOW;  
int ledState3 = LOW;  
int ledState4 = LOW;  
int buttonState;            
int lastButtonState = LOW;

unsigned long previousMillis = 0;   //long data types used for blinking leds without delay and button debouncing     
long interval = 500;           
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

int pos = 1;  //variables for sequencer position, pitch, and play state initialised here
int pitch = 0;
bool play = false;

void setup() {
  pinMode(CalPin, OUTPUT);    //turn the built in LED on to indicate calibration
  digitalWrite(CalPin, HIGH);

  // calibrate for the first five seconds after program runs
  while (millis() < 5000) {
    // record the maximum sensor value
    sensorValue = analogRead(A0);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
     //record the minimum sensor value
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }
   // turn the built in LED off, signaling the end of the calibration period
  digitalWrite(CalPin, LOW);

  //assign the led pins as outputs and the button as an input
  pinMode(buttonPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
}

void loop() {
  //code implementing button debouncing is placed first
  unsigned long currentMillis = millis();
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the state of the play boolean if the button is high
      if (buttonState == HIGH) {
        play = !play;
      }
    }
  }

  sensorValue = analogRead(A0); //read the input from the ldr and assign it to the interval time
  interval = map(sensorValue, sensorLow, sensorHigh, 100, 1000);  //mapped between 100 to 1000 milliseconds
  
  if (play == true){  //only blink through the sequence if the play bool = true
    if (currentMillis - previousMillis >= interval) { //play through the sequence according to the interval variable
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      //use the pos variable to determine the position within the sequence,
      //and turn LEDS on and off and adjust pitch accordingly
      if (pos == 1) { 
        ledState1 = HIGH;
        ledState2 = LOW;
        ledState3 = LOW;
        ledState4 = LOW;
        pitch = 440;
        pos = 2;
      } else if (pos == 2) {
        ledState1 = LOW;
        ledState2 = HIGH;
        ledState3 = LOW;
        ledState4 = LOW;
        pitch = 523;
        pos = 3;
      } else if (pos == 3) {
        ledState1 = LOW;
        ledState2 = LOW;
        ledState3 = HIGH;
        ledState4 = LOW;
        pitch = 784;
        pos = 4;
      } else if (pos == 4) {
        ledState1 = LOW;
        ledState2 = LOW;
        ledState3 = LOW;
        ledState4 = HIGH;
        pitch = 988;
        pos = 1;
      }
    }
  } else {  // if play = false, turn off all leds, turn off the pitch and reset to the starting position of the sequence
      ledState1 = LOW;
      ledState2 = LOW;
      ledState3 = LOW;
      ledState4 = LOW;
      pitch = 0;
      pos = 1;
      }
  
  digitalWrite(ledPin1, ledState1); //output the state of the leds and the pitch value as determined previously
  digitalWrite(ledPin2, ledState2);
  digitalWrite(ledPin3, ledState3);
  digitalWrite(ledPin4, ledState4);
  tone(8, pitch, interval);
  lastButtonState = reading;
}
