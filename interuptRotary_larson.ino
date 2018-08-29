#include <Adafruit_NeoPixel.h>

// SETUP YOUR OUTPUT PIN AND NUMBER OF PIXELS
#define LEDPIN 5
#define NUM_PIXELS  60
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
///////////
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int switchpin = 4; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 1; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 1; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
int minencoderVal = 0; //lowest allowable
int maxencoderVal = 8; //max value

void setup() {
  strip.begin();
  clearStrip(); // Initialize all pixels to 'off'
  delay(1000);

    pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)

  Serial.begin(115200); // start the serial monitor link
  
}

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void loop() {
    if(oldEncPos != encoderPos) {
      if (encoderPos == (maxencoderVal +1))
      { encoderPos = minencoderVal;
        }
      if (encoderPos == 255)// byte has nonzero so rolls over to 255.
        { encoderPos = maxencoderVal;
      }
    oldEncPos = encoderPos;
  }
      Serial.println(encoderPos);
/*  knightRider(3, 50, 2, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, 50, 2, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, 50, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, 50, 2, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, 50, 2, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, 50, 2, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, 50, 2, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(3, 50, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
  delay(2000);

  // Iterate through a whole rainbow of colors
  //read rotary encoder to set colour
  
  for(byte j=0; j<252; j+=7) {
    knightRider(1, 50, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
 */
 switch (encoderPos) {
    case 0:
      clearStrip(); // Initialize all pixels to 'off'
      break;
    case 1:
      knightRider(1, 40, 2, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
      break;
    case 2:
      knightRider(1, 40, 2, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
      break;
    case 3:
      knightRider(1, 40, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
      break;
    case 4:
     knightRider(1, 40, 2, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
      break;
    case 5:
      knightRider(1, 40, 2, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
      break;
    case 6:
       knightRider(1, 40, 2, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
      break;
    case 7:
      knightRider(1, 40, 2, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
      break;
    case 8:
       knightRider(1, 40, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
      break;
    default:
      // if nothing else matches, do the default
      clearStrip(); // Initialize all pixels to 'off'
      break;
  } 
  //clearStrip();
  ;
}

// Cycles - one cycle is scanning through all pixels left then right (or right then left)
// Speed - how fast one cycle is (32 with 16 pixels is default KnightRider speed)
// Width - how wide the trail effect is on the fading out LEDs.  The original display used
//         light bulbs, so they have a persistance when turning off.  This creates a trail.
//         Effective range is 2 - 8, 4 is default for 16 pixels.  Play with this.
// Color - 32-bit packed RGB color value.  All pixels will be this color.
// knightRider(cycles, speed, width, color);
void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color) {
  uint32_t old_val[NUM_PIXELS]; // up to 256 lights!
  // Larson time baby!
  for(int i = 0; i < cycles; i++){
    for (int count = 1; count<NUM_PIXELS; count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      if(oldEncPos != encoderPos) {clearStrip(); break;}
      for(int x = count; x>0; x--) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x-1, old_val[x-1]); 
      if(oldEncPos != encoderPos) {clearStrip(); break;}
      }
      if(oldEncPos != encoderPos) {clearStrip(); break;}
      strip.show();
      delay(speed);
    }
    for (int count = NUM_PIXELS-1; count>=0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      if(oldEncPos != encoderPos) {clearStrip(); break;}
      for(int x = count; x<=NUM_PIXELS ;x++) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        if(oldEncPos != encoderPos) {break;}
        strip.setPixelColor(x+1, old_val[x+1]);
        if(oldEncPos != encoderPos) {clearStrip(); break;}
      }
      if(oldEncPos != encoderPos) {clearStrip(); break;}
      strip.show();
      delay(speed);
    }
  }
}

void clearStrip() {
  for( int i = 0; i<NUM_PIXELS; i++){
    strip.setPixelColor(i, 0x000000); strip.show();
  }
}

uint32_t dimColor(uint32_t color, uint8_t width) {
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}

// Using a counter and for() loop, input a value 0 to 251 to get a color value.
// The colors transition like: red - org - ylw - grn - cyn - blue - vio - mag - back to red.
// Entering 255 will give you white, if you need it.
uint32_t colorWheel(byte WheelPos) {
  byte state = WheelPos / 21;
  switch(state) {
    case 0: return strip.Color(255, 0, 255 - ((((WheelPos % 21) + 1) * 6) + 127)); break;
    case 1: return strip.Color(255, ((WheelPos % 21) + 1) * 6, 0); break;
    case 2: return strip.Color(255, (((WheelPos % 21) + 1) * 6) + 127, 0); break;
    case 3: return strip.Color(255 - (((WheelPos % 21) + 1) * 6), 255, 0); break;
    case 4: return strip.Color(255 - (((WheelPos % 21) + 1) * 6) + 127, 255, 0); break;
    case 5: return strip.Color(0, 255, ((WheelPos % 21) + 1) * 6); break;
    case 6: return strip.Color(0, 255, (((WheelPos % 21) + 1) * 6) + 127); break;
    case 7: return strip.Color(0, 255 - (((WheelPos % 21) + 1) * 6), 255); break;
    case 8: return strip.Color(0, 255 - ((((WheelPos % 21) + 1) * 6) + 127), 255); break;
    case 9: return strip.Color(((WheelPos % 21) + 1) * 6, 0, 255); break;
    case 10: return strip.Color((((WheelPos % 21) + 1) * 6) + 127, 0, 255); break;
    case 11: return strip.Color(255, 0, 255 - (((WheelPos % 21) + 1) * 6)); break;
    default: return strip.Color(0, 0, 0); break;
  }
}
