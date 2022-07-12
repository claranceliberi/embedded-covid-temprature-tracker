#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// Pins mapped with devices
#define REDPIN 14  // D5
#define GREENPIN 16 // D0
#define BUZZERPIN 12 // D6 
#define TEMPVOUT A0 // A0


WiFiClient wifiClient;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

    Serial.begin(9600);

    // Connecting to WiFi
     WiFi.begin("RCA-WiFi", "rca@2019");
     
    // LCD
    Wire.begin();
//    lcd.begin();
    lcd.init();
    lcd.backlight(); 

    // LEDs and Buzzer connection
    pinMode(GREENPIN, OUTPUT);
    pinMode(REDPIN, OUTPUT);
    pinMode(BUZZERPIN, OUTPUT);
}

void loop() {

  //Read and calculate temprature
  int analogValue = analogRead(TEMPVOUT);
  float celsius = (analogValue/1024.0) * 5 *100;

  Serial.print("Degrees READ=   ");
  Serial.println(celsius);

  String mData="";
  String dummy_device = "340722SPE0482022";
  mData = "device="+dummy_device+"&temperature="+(String)celsius;
    
  if(celsius < 35 ){
    
    digitalWrite(GREENPIN, HIGH); // Turn on GREEN LED
    digitalWrite(REDPIN, LOW);// Turn off GREEN LED
    digitalWrite(BUZZERPIN, LOW); // Turn off BUZZER

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((String)celsius + " celsius");   

    
  }else{

    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((String)celsius + " celsius");  

    
    digitalWrite(GREENPIN, LOW); // Turn on GREEN LED
    digitalWrite(BUZZERPIN, HIGH); // Turn off BUZZER
    digitalWrite(REDPIN, HIGH); // Turn off GREEN LED

    sendData(80, "192.168.1.150", "/iot/" , mData); 
  }

  delay(2000);

}


void sendData(const int httpPort, const char* clientHost,const char* path , String data){
  
  wifiClient.connect(clientHost, httpPort); 

  wifiClient.println("POST "+(String)path+" HTTP/1.1");
  wifiClient.println("Host: " + (String)clientHost);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.println("Content-Type: application/x-www-form-urlencoded");
  wifiClient.println("Content-Length: " +(String)data.length());
  wifiClient.println();
  wifiClient.print(data);

  Serial.println("Response: " + wifiClient.readStringUntil('\n'));

}
