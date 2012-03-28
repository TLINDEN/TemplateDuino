/*
 * Example sketch to show how to use the TemplateDuino script
 */
 
/* includes for the webduino webserver */ 
#define WEBDUINO_FAIL_MESSAGE "<h1>Request Failed</h1>"
#include "SPI.h" // new include
#include "avr/pgmspace.h" // new include
#include "Ethernet.h"
#include "WebServer.h"

/* struct for data to be passed to the index.tpl template */
struct DATA_index {
  float humidity;
  float temp;
};

/* struct for the loop items in the list.tpl template */
struct DATA_PinList {
  String name;
  int value;
  int pin;
};

/* struct for data to be passed to the list.tpl template */
struct DATA_list {
  DATA_PinList pins[3];
};

/* stream-like operator << for server output */
template<class T>
inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

/* simulate iostream's endl */
char endl = '\n';

/* the generated include file, located in libraries/Templates/Templates.h */
#include "Templates.h"

/* to output some real data, we're using a DHT22 sensor, replace with you own stuff */
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;

/* ethernet address and ipv4 address, adjust to fit your network */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 10, 0, 0, 2 };

/* the webduino instance */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

/* initialize the loop variable instances */
DATA_PinList button1;
DATA_PinList button2;
DATA_PinList button3;

/* initialize the list.tpl struct, we fill it later */
DATA_list buttonlist;

/* fetch data from DHT22 and store it globally */
void getSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  }
  else {
     temperature = t;
     humidity    = h;
  }
}

/* default handler for webduino */
void tempCmd(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  if (type == WebServer::POST)  {
    server.httpSeeOther("PREFIX");
    return;
  }

  server.httpSuccess();

  if (type == WebServer::GET) {
    getSensorData();
    DATA_index data;           /* initialize the index.tpl struct */
    data.temp = temperature;   /* fill it with our sensor data    */
    data.humidity = humidity;
    tpl_index(server, data);   /* finally call the generated function for the index.tpl template */
  }
}

/* webduino handler for the list */
void listCmd(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  if (type == WebServer::POST)  {
    server.httpSeeOther("PREFIX");
    return;
  }

  if (type == WebServer::GET) {
    /* fill the data of each instance with status data of some buttons */
    for (int i=0; i<3; i++) {
      buttonlist.pins[i].value = digitalRead(buttonlist.pins[i].pin);
    }
    tpl_list(server, buttonlist); /* finally call the generated function for the list.tpl template */
  }
}


/* the usual thing */
void setup() {
  /* setup ethernet and webduino */
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&tempCmd);
  webserver.addCommand("list", &listCmd);
  webserver.begin();
  
  /* initialize loop instances with PINs and names */
  button1.name = "Left Button";
  button1.pin  = 10;
  button2.name = "Middle Button";
  button2.pin  = 11;
  button3.name = "Right Button";
  button3.pin  = 12;
  
  /* put the instances into the list used in the list.tpl */
  buttonlist.pins[0] = button1;
  buttonlist.pins[1] = button2;
  buttonlist.pins[2] = button3;
  
  /* set the buttons to input mode */
  for (int i=0; i<3; i++) {
     pinMode(buttonlist.pins[i].pin, INPUT);
  }
}

/* lastly the loop */
void loop() {
  webserver.processConnection();
}


