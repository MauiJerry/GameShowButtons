// NeoDazzleWindow - a subclass of NeoWindow to add rainbow and dazzle efx
/** 
 *  @file NeoRainbowWindow.h
 *  @brief subclass of NeoWindow adding two effects rainbow and dazzle
 *  rainbow effect is taken from the Adafruit_NeoPixel Library's strandtest example
 *    it rolls a rainbow across length of window
 *    type 0 choses next color as single step around color wheel
 *    type 1 evenly distributes the rainbow across window size
 *  dazzle is a random multiSparkle using random colors from color wheel (full intensity)
 */
#include "NeoEffects.h"
#include "NeoWindow.h"

class NeoDazzleWindow : public NeoWindow 
{
  public:
    NeoDazzleWindow(NeoStrip *strip, int startPixel, int len); /*!< constructor passes to parent */

  /**
   * @brief Dazzle Effect: rainbow sparkle effect
   * NeoEffect that sparkles random rainbow of colors across window
   * @param flashTime time sparkles are ON
   * @param tweenTime time sparkles are OFF before next randomw step turned on
   * @param numActive number of sparkles each step; if ==0 then all pixels in window active
   * @param count number of times to sparkle before marking effect DONE
   */
    void setDazzleEfx( int flashTime, int tweenTime, int numActive = 0, int count = 0);

private:
  void rainbowEfxUpdate(void);
  int curColor;
  int rainbowType;

  void dazzleEfxUpdate(void);
  void dazzleEfxSelectPixels();
  boolean dazzleAll;
  uint32_t dazzleColor;
  int dazzleFlashTime;
  int dazzleTweenTime;
  int dazzleMaxCount;
  int dazzleNumActive;
  int dazzleState;

};

