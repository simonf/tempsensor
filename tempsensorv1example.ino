#include <AmHttpClient.h>
#include <b64.h>

#include <SPI.h>
#include <Ethernet.h>
#include <Xively.h>

bool connect = true;
int millisDelay = 15000;

byte mac[] = { 0x02, 0x00, 0x00, 0x75, 0x98, 0x43 };

char xivelyKey[] = "your key here";
#define FEED_ID 000000
char myFloatStream[] = "temperature";
XivelyDatastream datastreams[] = {
  XivelyDatastream(myFloatStream, strlen(myFloatStream), DATASTREAM_FLOAT)
};
XivelyFeed feed(FEED_ID, datastreams, 1);

EthernetClient client;
XivelyClient xivelyclient(client);

int sensorPin = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(connect) {
    if(Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
    } else {
      Serial.println("Ethernet connecting...");
      delay(1000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(sensorPin);
  Serial.print(reading);
  Serial.println(" analogIn");
  // Following formula is by trial and error
  float temp = (reading / 3.1)-24.0;
  Serial.print(temp);
  Serial.println(" degrees C");
  if(connect) {
    datastreams[0].setFloat(temp);
    int ret = xivelyclient.put(feed,xivelyKey);
    if(ret != 200) {
      Serial.print("xively client returned: ");
      Serial.println(ret);
    }
  }
  delay(millisDelay);
}
