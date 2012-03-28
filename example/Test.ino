#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

#include <EEPROM.h>
#include "EEany.h"

#define WEBDUINO_FAIL_MESSAGE "<h1>Request Failed</h1>"
#include "SPI.h" // new include
#include "avr/pgmspace.h" // new include
#include "Ethernet.h"
#include "WebServer.h"


#define PREFIX ""
WebServer webserver(PREFIX, 80);

struct DATA_index {
  int date;
};

char endl = '\n';

// no-cost stream operator as described at
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#include "Templates.h"





int maxchannels = 8;
int maxprograms = 32;
int dbversion   = 1000;

int tastled = 40;
int taster = 7;
int relais = 37;

int status = 0;
int prev   = 0;

Database db;

void setup() { 
  pinMode(tastled, OUTPUT);
  pinMode(relais, OUTPUT);
  pinMode(taster, INPUT);
  Wire.begin();
  RTC.begin();

  Serial.begin(9600);
  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  db = ee_getdb();
}

void prstatus(int old, int cur) {
  if(old != cur) {
    if(cur) {
     digitalWrite(tastled, HIGH);
     Serial.println("Taste: EIN");
     digitalWrite(relais, LOW);
     Serial.println("  Relais ON ");
    }
    else {
     digitalWrite(tastled, LOW);
     Serial.println("Taste: AUS");
     digitalWrite(relais, HIGH);
     Serial.println("  Relais OFF");
    }
    prev = status;
  }
}

bool chan_by_prog_on(DateTime now, int chan) {
  /*
   * check if the current time is between programmed
   * turn-on-time. Mode Manual.
   *
   * FIXME: add checking for 0 values
   */
  int nowmin   = ((int) now.hour() * 60) + (int) now.minute();
  int startmin = (db.programs[chan].start_hour * 60) + db.programs[chan].start_min;
  int stopmin  = (db.programs[chan].stop_hour * 60)  + (db.programs[chan].stop_min ? db.programs[chan].stop_min - 1 : 0);
  if(startmin <= nowmin && stopmin >= nowmin) {
    return true;
  }
  else {
    return false;
  }
}

void printdate(DateTime now) {
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void checkprog() {
 DateTime now = RTC.now();
 
 printdate(now);
 
 if(chan_by_prog_on(now, 0)) {
   digitalWrite(relais, HIGH);
   Serial.println("  Relais ON ");
 }
 else {
   digitalWrite(relais, LOW);
   Serial.println("  Relais OFF");
 }
}

void loop() {
  checkprog();
  delay(1000);
}
