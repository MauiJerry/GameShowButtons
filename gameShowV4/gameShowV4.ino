/*
//   3rd prototype GameShowFantatics rig
// update for testing wired up prototype hardware
// with comments for next round.
// ToDo: neoDazzle needs work, need to test player button logic
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
 
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>
#include <NeoEffects.h>
#include "NeoDazzleWindow.h"

// Two sets of buttons - 3 selectors and 2 players
// Two Player buttons
#define Player1Pin 22
#define Player2Pin 23

// for mode selections 3 pushbuttons on Octo pad
#define Mode1Pin 17
#define Mode2Pin 18
#define Mode3Pin 19

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

// Neopixel rings
#define LARGE_NEORING_SIZE 16

// Pins for strips connected via OctoWS2811
// these are assigned to odd ordering of pins for reasons internal to Octo library
// although we are not using the library, we may use the Octo interface board 
// which provides access to these pins via 2 RJ45 connectors.

// 2 sides and 2 tops
// and one that is full length
#define SIDE_LENGTH 60
#define TOP_LENGTH 20

const int LEFT_SIDE_START = 0;
const int LEFT_TOP_START = (LEFT_SIDE_START + SIDE_LENGTH);
const int RIGHT_TOP_START = (LEFT_TOP_START + TOP_LENGTH);
const int RIGHT_SIDE_START = (RIGHT_TOP_START + TOP_LENGTH);
const int total_strip_len = (2*TOP_LENGTH)+(2*SIDE_LENGTH);

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

NeoStrip strip1 = NeoStrip(total_strip_len, STRIP_PIN, NEO_GRB + NEO_KHZ800);
NeoWindow leftSide = NeoWindow(&strip1, LEFT_SIDE_START, SIDE_LENGTH);
NeoWindow leftTop = NeoWindow(&strip1, LEFT_TOP_START, TOP_LENGTH);
NeoWindow leftSideTop = NeoWindow(&strip1, LEFT_SIDE_START, SIDE_LENGTH+TOP_LENGTH);

NeoWindow rightSide = NeoWindow(&strip1, RIGHT_SIDE_START, SIDE_LENGTH);
NeoWindow rightTop = NeoWindow(&strip1, RIGHT_TOP_START, TOP_LENGTH);
NeoWindow rightSideTop = NeoWindow(&strip1, RIGHT_TOP_START, TOP_LENGTH+SIDE_LENGTH);

NeoDazzleWindow fullStrip = NeoDazzleWindow(&strip1, 0, total_strip_len);

NeoStrip neoRing1 = NeoStrip(LARGE_NEORING_SIZE, RING_1_PIN, NEO_GRB + NEO_KHZ800);
NeoStrip neoRing2 = NeoStrip(LARGE_NEORING_SIZE, RING_2_PIN, NEO_GRB + NEO_KHZ800);
NeoDazzleWindow ring1 = NeoDazzleWindow(&neoRing1, 0, LARGE_NEORING_SIZE);
NeoDazzleWindow ring2 = NeoDazzleWindow(&neoRing2, 0, LARGE_NEORING_SIZE);

// array of neoWindows
const int numWindows = 8;
NeoWindow *windows[] = {&ring1, &ring2, &leftSide, &leftTop, &leftSideTop, &rightSide, &rightTop, &rightSideTop };


// Mode Flora NeoPixels are just simple neopixels as we just do on/off in color
Adafruit_NeoPixel modePixels = Adafruit_NeoPixel(3, MODE_PIXELS, NEO_GRB + NEO_KHZ800);

//////// Mode
#define GAMESHOW_MODE_QUIET   0
#define GAMESHOW_MODE_GAME    1
#define GAMESHOW_MODE_DAZZLE  2
#define GAMESHOW_MODE_GAME_TEAM1  3
#define GAMESHOW_MODE_GAME_TEAM2  3

int gameShowMode = GAMESHOW_MODE_QUIET;

///////// Buttons 
Bounce player1Btn = Bounce(Player1Pin,DEBOUNCETIME );
Bounce player2Btn = Bounce(Player2Pin,DEBOUNCETIME);

Bounce mode1Btn = Bounce(Mode1Pin,DEBOUNCETIME);
Bounce mode2Btn = Bounce(Mode2Pin,DEBOUNCETIME);
Bounce mode3Btn = Bounce(Mode3Pin,DEBOUNCETIME);


void setupDBounced() {
    Serial.println("Setup DBounce buttons");
   /*
   // Setup the button pin with internal pullup
   pinMode( inputPin ,INPUT_PULLUP);
   */
  pinMode( Player1Pin, INPUT_PULLUP);  
  pinMode( Player2Pin, INPUT_PULLUP);
  pinMode( Mode1Pin, INPUT_PULLUP);
  pinMode( Mode2Pin, INPUT_PULLUP);
  pinMode( Mode3Pin, INPUT_PULLUP);
}

