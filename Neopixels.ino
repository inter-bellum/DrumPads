#include <Adafruit_NeoPixel.h>
#define LED_PIN    2
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(30, LED_PIN, NEO_GRB + NEO_KHZ800);


//how bright are all the pixels
int pixelBright = 255;

//how fast to dim the pixels
int pixelDimSpeed = 30;

//default color of the pixels
int color[3] = {255, 120, 50};

//how long should it take to dim the pixels
int pixelTimeOut = 100;

LPFilter pixelBrightFilter;

void initPixels() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


  for (int i = 0; i < strip.numPixels(); i ++) {
    strip.setPixelColor(i, strip.Color(100, 255, 10));
    strip.show();
    delay(50);
  }
  delay(100);
  for (int i = 0; i < strip.numPixels(); i ++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();

}

void updatePixels() {
  pixelBright = pixelBrightFilter.update(pixelBright, 0.8);
  
  if (pixelBright > 0) {
    pixelBright -= pixelDimSpeed;
  }
  pixelBright = constrain(pixelBright, 0, 255);

  for (int i = 0; i < strip.numPixels(); i ++) {
    strip.setPixelColor(i, strip.Color(color[0], color[1], color[2]));
  }
  strip.setBrightness(pixelBright); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();



  Serial.println(pixelBright);



}

void pixelHit() {
  pixelBright = 255;





}
