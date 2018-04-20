// July 2017 bounce button test for game show buttons
// use as unit test to show that all buttons work
// uses Bounce and INPUT_PULLUP and Serial to report activity
// LED lights on any button pressed

// bounce2 is new version that depricates the allinone initialization
#include <Bounce2.h>
//#include <Bounce.h>

// This code turns a led on/off through a debounced button
// Build the circuit indicated here: http://arduino.cc/en/Tutorial/Button

// Two sets of buttons - 3 selectors and 2 players
// Two Player buttons
#define Player1Pin 22
#define Player2Pin 23
// for mode selections 3 pushbuttons on Octo pad
#define Mode1Pin 17
#define Mode2Pin 18
#define Mode3Pin 19

#define BUTTON Mode1Pin
#define LED 13

// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce player1Btn = Bounce( Player1Pin,5 ); 
Bounce player2Btn = Bounce( Player2Pin,5 ); 
Bounce mode1Btn = Bounce( Mode1Pin,5 ); 
Bounce mode2Btn = Bounce( Mode2Pin,5 ); 
Bounce mode3Btn = Bounce( Mode3Pin,5 ); 

void setup() {
  Serial.begin(19200);
  pinMode(LED,OUTPUT);

  pinMode(Player1Pin,INPUT_PULLUP);
  pinMode(Player2Pin,INPUT_PULLUP);
  pinMode(Mode1Pin,INPUT_PULLUP);
  pinMode(Mode2Pin,INPUT_PULLUP);
  pinMode(Mode3Pin,INPUT_PULLUP);
}

void loop() {
 // Update the debouncer
  player1Btn.update ( );
  player2Btn.update ( );
  mode1Btn.update ( );
  mode2Btn.update ( );
  mode3Btn.update ( );
 
 // Get the update value
 int player1Value = player1Btn.read();
 int player2Value = player2Btn.read();
 int mode1Value = mode1Btn.read();
 int mode2Value = mode2Btn.read();
 int mode3Value = mode3Btn.read();

Serial.print("Buttons: P1 P2 M1 M2 M3 "); 
  Serial.print(player1Value); Serial.print(" ");
  Serial.print(player2Value); Serial.print(" ");
  Serial.print(mode1Value); Serial.print(" ");
  Serial.print(mode2Value); Serial.print(" ");
  Serial.println(mode3Value);
delay(250);

 int value = player1Value + player2Value + mode1Value + mode2Value + mode3Value;
 // Turn LED on if any button is pressed
 if ( value <1) {
   digitalWrite(LED, HIGH );
 } else {
    digitalWrite(LED, LOW );
 }
}

