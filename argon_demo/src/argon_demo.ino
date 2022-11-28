/*
 * Project argon_demo
 * Description:
 * Author:
 * Date:
 */
#include <SparkFunBME280.h>
#include <SFE_MicroOLED.h>

#define OLED_DC A5
#define OLED_CS A3
#define OLED_RESET A4

int photosensor = A0;
int analogvalue;

bool light;
float voltage;

BME280 bme;
MicroOLED oled(OLED_RESET, OLED_DC, OLED_CS);

unsigned long prevBMEms = 0;
const long  BMEInterval = 60000;
unsigned long BMEms;

unsigned long prevlightms = 0;
const long  lightInterval = 60000;
unsigned long lightms;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

  light = false;
  bme.begin();
  oled.begin();

    //oled.pixel(LCDWIDTH/2, LCDHEIGHT/2); // Add a pixel to the display buffer.
  oled.display(); // Draw whatever is in the display buffer.
  delay(3000);
  oled.clear(PAGE);
  oled.clear(ALL);
  oled.setFontType(0);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.


    lightms = millis();
    if(lightms - prevlightms >= lightInterval) {
        //digitalWrite(LED_PIN, HIGH);
        prevlightms = lightms;
        analogvalue = analogRead(photosensor);
        if (analogvalue>100 && light == false) {
        
           Particle.publish("Lights", "ON");
           light = true;
        }
        else if (analogvalue<=100 && light == true){
           Particle.publish("Lights", "OFF");
           light = false;
        }

        Particle.publish("Level", String(analogvalue));
        voltage = analogRead(BATT) * 0.0011224 * 0.89;

        Particle.publish("Batt", String(voltage));
        oled.clear(PAGE);
        oled.setCursor(0, 0);
        oled.print(voltage);
        oled.display();
    }
    
    BMEms = millis();
    if(BMEms - prevBMEms >= BMEInterval) {
        
        prevBMEms = BMEms;
        
        float currentTempC = bme.readTempC();

        String altitude = String(bme.readFloatAltitudeMeters(), 2);
        String tempC = String(bme.readTempC(), 2);
        //String tempF = String(bme.readTempF(), 2);
        String pressure = String(bme.readFloatPressure(), 2);
        String humidity = String(bme.readFloatHumidity(), 2);
        String jsonBMEData = "{\"temp\": "+tempC+", \"hum\": "+humidity+", \"pressure\": "+pressure+", \"altitude\": "+altitude+"}";
        Particle.publish("BMEData", jsonBMEData);
        
    }

}