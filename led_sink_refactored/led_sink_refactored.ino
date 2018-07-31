#include "FastLED.h"
// Because conditional #includes don't work w/Arduino sketches...
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
const int HEX_SIZE = 6;
#define NUM_LEDS 60
int numChars = NUM_LEDS*HEX_SIZE;
char receivedChars[NUM_LEDS*HEX_SIZE];
char colorArray[NUM_LEDS*HEX_SIZE];
const byte CMD = 0;
const byte BRIGHTNESS = 1;
const byte COLORS = 2;
byte recvState = CMD;

const byte CROSS = 0;
const byte PULSE = 1;
const byte WAITING = 2;
const byte SLIDE = 3;
const byte IDLE = 4;

byte animate = 0;

//  Global Vars
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

// Define the array of leds
CRGB leds[NUM_LEDS];

boolean newData = false;

unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 200; //ms

void setup() {

    Serial.begin(9600);
    FastLED.addLeds<DOTSTAR, DATAPIN, CLOCKPIN, RGB>(leds, NUM_LEDS);

    //strip.begin(); // Initialize pins for output
    //strip.show();  // Turn all LEDs off ASAP

}


void loop() {
    recvWithStartEndMarkers();
    parseNewData();
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // save the current time
        previousMillis = currentMillis;
        outputColor();
    }

}

void outputColor(){
    if (animate == WAITING){
        pushToStrip(colorArray);
        animate = IDLE;
    } else if (animate == SLIDE){

    } else if (animate == IDLE){

    }
}

void pushToStrip(char* pointer){
    ptr = pointer;
    for(uint8_t j = 0; j < NUM_LEDS; j++){
        for(int i = 0; i < 3; i++){
            rgb[i] = charToHex(*ptr) << 4 | charToHex(*(ptr+1));
            ptr += 2;
         }
         //CRGB color = CRGB(rgb[0],rgb[1],rgb[2]);

         leds[j].setRGB(rgb[2],rgb[1],rgb[0]);
    }
    FastLED.show();
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
        switch(recvState){
            case CMD:
                if (receivedChars[0] == '0'){
                    recvState++;
                    animate = WAITING;


                } else if (receivedChars[0] == '1') {
                    // Handle other cmds
                    if (receivedChars[1] == '0'){
                        animate = CROSS;
                    } else if (receivedChars[1] == '1'){
                        animate = PULSE;
                    } else if (receivedChars[1] == '2') {
                        animate = SLIDE;
                    }
                }
                break;
            case BRIGHTNESS:
                b = charToHex(*ptr) << 4 | charToHex(*(ptr+1));

                FastLED.setBrightness(b);
                recvState++;
                break;
            case COLORS:
                copy(receivedChars, colorArray, NUM_LEDS*HEX_SIZE);
                recvState = CMD;
                break;
        }

        newData = false;
    }
}
void copy(char *arrayOriginal, char *arrayCopy, int arraySize){
  while(arraySize--) *arrayCopy++ = *arrayOriginal++;
}

uint8_t charToHex(unsigned char value)
{
  if (value >= '0' && value <= '9') return value - '0';
  else if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  else if (value >= 'a' && value <= 'f') return value - 'a' + 10;
}
