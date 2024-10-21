#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>

#define CLK D3   
#define DIO D4

TM1637Display display = TM1637Display(CLK, DIO);

const char *ssid     = "Birru";
const char *password = "14072022";

const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Create a display object of type TM1637Display


const uint8_t aboy[] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // A
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,  // b
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,          // o
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,                  // y
};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

int hh, mm, ss, timeToDisplay;

void setup() {
  Serial.begin(115200);
  display.setBrightness(7);
  display.setSegments(aboy);
  delay(2000);
  timeClient.begin(); 
 
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
  timeClient.update();
  hh = timeClient.getHours();
  mm = timeClient.getMinutes();
  
  ss = timeClient.getSeconds();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(hh);
  Serial.print(":");
  Serial.print(mm);
  Serial.print(":");
  Serial.println(ss);
  Serial.println(timeClient.getFormattedTime());

  if (mm%2==0){
    display.setBrightness(7);
    for(int i=0;i<hh; i++){
      display.setSegments(aboy);
      delay(500);
      display.clear();
      delay(500);
      }   
    }else{
      display.setBrightness(1);
      }
  
  timeToDisplay = (hh*100)+mm;
  
  display.showNumberDecEx(timeToDisplay, 0b11100000, false, 4, 0);
//  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  display.showNumberDecEx(timeToDisplay,true);  
//  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
