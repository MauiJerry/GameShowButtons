// NeoDazzleWindow - a subclass of NeoWindow to add dazzle efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example
// type 0 = window shows myPixelCount portion of wheel each update, thru all 255 colors
// type 1 = evenly distributes wheel across N pixels in window (full rainbow in window)

#include "NeoDazzleWindow.h"

NeoDazzleWindow::NeoDazzleWindow(NeoStrip *strip, int startPixel, int len)
  : NeoWindow(strip, startPixel, len)
{
  
}

// couple flags for the state
static const int dazzleFLASH = 1;
static const int dazzleTWEEN = 0;

void NeoDazzleWindow::setDazzleEfx( int flashTime, int tweenTime, int numActive, int count)
{
  setNoEfx(); // reset values
    
  effectDelay = flashTime;
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoDazzleWindow::dazzleEfxUpdate;

  dazzleFlashTime = flashTime;
  dazzleTweenTime = tweenTime;
  dazzleMaxCount = count;
  dazzleNumActive = numActive;
  dazzleState = dazzleFLASH;
  
  if (numActive <= 0 || numActive > myPixelCount)
    numActive = myPixelCount;
  dazzleNumActive = numActive;
  
  clearActive();
  fillBlack(); // clear it

  // Select N pixels, make them Active and set them to the color
  dazzleEfxSelectPixels();

  myStrip->setStripChanged(); // mark the strip changed

}

void NeoDazzleWindow::dazzleEfxUpdate(void)
{
  
  if (dazzleState == dazzleFLASH) {
//    Serial.println("  clear and set to TWEEN");
    // it is on, turn off and set to dazzleTWEEN
    for (int idx = myStartPixel; idx <= myEndPixel;idx++) {
      if (myStrip->isPixelActive(idx))
        myStrip->setPixelColor(idx, 0);
    }
    clearActive();
    dazzleState = dazzleTWEEN;
    effectDelay = dazzleTweenTime;
  } else {
    // it is in TWEEN, so turn to FLASH: select new pixels
    dazzleEfxSelectPixels();
    
    dazzleState = dazzleFLASH;
    effectDelay = dazzleFlashTime;
  }

  effectCount++;
  if (dazzleMaxCount > 0 && effectCount > dazzleMaxCount)
  {
    efxDone = true;
  }

}

void NeoDazzleWindow::dazzleEfxSelectPixels()
{
  
    if (dazzleNumActive == 0 || dazzleNumActive == myPixelCount) {
        // do all pixels in window; skip random choice so it runs fast
        for (int i=myStartPixel; i<= myEndPixel; i++) {
            myStrip->setPixelActive(i);
            myStrip->setPixelColor(i,NeoStrip::randomWheelColor());
        }
    } else {
        for (int i = 0; i < dazzleNumActive; i++){
          int idx = random(myStartPixel,myEndPixel);
          while (myStrip->isPixelActive(idx))
              // if it is already active, pick another; note this could take a while to fill large number
              idx = random(myStartPixel,myEndPixel);
    //      Serial.print("Set Pixel Active: "); Serial.print(idx);Serial.println();
          myStrip->setPixelActive(idx);
          myStrip->setPixelColor(idx,NeoStrip::randomWheelColor());
        }
    }
}



