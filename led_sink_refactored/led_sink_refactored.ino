
#include "Adafruit_DotStar.h"
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
const int HEX_SIZE = 6;
const int NUM_LEDS = 60;
int numChars = NUM_LEDS*HEX_SIZE;
char receivedChars[NUM_LEDS*HEX_SIZE];
const byte CMD = 0;
const byte BRIGHTNESS = 1;
const byte COLORS = 2;
byte state = CMD;

//  Vars
char* ptr;
uint8_t b;
uint8_t rgb[3];
int currBlock = 0;

    static boolean recvInProgress = false;
    static int ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;




#define DATAPIN    4
#define CLOCKPIN   5
#define offset     1

Adafruit_DotStar strip = Adafruit_DotStar(
  NUM_LEDS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

boolean newData = false;

void setup() {
    Serial.begin(9600);
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
        clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
    #endif

    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP
}

void loop() {
    recvWithStartEndMarkers();
    parseNewData();
}

void recvWithStartEndMarkers() {


    

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
              
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    
}

void parseNewData() {
 
    if (newData == true) {
        ptr = receivedChars;
        switch(state){
            case CMD:
                if (receivedChars[0] == '0'){
                    state++;
                  

                } else {
                    // Handle other cmdss
                }
                break;
            case BRIGHTNESS:
                b = charToHex(*ptr) << 4 | charToHex(*(ptr+1));

                strip.setBrightness(b);
                state++;
                break;
            case COLORS:
                for(uint8_t j = 0; j < NUM_LEDS; j++){
                    for(int i = 0; i < 3; i++){
                        rgb[i] = charToHex(*ptr) << 4 | charToHex(*(ptr+1));
                        ptr += 2;
                     }
                    
                     strip.setPixelColor(j,rgb[1],rgb[0],rgb[2]);
                }
                strip.show();
                state = CMD;
                break;
        }
        
        newData = false;
    }
}

uint8_t charToHex(unsigned char value)
{
  if (value >= '0' && value <= '9') return value - '0';
  else if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  else if (value >= 'a' && value <= 'f') return value - 'a' + 10;
}
