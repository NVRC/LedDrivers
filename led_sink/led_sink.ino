


#include "Adafruit_DotStar.h"
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

const int NUM_LEDS = 60;
const int HEX_SIZE = 6;
const int CMD_LENGTH = 1;
const int BLOCK_SIZE = 10;


char set[NUM_LEDS*HEX_SIZE];

union ArrayToInteger {
    byte array[4];
    uint32_t integer;
};

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
#define offset     1
Adafruit_DotStar strip = Adafruit_DotStar(
  NUM_LEDS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup(){
    Serial.begin(9600);  //Started the serial communication at 9600 baudrate
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
        clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
    #endif

    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP

}


uint32_t color = 0xFF0000;      // 0xFF0000 'On' color (starts red)
unsigned char charArray[HEX_SIZE*BLOCK_SIZE];
uint8_t rgb[3];
// Testing
// 0
// 00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF00

void loop(){
    if (Serial.available() == 1){  //Looking for incoming data
        char anim_flag;
        size_t bytes_read = Serial.readBytes(&anim_flag, CMD_LENGTH);
        Serial.println("Checking flag");

        if(anim_flag == '0'){
            while(Serial.available() < 2){
                //    BLOCK
            }
            char brightness[2];
            bytes_read = Serial.readBytes(brightness,2);
            char* temp = brightness;
            uint8_t b = charToHex(*temp) << 4 | charToHex(*(temp+1));
            Serial.println(b,DEC);
            strip.setBrightness(b);
            for(int k = 0; k < NUM_LEDS/BLOCK_SIZE; k++){
                //  64 bytes in the stream 60 int div 6 = 10
                while(Serial.available() < BLOCK_SIZE*HEX_SIZE){
                  //  BLOCK
                }
                if(Serial.available() >= BLOCK_SIZE*HEX_SIZE){
                    bytes_read = Serial.readBytes(charArray,HEX_SIZE*BLOCK_SIZE);
                    char* ptr = charArray;
                    for(uint8_t j = 0; j < BLOCK_SIZE; j++){
                          for(int i = 0; i < 3; i++){
                              rgb[i] = charToHex(*ptr) << 4 | charToHex(*(ptr+1));
                              ptr += 2;
                          }

                          strip.setPixelColor(j+10*k,rgb[1],rgb[0],rgb[2]);
                      }

                 }
             }
             strip.show();
        } else if(anim_flag == '1'){

        }

    } else {

    }
}

uint8_t charToHex(unsigned char value)
{
  if (value >= '0' && value <= '9') return value - '0';
  else if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  else if (value >= 'a' && value <= 'f') return value - 'a' + 10;

}
