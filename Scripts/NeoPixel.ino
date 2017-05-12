//Neopixel main script
#include <Adafruit_NeoPixel.h>

#define PIN 6 //strip pin
int analogPin=0; //A0 pin for reading frequency
int strobePin=2; //Ask MSGEQ7 for data
int resetPin=3; //End communication with MSGEQ7
int spectrumValue[7]; //Array for frequencies
int filter=80;
int highest;
int rotation=0;
int rotationdelay=0;

int pattern = 0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600); //Debugging serial port
  /**
  pinMode setup for MSGEQ7
  **/
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  strip.begin();
  strip.show();
}

void loop() {
  switch(pattern){
    case 0:
    colorLoopProgression();
    break;
    case 1:
      //placeholder
      break;
  }
}

  void colorLoopProgression(){
    digitalWrite(resetPin, HIGH);
    digitalWrite(resetPin, LOW);
    for (int i=0;i<7;i++){
      digitalWrite(strobePin, LOW);
      delay(10);
      spectrumValue[i]=analogRead(analogPin);
      spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
      spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
      Serial.print(spectrumValue[i]);
      Serial.print(" ");
      digitalWrite(strobePin, HIGH);
    }
    Serial.println();

    highest =0;
    for(int i=0; i<7; i++){
      if(spectrumValue[i]>spectrumValue[highest]){
        highest = i;
      }
    }
    Serial.println(highest);
    for(int i=30; i<60; i++){
      if(i<30+(spectrumValue[highest]/9)){
        int highestt=0;
        if(highestt==0){
          strip.setPixelColor(i, Wheel(rotation+(spectrumValue[highest]/6)+(highest*8)));
        }
        else if(highest==1){
          strip.setPixelColor(i, strip.Color(spectrumValue[highest], (255-spectrumValue[highest])/2, 0));
        }
        else if(highest==2){
          strip.setPixelColor(i, strip.Color(255-spectrumValue[highest], spectrumValue[highest], 0));
        }
        else if(highest==3){
          strip.setPixelColor(i, strip.Color(0, spectrumValue[highest], 0));
        }
        else if(highest==4){
          strip.setPixelColor(i, strip.Color(0, 0, spectrumValue[highest]));
        }
        else if(highest==5){
          strip.setPixelColor(i, strip.Color(75, 0, 130));
        }
        else if(highest==6){
          strip.setPixelColor(i, strip.Color(100,100,100));
        }

      }
      else{
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }
    for(int i=30; i>0; i--){
      if(i>30-(spectrumValue[highest]/9)){
        int highestt=0;
        if(highestt==0){
          strip.setPixelColor(i, Wheel(rotation+(spectrumValue[highest]/6)+(highest*8)));
        }
        else if(highest==1){
          strip.setPixelColor(i, strip.Color(255, 127, 0));
        }
        else if(highest==2){
          strip.setPixelColor(i, strip.Color(255, 255, 0));
        }
        else if(highest==3){
          strip.setPixelColor(i, strip.Color(0, 255, 0));
        }
        else if(highest==4){
          strip.setPixelColor(i, strip.Color(0, 0, 255));
        }
        else if(highest==5){
          strip.setPixelColor(i, strip.Color(75, 0, 130));
        }
        else if(highest==6){
          strip.setPixelColor(i, strip.Color(100,100,100));
        }
      }
      else{
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }
    if(rotationdelay>=1){
      rotation++;
      rotationdelay=0;
    }
    rotationdelay++;
    strip.show();
  }
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
