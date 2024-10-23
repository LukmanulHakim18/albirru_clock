#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <Servo.h>


// ================================ oop start ================================

const char NIGHT = 'N';
const char DAY = 'D';

class BirruClock{
private:
   void setDayNightCycle(){
      if (Hour >= 6 && Hour >= 17){
         DayNightCycle = NIGHT;
         Serial.println("set DayNightCycle to Night");
      }else{
         DayNightCycle= DAY;
         Serial.println("set DayNightCycle to Day");
      };
   };

   void setAngleOfServo(){
      if (DayNightCycle == NIGHT){
         if (AngleOfServo < 180){
            AngleOfServo++;
            Serial.println("set AngleOfServo ++");
         }
      }else{
         if (AngleOfServo > 0){
            AngleOfServo--;
            Serial.println("set AngleOfServo ++");
         };
      };
   };

   void setBrightness(){
      if (DayNightCycle == NIGHT){
         if (DisplayBrightness > 1){
            DisplayBrightness--;
            Serial.println("set DayNightCycle --");
         }
      }else{
         if (DayNightCycle < 7 ){
            DayNightCycle++;
            Serial.println("set DayNightCycle ++");
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
      DayNightCycle = DAY;
      
      Serial.println("masuk ke Update");
      setDayNightCycle();
      setAngleOfServo();
      setBrightness();

      Serial.println(DisplayBrightness);
      Serial.println(DayNightCycle);
      Serial.println(AngleOfServo);
      
   };
};

// ================================ oop end ================================



// Declarated pin
#define CLK D3   
#define DIO D4
#define PIN_SERVO D8

// Declarated Object
TM1637Display display = TM1637Display(CLK, DIO);
Servo myServo; 
BirruClock myClock;

String ssid = "";
String password = "";

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
   pinMode(LED_BUILTIN, OUTPUT);  
   digitalWrite(LED_BUILTIN, LOW);
   myClock.setupDefault();
   Serial.begin(115200);

   display.setBrightness(myClock.DisplayBrightness);
   timeClient.begin(); 

   myServo.write(myClock.AngleOfServo);
   
   Serial.println("Please input your Wi-Fi SSID:");
   while (ssid == "") {
    if (Serial.available()) {
      ssid = Serial.readStringUntil('\n');
      ssid.trim();  // Remove any extra whitespace characters
      Serial.print("SSID set to: ");
      Serial.println(ssid);
      delay(500);
    }
  }
  Serial.println("Please input your Wi-Fi Password:");
  while (password == "") {
    if (Serial.available()) {
      password = Serial.readStringUntil('\n');
      password.trim();  // Remove any extra whitespace characters
      Serial.print("Password set to: ");
      Serial.println(password);
      delay(500);
    }
  }

   WiFi.begin(ssid, password);

   while ( WiFi.status() != WL_CONNECTED ) {
      display.setSegments(aboy);
      
      digitalWrite(LED_BUILTIN, LOW); 
      delay ( 5000 );
      display.clear();
      Serial.print ( "." );      
      digitalWrite(LED_BUILTIN, HIGH);
   }
   digitalWrite(LED_BUILTIN, HIGH); 
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

   myClock.Update(hh, mm);
   display.setBrightness(myClock.DisplayBrightness);
   myServo.write(myClock.AngleOfServo);
   
   timeToDisplay = (hh*100) + mm;
   display.showNumberDecEx(timeToDisplay, 0b11100000, false, 4, 0);
   delay(500);
   display.showNumberDecEx(timeToDisplay,true);  
   delay(500); 
}
