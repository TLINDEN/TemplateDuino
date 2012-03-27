
#define WEBDUINO_FAIL_MESSAGE "<h1>Request Failed</h1>"
#include "SPI.h" // new include
#include "avr/pgmspace.h" // new include
#include "Ethernet.h"
#include "WebServer.h"


struct DATA_index {
  float temp;
  float humidity;
};


template<class T>
inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

char endl = '\n';

#include "Templates.h"




#include "DHT.h"

#define DHTPIN 8
#define DHTTYPE DHT22

#define VERSION_STRING "0.1"

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
static uint8_t ip[] = { 10, 0, 0, 2 };



int all[] = {31, 33, 35};
int prog = 0;

float temperature;
float humidity;

DHT dht(DHTPIN, DHTTYPE);

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

void getSensorData() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  }
  else {
     temperature = t;
     humidity    = h;
  }
}

void ledprog() {
  switch (prog) {

  case 0:
    for (int i = 0; i<3; i++) {
     digitalWrite( all[i], HIGH);
     delay(100);
     digitalWrite( all[i], LOW);
    }
    break;  
 
  case 1:
    for (int i = 0; i<3; i++) {
     digitalWrite( all[i], HIGH);
    }

    delay(300);

    for (int i = 0; i<3; i++) {
     digitalWrite( all[i], LOW);
    }    
   
    delay(100);
    break;
    
  case 2:
    for (int i=0; i<4; i++) {
       for(int n=0; n<5; n++) {
         digitalWrite(all[i], HIGH);
         delay(100);
         digitalWrite(all[i], LOW);
         delay(100);
       }
       delay(100);
    }
    break;
  }
  delay(300);
}





void tempCmd(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  if (type == WebServer::POST)  {
    server.httpSeeOther("PREFIX");
    return;
  }

  server.httpSuccess();

  if (type == WebServer::GET) {
    getSensorData();
    DATA_index data;
    data.temp = temperature;
    data.humidity = humidity;
    tpl_index(server, data);
  }
}

void setup() {
  /* initialize the Ethernet adapter */
  Ethernet.begin(mac, ip);

  /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&tempCmd);

  /* start the webserver */
  webserver.begin();
  
  for (int i = 0; i<3; i++) {
     pinMode( all[i], OUTPUT);
  }
  
  dht.begin();
  Serial.begin(9600);
  delay(1000);
  digitalWrite(31, HIGH);
  delay(2000);
  digitalWrite(31, LOW);
}

void loop() {
  webserver.processConnection();
     
  Serial.print("prog: ");
  Serial.print(prog);
  Serial.println();

  ledprog();
}

