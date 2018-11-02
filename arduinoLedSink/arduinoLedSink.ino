
#include "Adafruit_DotStar.h"
#include <RingBuf.h>

// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>
const int HEX_SIZE = 6;
const int NUM_LEDS = 60;
int numChars = NUM_LEDS*HEX_SIZE-1;
char receivedChars[NUM_LEDS*HEX_SIZE];

//  LED nested structs
struct colorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct led {
    uint8_t index;
    struct colorRGB color;
};
//  LED pointer array for indexed accessing
//  Allocated array of pointers where each element points to a 'struct led'
struct led *ledPtrs[NUM_LEDS];
/*
    Ring Buffer dynamic memory (RAM) demands
        ** assuming unint8_t = 1 word = 1 byte = 8 bits = 2^8 = 256 states

    Therefore,
        colorRGB: 3 bytes
        led:
            1 bytes +
            3 bytes
            -------
            4 bytes

        leds:
            led * NUM_LEDS
            4 * 60 = 240 bytes

        struct pointer array:
            int * NUM_LEDS
            2 * 60 = 120 bytes

        total:
            leds + pointers
            240 + 120 = 360 bytes
*/



//  Init Properly sized ring buffer
RingBuf *ledBuffer = RingBuf_new(sizeof(struct led), NUM_LEDS);


//  State constants
const byte CMD = 0;
const byte BRIGHTNESS = 1;
const byte COLORS = 2;
const byte ANIM = 3;
const byte RATE = 5;
const byte TWOD = 4;
const byte CYCLE = 6;


byte state = CMD;

//  Vars
char* ptr;
uint8_t b;
uint8_t rgb[3];
long rate = 0;
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
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 200; // ms


struct led *(*ledRef)[NUM_LEDS] = &ledPtrs;
void setup() {
    Serial.begin(9600);
    for (uint8_t i = 0; i < NUM_LEDS; i++){
        //  allocate and assign pointers to NUM_LEDS structs
        ledPtrs[i] = malloc(sizeof(struct led));
    }
    //  declare pointer to ledPtrs array
    //  enables the use of (*ledRef)[n]->data to reference the n-th elem
    

    strip.begin(); // Initialize pins for output
}

void loop() {


    recvWithStartEndMarkers();
    parseNewData();
    currentMillis = millis();
    if (rate > 0){
        if (currentMillis - previousMillis >= rate) {
            // save the current time
            previousMillis = currentMillis;

            // Output next color
        }
    }

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

void pushToStrip(){
    //  forEach elem in circular buffer
    for(uint8_t i = 0; i<NUM_LEDS; i++){
        strip.setPixelColor(i,(*ledRef)[i]->color.g,
            (*ledRef)[i]->color.r,
            (*ledRef)[i]->color.b);
    }
    strip.show();


}

void parseNewData() {
    if (newData == true) {
        ptr = receivedChars;
        switch(state){
            case CMD:
                // Static Display
                if (receivedChars[0] == '0'){
                    state = BRIGHTNESS;
                // 2D circular cycling animation
                } else if (receivedChars[0] == '1'){
                    state = RATE;

                }else {
                    // Handle other cmds
                }
                break;
            case RATE:
                rate = (int)atoi(ptr);
                state = BRIGHTNESS;
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
                     (*ledRef)[j]->index = j;
                     (*ledRef)[j]->color.r = rgb[0];
                     (*ledRef)[j]->color.g = rgb[1];
                     (*ledRef)[j]->color.b = rgb[2];
                     ledBuffer->add(ledBuffer, &(*ledRef)[j]);
                }
                pushToStrip();
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
