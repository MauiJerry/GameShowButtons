
/*
 * control buttons and leds only
//   test rig for GameShowFantatics
// Test buttons + Led, not pixel strip
// Flashes all led 2x, blink in sequence 2x, Blink all again 1x
// then enter btn test loop:
//     if button is pressed, report it and turn on its led while button is down.
//   btn down or toggle?
// ToDo: 
//
//  three mode push buttons (quiet, game, dazzle)
//  Two buttons for participants
//  2 NeoPixel 16LED ring (2 neo Windows
//  1 Neopixel strip (4 NeoWindows)
//     left top, left side
//     right top, right side
//   intended for front side top/down
//     to indicate winning side
//  
//  PJRC OctoWS2811 board provides
//  2 RJ45 connectors, each with 4 lines providing 5v level shifting
//    RJ1 - 2, 14, 7, 8
//    RJ2 - 6, 20, 21, 5
//  and a set of 7 solder points for pins:
//    0, 1, 17, 18, 19, 22, 23
//  skip 0, 1 as those are rx/tx and use would conflict with usb comms
//   
//  For ease of wiring, use these for all but Audio connection (on A14)
//  
//  2 rings and 2 lights in buttons on JR1
//  1 string and 3 mode lights on JR2
//  5 buttons go on 5 pins (17-19, 22,23)
//  
 */
 
#include <Adafruit_NeoPixel.h>
#include <NeoEffects.h>
#include <NeoStrip.h>
#include <NeoWindow.h>
#include "NeoDazzleWindow.h"

////////////////////
// Button Stuff
// bounce2 is new version that depricates the allinone initialization
#include <Bounce2.h>

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
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce player1Btn = Bounce( Player1Pin,5 ); 
Bounce player2Btn = Bounce( Player2Pin,5 ); 
Bounce mode1Btn = Bounce( Mode1Pin,5 ); 
Bounce mode2Btn = Bounce( Mode2Pin,5 ); 
Bounce mode3Btn = Bounce( Mode3Pin,5 ); 

// Button Status; updated during updateButtons
int player1Value = 0;
int player2Value = 0;
int mode1Value = 0;
int mode2Value = 0;
int mode3Value = 0;
 
void setupButtons() {
  pinMode(Player1Pin,INPUT_PULLUP);
  pinMode(Player2Pin,INPUT_PULLUP);
  pinMode(Mode1Pin,INPUT_PULLUP);
  pinMode(Mode2Pin,INPUT_PULLUP);
  pinMode(Mode3Pin,INPUT_PULLUP);
  Serial.println("Buttons Setup Complete");
}

// updateButtons() returns 0 if no change in buttons, 1+ otherwise
int updateButtons() {
  int change = 0;
  change = player1Btn.update ( );
  change += player2Btn.update ( );
  change += mode1Btn.update ( );
  change += mode2Btn.update ( );
  change += mode3Btn.update ( );
  // Get the update value
  player1Value = player1Btn.read();
  player2Value = player2Btn.read();
  mode1Value = mode1Btn.read();
  mode2Value = mode2Btn.read();
  mode3Value = mode3Btn.read();
 
 return change; // 0 none pressed, 1+ something changed
}

void logButtons() {
  Serial.print("Buttons: P1 P2 M1 M2 M3 "); 
  Serial.print(player1Value); Serial.print(" ");
  Serial.print(player2Value); Serial.print(" ");
  Serial.print(mode1Value); Serial.print(" ");
  Serial.print(mode2Value); Serial.print(" ");
  Serial.println(mode3Value);
}

//////////////////

// single white leds in the buttons on RJ1
#define Player1LedPin 14
#define Player2LedPin 8

// NeoPixel Strips
// player rings and single LED on RJ1 (with their single leds)
// strip and 3 model LEDs on RJ2 (with 3 mode leds)
#define RING_1_PIN 2
#define RING_2_PIN 7

// Vertical/top cross strip is on RJ2
#define STRIP_PIN 5

// mode buttons lit by chained Flora NeoPixels on RJ2
#define MODE_PIXELS 6
// and buttons in order are
#define QUIET_PIXEL 0
#define GAME_PIXEL 1
#define DAZZLE_PIXEL 2

// time to allow switches to settle for debouncing
#define DEBOUNCETIME 5

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

//NeoStrip strip1 = NeoStrip(total_strip_len, STRIP_PIN, NEO_GRB + NEO_KHZ800);
//NeoDazzleWindow fullStrip = NeoDazzleWindow(&strip1, 0, total_strip_len);

// Mode Flora NeoPixels are just simple neopixels as we just do on/off in color
Adafruit_NeoPixel modePixels = Adafruit_NeoPixel(4, MODE_PIXELS, NEO_GRB + NEO_KHZ800);

void setupNeoPix() {
  // start the strip.  do this first for all strips
  Serial.println("Setup Neo Pixels");
  NeoWindow::updateTime();
  modePixels.begin();

}

void modePixOn()
{
    Serial.println("Mode On");
  //modePixels.setPixelColor(DEAD_PIXEL,  modePixels.Color(100,100,100));
  modePixels.setPixelColor(QUIET_PIXEL,  modePixels.Color(255,0,0));
  modePixels.setPixelColor(GAME_PIXEL,   modePixels.Color(0,255,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,255));
  modePixels.show();
}

void modePixOff()
{
  Serial.println("Mode Off");
  //modePixels.setPixelColor(DEAD_PIXEL,  0);
  modePixels.setPixelColor(QUIET_PIXEL,  0);
  modePixels.setPixelColor(GAME_PIXEL,   0);
  modePixels.setPixelColor(DAZZLE_PIXEL, 0);
  modePixels.show();
}


void allOff() {
  Serial.println("Turn all LED OFF");
  modePixOff();
}

////////////////////////////////////
void setup() {
  Serial.begin(115200);
  delay(500); // wait half sec to open serial monitor window
  Serial.println("Starting gameShow led test one in sequence");

  setupButtons();
  
  // raw LED Pins
//  pinMode(Player1LedPin, OUTPUT);
  pinMode(Player2LedPin, OUTPUT);
    
  setupNeoPix();

  //startupBlinks();
  startupModeBlinks();
  allOff();
}

void  startupModeBlinks()
{
  Serial.println("Startup Mode Blinks Begin");
  for (int i =0; i< 10; i++)
  {
    modePixOn();
    delay(1000);
    
    modePixOff();
    delay(1000);
  }
  Serial.println("Startup Mode Blinks Done");
}

int color =0;
int direction = 1;
void loop() {
 modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(color,0,0));
 modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,color,0));
 modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,color));
 modePixels.show();
 delay(500);
 if (direction) {
  color++;
  if (color >255) {
    Serial.println("Change to go Down");
    direction = 0; 
  }
 } else {
  color --;
  if (color < 0) {
    Serial.println("Change to go UP");
    direction = 1;
  }
 }
}


