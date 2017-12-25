#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <avr/io.h>
#define PIN 6

uint8_t wheelArray[15]={85,78,71,64,57,50,43,36,29,22,15,7,0,0,0};
uint32_t colorSet[15];
byte dimFactor=4;
uint8_t whiteVal=100;
uint32_t whiteTriplet;
uint8_t initDelay=50;
signed long loopVal=0;
signed long loopDelay=250000;
uint8_t actRev=0;
uint8_t prevActRev=0;
uint8_t revSim[60]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,12,8,9,10,11,12,13,14,15,12,8,9,10,11,12,13,14,15,11,8,9,10,11,12,13,14,15,12,8,9,10,11,12,13,14,15,12,8,9,10,11,12,13,14,15};
bool rising=true;
signed long blinkTime=0;
signed long blinkVal=45000;
const byte interruptPin = 2;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, PIN, NEO_GRB + NEO_KHZ800);

void setup() 

{
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    //  #if defined (__AVR_ATtiny85__)
    //    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    //  #endif
    // End of trinket special code

    //generate the colorSet, uses more memory, but less processing power
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ignitionTiming, RISING);
    
  for(uint8_t i=0;i<15;i++)
  {
      colorSet[i]=dimColor(Wheel(wheelArray[i]));
  }

    whiteTriplet=strip.Color(whiteVal,whiteVal,whiteVal);
  
    strip.Color(whiteVal,whiteVal,whiteVal);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

initScale();
    
}

void loop()
{
  uint8_t n =0;
  
  while(n<61){
  
    if((micros()-loopVal)>loopDelay){
      
      actRev=revSim[n];
      
      n++;
      loopVal=micros();

      }

    if(actRev>prevActRev)
    {
       
      if(prevActRev==11)
      {
        
        //in case the reset timer has not expired when the transition is made and resetShiftIndicator is not invoked, we reset it here
        strip.setPixelColor(14,0);
        strip.setPixelColor(13,0);
        strip.setPixelColor(12,0);
         
      }
      strip.setPixelColor(prevActRev,colorSet[prevActRev]);
      strip.show();
      prevActRev++;
      rising=true;

    }
    else if(actRev<prevActRev)
    {
      
      strip.setPixelColor(prevActRev,0);
      strip.show();
      prevActRev--;
      rising=false;

    }
if(actRev==11&&rising){
  
    if(((micros()-blinkTime)<blinkVal))
    {

      setShiftIndicator();
    
    }
    
      else if((micros()-blinkTime)<2*blinkVal)    
    {
    
      resetShiftIndicator();
                     
    }
    
      else 
    {
    
      blinkTime=micros();
         
    }
}    
  
}
}


///useful functions
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t dimColor(uint32_t color)
    {
        uint32_t dimColor = strip.Color(Red(color) >> dimFactor, Green(color) >> dimFactor, Blue(color) >> dimFactor);
        return dimColor;
    }

        // Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }
 
    // Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }
 
    // Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }


void initScale(){

  strip.setPixelColor(0,whiteTriplet);
  
  for(uint8_t i=1;i<15;i++)
  {
    strip.setPixelColor(i,whiteTriplet);
    strip.setPixelColor(i-1,colorSet[i-1]);
    strip.show();
    delay(initDelay);
  }

    for(int8_t i=14;i>-1;i--)
  {
    strip.setPixelColor(i,whiteTriplet);
    strip.setPixelColor(i+1,0);
    strip.show();
    delay(initDelay);
  }
//
   strip.setPixelColor(0,0);
   strip.show();
  
}

void setShiftIndicator()
{

  for(uint8_t k=11;k<15;k++){

  strip.setPixelColor(k,whiteTriplet);
  
  }
  strip.show();
}

void resetShiftIndicator()
{

  for(uint8_t k=11;k<15;k++){

  strip.setPixelColor(k,0);
  }
  strip.show();
}

void ignitionTiming() {
  
}

