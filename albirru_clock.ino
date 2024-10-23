#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <Servo.h>

// Declarated pin
#define CLK D3   
#define DIO D4
#define PIN_SERVO D8

// Declarated Object
TM1637Display display = TM1637Display(CLK, DIO);
Servo setupDefault(); 
BirruClock myClock;

const char *ssid     = "Birrur";
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
   myClock.setupDefault();
   Serial.begin(115200);
   display.setBrightness(7);
   timeClient.begin(); 

   WiFi.begin(ssid, password);

   while ( WiFi.status() != WL_CONNECTED ) {
      display.setSegments(aboy);
      delay ( 500 );
      display.clear();
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


// ================================ oop ================================

const char NIGHT = 'NIGHT';
const char DAY = 'DAY';

class BirruClock{
private:
   void setDayNightCycle(){
      if (Hour >= 17){
         DayNightCycle = NIGHT;
      }else{
         DayNightCycle= DAY;
      };
   };

   void setAngleOfServo(){
      if (DayNightCycle == NIGHT){
         if (AngleOfServo < 180){
            AngleOfServo++;
         }
      }else{
         if (AngleOfServo > 0){
            AngleOfServo--;
         };
      };
   };

   void setBrightness(){
      if (DayNightCycle == NIGHT){
         if (DisplayBrightness < 7){
            DisplayBrightness--;
         }
      }else{
         if (AngleOfServo > 0){
            AngleOfServo--;
         };
      };
   };

public: 
   int Hour;
   int Minute;
   int DisplayBrightness;

   char DayNightCycle;

   int AngleOfServo;
   
   void setupDefault(){
      AngleOfServo = 0;
      DisplayBrightness = 7;
   }
   void Update(int hh ,int mm ){
      Hour = hh;
      Minute = mm;
      setDayNightCycle();
      setAngleOfServo();
   };
};


// class String{};
