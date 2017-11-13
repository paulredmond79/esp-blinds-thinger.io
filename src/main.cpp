#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include <RAEXRemote.h>
#include <EEPROM.h>

#define LED_PIN         2
#define EEPROM_SALT     533
#define TX_PIN          13


static Transmitter transmitter = Transmitter(TX_PIN);
static Scheduler scheduler = Scheduler();
static RAEXRemote raexRemote = RAEXRemote(&scheduler, &transmitter);

ThingerWifi thing("paulredmond", "ESP_Blinds", "tG0##3iU#0Dr");

void controlRaex(unsigned int channel,unsigned int remote,unsigned int action) {

  Serial.print("Channel: ");
  Serial.print(channel);
  Serial.print(", Remote: ");
  Serial.print(remote);
  Serial.print(", Action: ");
  Serial.println(action);

  RAEXRemoteCode raexRemoteCode = RAEXRemoteCode(channel, remote, action);
  raexRemote.transmitCode(&raexRemoteCode);
}


void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  thing.add_wifi("iQGuest", "iquate123");
  thing["payload"] << [](pson& in){
    unsigned int channel = in["channel"];
    unsigned int remote = in["remote"];
    unsigned int action = in["action"];
    controlRaex(channel,remote,action);
};
}

void loop() {
  thing.handle();
}

