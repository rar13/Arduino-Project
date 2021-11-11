#include <ESP8266WiFi.h> 
#include <DHT.h>
#include <ThingerESP8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 
 
#define USERNAME "rifqiammar" //Username thinger.io
#define DEVICE_ID "AMMAR_IOT" 
#define DEVICE_CREDENTIAL "wS9dTwmowZmV"
 
#define SSID "Red" //Hotspot yang kita pakai
#define SSID_PASSWORD "12345678910"
 
#define DHTPIN D6 //Pin data dari DHT terhubung ke pin D7 NodeMCU
#define DHTTYPE DHT22
 
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
 
DHT dht(DHTPIN, DHTTYPE);
 
float hum,temp;

const int pinLpg = D5;


const int pinBuzzer = D7;

int data;
 
void setup() {

  lcd.begin(16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  pinMode(pinLpg, INPUT);
  
   
   pinMode(pinBuzzer, OUTPUT);


  
  
  Serial.begin(115200);
  Serial.println("Baca DHT22");
 
  dht.begin();
 
  thing.add_wifi(SSID, SSID_PASSWORD);
 
  thing["Monitoring_Suhu_Kelembapan"] >> [](pson& out){
    out["KELEMBAPAN"] = hum;
    out["SUHU"] = temp;
  };
}
 
void loop() {  

 data = digitalRead(pinLpg);
   if (data == HIGH)
 {
 // alarm mati
  
    
   digitalWrite(pinBuzzer, LOW);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print(" KONDISI RUANGAN ");
  lcd.setCursor(0,1);
  lcd.print("===== BAIK =====");
  
 }
  else if (data == LOW)
 {
 
 
  digitalWrite(pinBuzzer, HIGH);
  
  
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("KONDISI RUANGAN ");
   lcd.setCursor(0,1);
  lcd.print("=== BERBAHAYA ====  ");
 }
  

thing.handle(); 
  delay(1000); //1 detik
  //sensor membutuhkan waktu 250 ms ketika membaca suhu dan kelembaban 
  float h = dht.readHumidity(); //Membaca kelembaban
  float t = dht.readTemperature(); //Membaca suhu dalam satuan Celcius
  
 
  hum = h;
  temp = t;
   delay(1000); //1 detik
  
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.print("% RH ");
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.print(" *C ");

}
