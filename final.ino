/* This program was intially made to be in a car, but was never implemented
 *  cause my dad didn't want them permanently in his car.
 *  The animation files aren't mine, the header files were coded by a fellow 
 *  LED enthusiasts, Mark Kriegsman and Mary Corey March. The reactivity is all mine, 
 *  and the FFT implementationwas changed from my implementation a library as the library 
 *  was much faster. This code is very much patch work of already written code.
 */

//Includes
#include <arduinoFFT.h>
#include "FastLED.h"


//Choose Gradient Pallette, got this one from a website.
DEFINE_GRADIENT_PALETTE( Schindlers_List_gp ) {
    0,   1,  3,  1,
   51,   1,  3,  1,
   51,   8, 13,  9,
  102,   8, 13,  9,
  102,  49, 63, 48,
  153,  49, 63, 48,
  153, 175,186,158,
  204, 175,186,158,
  204, 150,  0,  0,
  255, 150,  0,  0};



//FFT Definitions and Instantiations
#define NUM_SAMPLES 8
#define SAMPLING_FREQUENCY 800
double im[NUM_SAMPLES], data[NUM_SAMPLES];
unsigned int sampling_period_us;
unsigned long microseconds;
int colorPicker = 0;
int prevColor = 1;
int brightness = 0;
int prevBrightness = 1;
int counter = 0;
arduinoFFT FFT = arduinoFFT();

//LED Strip Definitions and Instantiations
#define SEAT_LEDS 100

//counters
int i;

CRGB SeatType[SEAT_LEDS];
CRGBPalette16 currentPalette = RainbowColors_p;                                   
CRGBPalette16 targetPalette = RainbowColors_p;                                    
uint8_t gBrightness = 128;


//Animation Includes
#include "ripple.h"
#define maxRipples 12                                                            
typedef struct ripple Ripple;
Ripple ripples[maxRipples];

//Pacifica
#include "Pacifica.H"

void setup() { 

  Serial.begin(9600);

  //Allow flow of electrons
  delay(1000);

  //Type of Strip SetUp
  FastLED.addLeds<NEOPIXEL, 4>(SeatType, SEAT_LEDS);
  
  /* For other sections in car.
  FastLED.addLeds<NEOPIXEL,2>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL,10>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL,11>(leds, NUM_LEDS);
  */

  //Calculate Sample Period
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));

  
  
  

}

void loop() {
  fill_strip_color();
  //updatePacifica();
  //updateRipple();
  //updateLEDS();
  FastLED.show();
}

void updateLEDS(){
  FastLED[0].showLeds(gBrightness);
}

void performFFT() {
  //Record Samples to Preform FFT On
  for (i = 0; i < NUM_SAMPLES; i++) {                         //take 128 samples
      microseconds = micros();
      data[i] = analogRead(0);                                  //get audio from Analog 0
      im[i] = 0;
      while(micros() < (microseconds + sampling_period_us)){
          }
    };

   FFT.Compute(data, im, NUM_SAMPLES, FFT_FORWARD);                           //perform the FFT on data
   FFT.ComplexToMagnitude(data, im, NUM_SAMPLES); 
}
/*
Ripple createRipples(){                                                     //Create Ripples 
  
  
  int* ptr = (int*) malloc(MAX_RIPPLES, sizeof(ripple));
  for (i = 0; i < MAX_RIPPLES; i ++){
    ptr[i] = Ripple; 
  }
  return ptr;

  
}*/

/*NOT MY FUNCTION!!!*/
void SetupMySimilar4Palette() {                                                   // Create a palette with colours close to each other.

  uint8_t thishue = random8();
  
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                                CHSV(thishue+random8(32), 255, random8(192,255)),
                                CHSV(thishue+random8(32), 192, random8(192,255)),
                                CHSV(thishue+random8(32), 255, random8(128,255)));
                                
} // SetupMySimilar4Palette()

  void updateRipple(){

    EVERY_N_MILLISECONDS(50) {                                                      // Smooth palette transitioning runs continuously.
        uint8_t maxChanges = 24;
          nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
      }

      EVERY_N_SECONDS(5) {
        SetupMySimilar4Palette();                                                     // Change palette colours every 5 seconds.
      }

      performFFT();
      int dat = sqrt(data[2] * data[2] + im[2] * im[2]);

      //Animation Creator's function
      for (int i = 0; i < maxRipples; i += 2) {                                       // Check to see if ripple has expired, and if so, create a new one.
        if (random8() > 20 && !ripples[i].exist && dat > 9) {                                   // Randomly create a new ripple if this one has expired.
          ripples[i].Init(1000, 60);                                                   // Initialize the ripple array with Fade and MaxLife values.
          ripples[i + 1] = ripples[i];                                                // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
          ripples[i + 1].velocity *= -1;                                              // Make sure other half go other direction.
          break;
        }
    }

      EVERY_N_MILLIS(50) {                                                            // Sets the original delay time.

      //Animation Creator's function
      for (int i = 0; i < maxRipples; i++) {                                          // Move the ripple if it exists
        if (ripples[i].exist) {
          SeatType[ripples[i].pos] = ColorFromPalette(currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
          ripples[i].Move();
            }
      }

      fadeToBlackBy(SeatType, SEAT_LEDS, 160);
      }

          FastLED[0].showLeds(gBrightness);                                                                 // Power managed display of LED's.

  }


void fill_strip_gradient(){
  colorPicker = analogRead(A1);
  int check = colorPicker - prevColor;


  //Smoothing for color display, only updates in magnitude differences of 5.
  if (check > 5 || check < -5){
    counter ++;
    if (counter > 5){
    prevColor = colorPicker;
    
    colorPicker = int(map(analogRead(A1), 0, 1023, 0, SEAT_LEDS));
    for (i = 0; i < SEAT_LEDS; i++){
    SeatType[i] = ColorFromPalette(currentPalette, int((255*((i+colorPicker)%SEAT_LEDS))/SEAT_LEDS), 100, LINEARBLEND);
    counter = 0 ;
    }
    }
  }
}

void fill_strip_color(){
  colorPicker = analogRead(A1);
  int check = colorPicker - prevColor;
  int checkb = brightness - prevBrightness;
  brightness = analogRead(A2);

  //Not an extremely effecient way of updating, but gets the job done.
  if ((check > 5 || check < -5) || (checkb > 5 || checkb < -5)){
    Serial.println(colorPicker);
    prevColor = colorPicker;
    prevBrightness = brightness;
    for (i = 0; i < SEAT_LEDS; i++){
      SeatType[i] = ColorFromPalette(currentPalette, int(map(colorPicker, 0, 1023, 0, 252)), int(map(brightness, 0, 1023, 0, 255)), LINEARBLEND);
    }
  }
}