void setModeQuiet() {
    Serial.println("Set Mode Quiet");
  // Neo efx do slow rainbow
  ring1.setRainbowEfx(500,0);
  ring2.setRainbowEfx(500,0);
  fullStrip.setRainbowEfx(500,0);
  leftSide.setNoEfx();
  leftTop.setNoEfx();
  leftSideTop.setNoEfx();
  rightSide.setNoEfx();
  rightTop.setNoEfx();
  rightSideTop.setNoEfx();

  // full red, lowered green, blues
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(255,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,100,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,100));
  modePixels.show();

  strip1.show();
  neoRing1.show();
  neoRing2.show();

  digitalWrite(Player1LedPin, HIGH);
  digitalWrite(Player2LedPin, HIGH);

  gameShowMode = GAMESHOW_MODE_QUIET;
}

void setModeGame() {
    Serial.println("Set Mode GAME");
  // Set Control Buttons
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(100,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,255,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,100));
  modePixels.show();

  // Rings and strip are dark
  ring1.setNoEfx();
  ring2.setNoEfx();
  fullStrip.setNoEfx();
  leftSide.setNoEfx();
  leftTop.setNoEfx();
  leftSideTop.setNoEfx();
  rightSide.setNoEfx();
  rightTop.setNoEfx();
  rightSideTop.setNoEfx();
  
  strip1.clearStrip();
  neoRing1.clearStrip();
  neoRing2.clearStrip();
 
  strip1.show();
  neoRing1.show();
  neoRing2.show();

  // Player LEDs ON
  digitalWrite(Player1LedPin, HIGH);
  digitalWrite(Player2LedPin, HIGH);

  gameShowMode = GAMESHOW_MODE_GAME;
}

void setModeDazzle() {
    Serial.println("Set Mode Dazzle");
  // Set Mode Buttons
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(100,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,100,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,255));
  modePixels.show();

  // Rings and strip are dark
  ring1.setNoEfx();
  ring2.setNoEfx();
  fullStrip.setNoEfx();
  leftSide.setNoEfx();
  leftTop.setNoEfx();
  leftSideTop.setNoEfx();
  rightSide.setNoEfx();
  rightTop.setNoEfx();
  rightSideTop.setNoEfx();
  
  strip1.clearStrip();
  neoRing1.clearStrip();
  neoRing2.clearStrip();
 
  strip1.show();
  neoRing1.show();
  neoRing2.show();

  // Player LEDs Off
  digitalWrite(Player1LedPin, LOW);
  digitalWrite(Player2LedPin, LOW);

  gameShowMode = GAMESHOW_MODE_DAZZLE;
}

void updateButtons()
{
//    Serial.println("Update Buttons");

  player1Btn.update();
  player2Btn.update();
  mode1Btn.update();
  mode2Btn.update();
  mode3Btn.update();
}

void setupNeoPix() {
    // start the strip.  do this first for all strips
      Serial.println("Setup Neo Pixels");
//Also the straight LEDs

  strip1.begin();
  neoRing1.begin();
  neoRing2.begin();
  modePixels.begin();

  blinkLEDS();
}

