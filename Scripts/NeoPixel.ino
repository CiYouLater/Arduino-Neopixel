//Neopixel main script
#include <Adafruit_NeoPixel.h>

#define PIN 6 //strip pin
int analogPin=0; //A0 pin for reading frequency
int strobePin=2; //Ask MSGEQ7 for data
int resetPin=3; //End communication with MSGEQ7
//int spectrumValue[7]; //Array for frequencies
int filter=80;
int highest;
int rotation=0;
int rotationdelay=0;

int pattern = 1;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

//Define Specific colors
uint32_t red = strip.Color(255, 0, 0);
uint32_t orange = strip.Color(255, 127, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t purple = strip.Color(75, 0, 130);

void setup() {
  //Serial.begin(9600); //Debugging serial port
  /**
  pinMode setup for MSGEQ7
  **/
  strip.begin();
  strip.show();
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}

void loop() {
  if(pattern ==0){
      colorLoopProgression();
  }else if(pattern == 1){
      RainbowEqualizer();
  }
}

void RainbowEqualizer(){
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  int spectrumValue[6];
  for (int i = 0; i < 6; i++)
  {
   digitalWrite(strobePin, LOW);
   delayMicroseconds(30); // to allow the output to settle
   spectrumValue[i] = map(analogRead(analogPin), 0, 1023, 0, 9);
   setPixel(i*10, i*10+spectrumValue[i]);
   setReverse(i*10, i*10+spectrumValue[i]);
   strip.show();
   digitalWrite(strobePin, HIGH);
  }
  delay(30);
}

  void colorLoopProgression(){
    int spectrumValue[7];
    digitalWrite(resetPin, HIGH);
    digitalWrite(resetPin, LOW);
    for (int i=0;i<7;i++){
      digitalWrite(strobePin, LOW);
      delay(10);
      spectrumValue[i]=analogRead(analogPin);
      spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
      spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
      //Serial.print(spectrumValue[i]);
      //Serial.print(" ");
      digitalWrite(strobePin, HIGH);
    }
    //Serial.println();

    highest =0;
    for(int i=0; i<7; i++){
      if(spectrumValue[i]>spectrumValue[highest]){
        highest = i;
      }
    }
   // Serial.println(highest);
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

  void setPixel(int start_bit, int pixelValue){
    for(int j=start_bit; j<pixelValue; j++){
      strip.setPixelColor(j, colorChoose(start_bit));
    }
  }


  void setReverse(int start_bit, int pixelValue){
    for(int k=start_bit+9; k>pixelValue;k--){
      strip.setPixelColor(k, 0,0,0);
    }
  }

  uint32_t colorChoose(int start_bit){
    switch (start_bit){
      case 0:
        return red;
        break;
      case 10:
        return orange;
        break;
      case 20:
        return yellow;
        break;
      case 30:
        return green;
        break;
      case 40:
        return blue;
        break;
      case 50:
        return purple;
        break;
    }
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
