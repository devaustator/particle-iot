/*
 * Project boron-gps-tracker
 * Description:
 * Author:
 * Date:
 */
#include <TinyGPS++.h>

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

Serial1.begin(GPSBaud);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  Particle.publish("Lat", String(gps.location.lat()));
  Particle.publish("Lng", String(gps.location.lng()));
  // The core of your code will likely live here.
smartDelay(30000);
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
 //   while (ss.available())
    while (Serial1.available())
  //    gps.encode(ss.read());
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

String gpsTime(TinyGPSTime &t)
{
    String val = "XX:XX:XX";
  
    if (t.isValid())
    {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        val = String(sz);
    }
  
  smartDelay(0);
  return val;
}

String gpsDate(TinyGPSDate &d)
{
    String val = "XX.XX.XXXX";
  
  if (d.isValid())
  {
    char sz[32];
    sprintf(sz, "%02d.%02d.%02d ", d.day(), d.month(), d.year());
    val = String(sz);
  }
  
  smartDelay(0);
  return val;
}

String publishDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (d.isValid() && t.isValid())
  {
    char sd[32];
    sprintf(sd, "%02d-%02d-%02d", d.year(), d.month(), d.day());
    
    char st[32];
    sprintf(st, "%02d:%02d:%02d.%02d", t.hour(), t.minute(), t.second(), t.centisecond());
    
    String val = String(sd) + "T" + String(st) +"0Z";
    
    return val;
  } else {
      return "0000-00-00T00:00:00.000Z";
  }
  
  smartDelay(0);
}