void blinkLEDS () {
// NeoPixels can be very bright, and at full power can use lots of power
// longer 'strips' require extra power to run full bright. 
// brightness runs 0-255 and scales all colors to match that dark->bright
// strip1.setBrightness(100);

 // first we Blink the whole strip to show app is running
  strip1.clearStrip();
  strip1.show();
  neoRing1.clearStrip();
  neoRing1.show();
  neoRing2.clearStrip();
  neoRing2.show();
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(0,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,0,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,0));
  modePixels.show();
  // Player LEDs Off
  digitalWrite(Player1LedPin, LOW);
  digitalWrite(Player2LedPin, LOW);
  delay(1000);    
  
  strip1.fillStrip(Adafruit_NeoPixel::Color(255,0,0));
  strip1.show();
  
  neoRing1.fillStrip(Adafruit_NeoPixel::Color(0,255,0));
  neoRing1.show();
  
  neoRing2.fillStrip(Adafruit_NeoPixel::Color(0,0,255));
  neoRing2.show();
  
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(255,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,255,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,255));
  modePixels.show();
  digitalWrite(Player1LedPin, HIGH);
  digitalWrite(Player2LedPin, HIGH);
  delay(10000);

  strip1.clearStrip();
  strip1.show();
  neoRing1.clearStrip();
  neoRing1.show();
  neoRing2.clearStrip();
  neoRing2.show();
  modePixels.setPixelColor(QUIET_PIXEL, modePixels.Color(0,0,0));
  modePixels.setPixelColor(GAME_PIXEL, modePixels.Color(0,0,0));
  modePixels.setPixelColor(DAZZLE_PIXEL, modePixels.Color(0,0,0));
  modePixels.show();
  // Player LEDs Off
  digitalWrite(Player1LedPin, LOW);
  digitalWrite(Player2LedPin, LOW);
  delay(1000);
}

void doTeam1Efx()
{
  Serial.println("Do Team 2 Efx");
  // light up Team1 side with Marquee ripple
  // play Team1 sound
}

void doTeam2Efx()
{
    Serial.println("Do Team 2 Efx");

  // light up Team2 side with Marquee ripple
  // play Team2 sound  
}

////////////////////////////////////
void setup() {
  Serial.begin(115200);
  delay(500); // wait half sec to open serial monitor window
  Serial.println("Starting gameShow test Two - buttons + leds");

  // raw LED Pins
  pinMode(Player1LedPin, OUTPUT);
  pinMode(Player2LedPin, OUTPUT);
    
  setupDBounced();
  setupNeoPix();
  
  setModeQuiet();

  Serial.println("Setup Done");
}

void loop() {

//    blinkLEDS();

  // Player LEDs Off
//  Serial.println("HIGH");
//  digitalWrite(Player1LedPin, HIGH);
//  digitalWrite(Player2LedPin, HIGH);
//delay(1000);
//  Serial.println("LOW");
//    // Player LEDs Off
//  digitalWrite(Player1LedPin, LOW);
//  digitalWrite(Player2LedPin, LOW);
//delay(1000);

// game logic
  updateButtons();
  // Check Control Buttons
  if (mode1Btn.fell()) {
    setModeQuiet();
  } else if (mode2Btn.fell()) {
    setModeGame();
  } else if (mode3Btn.fell()) {
    setModeDazzle();
  }

  if (  gameShowMode == GAMESHOW_MODE_GAME) {
    // we are in game mode.
    // which ever player hits button first we transition to their win
    // GAMESHOW_MODE_GAME_TEAM1 GAMESHOW_MODE_GAME_TEAM2
    if (player1Btn.fell())
      doTeam1Efx();
    else if (player2Btn.fell())
      doTeam2Efx();
    //
  } else if (gameShowMode == GAMESHOW_MODE_GAME_TEAM1) {
    // team1 hit first; do their effect
    // if effect over, reset to game mode
  } else if (gameShowMode == GAMESHOW_MODE_GAME_TEAM2) {
    // team2 hit first; do their effect
    
  }
  
  // update neoStrips...
  // now update each Window, using the array
  for (int i=0; i< numWindows;i++)
  {
      windows[i]->updateWindow();
  }
  strip1.show();
  neoRing1.show();
  neoRing2.show();


  Serial.println(millis());
  Serial.print("Player1: "); Serial.println(player1Btn.read());
  Serial.print("Player2: "); Serial.println(player2Btn.read());
  delay(500);
}

