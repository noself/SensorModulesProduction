#include "Arduino.h"
#include <EEPROM.h>
#define R1 10000
#define R2 2200
#define VREF 1.2


struct eepromStruct {
  unsigned char sensorID[20];
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

#define EEPROM_read(record, p)  \
{\
  int i = 0;\
  byte *pp = (byte*)&(p);\
  for(; i < sizeof(p);i++)\
  {\
    pp[i]=EEPROM.read(offsetof(struct eepromStruct, record)+i);\
    delay(100);\
  }\
}


#define  EEPROM_write(record, p)  \
{\
  int i = 0;\
  byte *pp = (byte*)&(p);\
  for(; i < sizeof(p); i++)\ 
  {\
    EEPROM.write(offsetof(struct eepromStruct, record)+i, pp[i]);\
    delay(100);\
   }\
}


// crc 8 calculated with http://tomeko.net/online_tools/crc8.php?lang=en
/* Free Family code 0x52-0x80 */
//char sensorID[20]="2A000000001077F3"; //PhSensor Family code 2A
//char sensorID[20]="5200000000117715"; //TurbiditySensor Family code 52
char sensorID[20]="530000000012777D"; //SecurityJacketSensor Family code 53
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

/*  
 * crc8.c
 * 
 * Computes a 8-bit CRC 
 * 
 */

#include <stdio.h>


#define GP  0x107   /* x^8 + x^2 + x + 1 */
#define DI  0x07


static unsigned char crc8_table[256];     /* 8-bit table */
static int made_table=0;

static void init_crc8()
     /*
      * Should be called before any other crc function.  
      */
{
  int i,j;
  unsigned char crc;
  
  if (!made_table) {
    for (i=0; i<256; i++) {
      crc = i;
      for (j=0; j<8; j++)
        crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
      crc8_table[i] = crc & 0xFF;
      /* printf("table[%d] = %d (0x%X)\n", i, crc, crc); */
    }
    made_table=1;
  }
}


void crc8(char *crc, char m)
     /*
      * For a byte array whose accumulated crc value is stored in *crc, computes
      * resultant crc obtained by appending m to the byte array
      */
{
  if (!made_table)
    init_crc8();

  *crc = crc8_table[(*crc) ^ (m-'0')];
  *crc &= 0xFF;
}


void setup() {
  int i;
  init_crc8();
  // put your setup code here, to run once:
  EEPROM.begin(512);
  Serial.begin(115200);
  delay(200);
  Serial.println("Start EEPROM programming");
  sensorID[15]=0;
  for(i=0; i < 14; i++)
	  crc8(&sensorID[15], sensorID[i]);
  sprintf(&sensorID[14],"%02x",sensorID[15]);
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
  Serial.print("SensorID: ");
	Serial.print(sensorID);
  Serial.println();
  Serial.println("EEPROM programmed");
}

void loop() {

}
