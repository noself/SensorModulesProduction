#include "Arduino.h"
#include <EEPROM.h>
#define R1 10000
#define R2 2200
#define VREF 1.2


struct eepromStruct {
  char sensorID[20];
  char wifi_ssid[30];
  char wifi_password[20];
  char AIO_SERVER[30];
  unsigned short int  AIO_SERVERPORT;
  char AIO_USERNAME[10];
  char AIO_KEY[10];
  char serverAddress[20];
  float slopeValue;
  float interceptValue;
  float voltageMultiplier;
  char dummy[20];
};

#define EEPROM_read(record, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) {pp[i]=EEPROM.read(offsetof(struct eepromStruct, record)+i); delay(100);}}
#define EEPROM_write(record, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) {EEPROM.write(offsetof(struct eepromStruct, record)+i, pp[i]); delay(100);}}

// crc 8 calculated with http://tomeko.net/online_tools/crc8.php?lang=en
/* Free Family code 0x52-0x80 */
//char sensorID[20]="2A000000001077F3"; //PhSensor Family code 2A
char sensorID[20]="5200000000117715"; //TurbiditySensor Family code 52
char wifi_ssid[30]="IoThingsWareBus";
char wifi_password[20]="07B04U1957S";
char AIO_SERVER[30]="raspberrypi.local";
unsigned short int AIO_SERVERPORT=1883;
char AIO_USERNAME[10]="";
char AIO_KEY[10]="";
char serverAddress[20]="";
float slopeValue=0.02;
float interceptValue=-1.38;
float voltageMultiplier=(R1+R2)/R2;


#if 0
char SensorID[16]="08024";
char wifi_ssid[] = "IoThingsWareBus";    //  your network SSID (name)
char wifi_password[] = "07B04U1957S";   // your network password
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "raspberrypi.local"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""
/************ Global State (you don't need to change this!) ******************/
#endif




void setup() {
  // put your setup code here, to run once:
  EEPROM.begin(512);
  Serial.begin(115200);
  delay(10);
  Serial.println("Start EEPROM programming");
  EEPROM_write(sensorID, sensorID);
  EEPROM_write(wifi_ssid, wifi_ssid);
  EEPROM_write(wifi_password, wifi_password);
  EEPROM_write(AIO_SERVER, AIO_SERVER);
  EEPROM_write(AIO_SERVERPORT, AIO_SERVERPORT);
  EEPROM_write(AIO_USERNAME, AIO_USERNAME);
  EEPROM_write(AIO_KEY, AIO_KEY);
  EEPROM_write(serverAddress, serverAddress);
  EEPROM_write(slopeValue, slopeValue);
  EEPROM_write(interceptValue, interceptValue);
  EEPROM_write(voltageMultiplier, voltageMultiplier);
  EEPROM.commit();
  Serial.println("EEPROM programmed");
}

void loop() {

}
