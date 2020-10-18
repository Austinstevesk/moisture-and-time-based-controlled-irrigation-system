#include <LiquidCrystal.h> //LCD library
#include <SoftwareSerial.h>
#include <string.h>
#include <Wire.h>
#include <DS3231.h>
SoftwareSerial sim(9, 10); //Rx, Tx
int _timeout;
String _buffer;
String number = "+254721992025";
bool pumping = false;

DS3231  rtc(SDA, SCL);
//Relay pin
int Relay = 8;
int percentval;
bool state = false;

//Setting up the time when we need the the pump on and off
Time t;

int OnHour = 19;
int OnHour1 =19 ;
int OnMin = 23;
int OnMin1 = 25;

int current_minute = 0;

bool pump_on = false;

LiquidCrystal lcd(2,3,4,5,6,7);//RS, E, D4, D5, D6, D7


void setup() {
  delay(5000); //delay for 5 seconds to make sure the modules get the signal
  Serial.begin(9600);
  pinMode(Relay,OUTPUT);
  // Uncomment the next line if you are using an Arduino Leonardo
  //while (!Serial) {}
  
  // Initialize the rtc object
  rtc.begin();
  lcd.begin(16,2);
  Serial.println("System Started...");
  sim.begin(9600);
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(14, 38, 00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(10, 14, 2020);   // Set the date to January 1st, 2014
  //lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the 
                    //dimensions (width and height) of the display } 
   t = rtc.getTime();
   current_minute= t.min;
   
                   
  


}

void loop() {
  // put your main code here, to run repeatedly:
  // Send Day-of-Week
  /*
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  lcd.setCursor(1,0);
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  lcd.print(rtc.getDateStr());
  lcd.print("--");
  */
  
  // Send time
  t = rtc.getTime();
  String T = rtc.getTimeStr();
  Serial.print(T);
  
  
  // Wait one second before repeating
  int sensorval = analogRead(A0);

    Serial.print("\nAnalogValue: ");
    Serial.print(sensorval);
    percentval = map(sensorval, 1023, 200, 0, 100);
    //percentval = 5;
    Serial.print("\nPercentval: ");
    Serial.print(percentval);
    Serial.println(" ");
    //Serial.println(percentval);

  //if(current_minute != t.min){
     if(t.hour == OnHour && t.min == OnMin && percentval<30){
        Serial.print("Pumping\n");
        pump_on = true;
        digitalWrite(Relay, LOW);
        if (t.sec == 10){
          pumpOnMessage();
          state = true;
        }
      }
  else if(t.hour == OnHour1 && t.min == OnMin1 && percentval<30){
        Serial.print("Pumping\n");
        pump_on = true;
        digitalWrite(Relay, LOW);
        if (t.sec == 10){
          pumpOnMessage();
          state = true;
        }
        
        
      }
  else{
    if(pump_on == true){
       Serial.println("\nDone pumping\n");
      digitalWrite(Relay, HIGH);
      pump_on = false;
      pumpOffMessage();
    }
     
       }

   if(percentval > 40){
     if(pump_on == true){
     Serial.println("\nDone pumping\n");
      digitalWrite(Relay, HIGH);
      pump_on = false;
      if(state == true){
         pumpOffMessage();
         state = false;

      }
     }
  }
 
  delay (1000);
   lcd.setCursor(3,0);
   lcd.print(T);
   lcd.setCursor(7,1);
   lcd.print(percentval);
   
}


    void pumpOnMessage()
    {
      Serial.println ("Sending Message........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number"); nb
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      char value[5];
      itoa(percentval,value,10); //convert integer to char array      
      String SMS = "Pump on, moisture content is "+String(value);
      sim.println(SMS);
      Serial.println(SMS);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //_buffer = _readSerial();
    }

    


    void pumpOffMessage()
    {
      Serial.println ("Sending Message..........\n");
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number");
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
      char value[5];
      itoa(percentval,value,10); //convert integer to char array      
      String SMS = "Pump off, moisture content is "+String(value);
      sim.println(SMS);
      Serial.println(SMS);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
     // _buffer = _readSerial();
    }
    
   /* String _readSerial() {
      _timeout = 0;
      while  (!sim.available() && _timeout < 12000  )
      {
        delay(13);
        _timeout++;
      }
      if (sim.available()) {
        return sim.readString();
        Serial.print("sms sent");
      }
    }*/
