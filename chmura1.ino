/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 December, 2016
 * Copyright 2016 Jakub Borowka
 */
// najpierw sprawdzac czy jest zmiana jasnosci a potem kolory

#include <IRremote.h>
#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<6> ledStrip;
// Create a buffer for holding the colors (3 bytes per color). example: [255,255,25
#define LED_COUNT 6 // numbers of leds + 1
rgb_color colors[LED_COUNT];
rgb_color color;
//recive pin for infrared reciver
int RECV_PIN = A5;
IRrecv irrecv(RECV_PIN);
decode_results results;

//start brightness
byte brightness = 153;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("lol");
  
}

rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}

void rainbow(byte brightness, boolean allTheSame=false){ 
  uint16_t time = millis() >> 5;
  for(uint16_t i = 0; i < LED_COUNT; i++){
    byte x = (time >> 2);
    if (!allTheSame){
      x -= (i << 4);
    }
    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, brightness);
  }
  ledStrip.write(colors, LED_COUNT);
  delay(10);
}

void setColor(uint16_t h,uint8_t s, uint8_t v){
  for(uint16_t i = 0; i < LED_COUNT; i++){ 
    colors[i] = hsvToRgb(h, s, v);
  }
  ledStrip.write(colors,LED_COUNT);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    if (results.value == 16736925){
      Serial.println("up2");
      if (brightness != 255){
        // maxymalnie jest 254
        brightness += 51;
      }
    }
    else if (results.value == 16754775){
      if (brightness != 0){
      Serial.println("down2");
      brightness -= 51;
      }
    }
    // jak up lub down to wykonywac dla poprzedniego kodu
    
    switch (results.value){ // to na dec
      case 16738455:
        Serial.println(brightness);
        setColor(300,255,brightness);
        Serial.println("czerwony");
        break;
      case 16750695:
        setColor(300,255,brightness);
        Serial.println("zielony");
        break;
      case 16756815:
        Serial.println("niebieski");
        break;
      case 16724175:
        Serial.println("pomaranczowy");
        setColor(20,255,brightness);
        break;
      case 16718055:
        Serial.println("zolty");
        break;
      case 16743045:
        Serial.println("biały");
        break;
      case 16712445:
        Serial.println("rainbow");
        while (true){
          rainbow(brightness); // jak true to takei same kolory
        }
        // nasłuchiwac i while jakis klawisz to breakowac
        break;
      case 16730805:
        Serial.println("wylacz");
        break;
      default: // FFFFFFF albo inny niz zakodowane
        Serial.println("niema");
        break;
      } 
    irrecv.resume(); // Receive the next value
    }
  delay(100); // chyba mozna usunac
}
