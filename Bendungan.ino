#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <HCSR04.h>
#include <Servo.h>
#include <SoftwareSerial.h>

BlynkTimer timer;
Servo servo;

WidgetLCD lcd(V2);
WidgetLCD lcdStat(V3);
SoftwareSerial EspSerial(2, 3); // RX, TX

#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

int trigPin = 6;
int echoPin = 7;
long duration;
int depthTemp;
int i = 1;
int depth;
int maxDepth = 21;
int avgVolumeTemp;
int avgVolume;

char auth[] = "19Quig28ELTKssw5tzxUurcq7HTwwKE_"; 
char ssid[] = "Telor Asin";
char pass[] = "123456eh123";

void setup()
{
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  

  Serial.begin(9600);
  delay(10);
  
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  
  Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8080);
  // Blynk.begin(auth, wifi, ssid, pass);
  
  servo.attach(9);
  
  lcd.clear();

}

BLYNK_WRITE(V1){
  servo.write(param.asInt());
  
}

void loop()
{   
    Blynk.run();
    timer.run();
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      depthTemp = (duration * 0.0345) / 2;
      depth = maxDepth - depthTemp;

    avgVolumeTemp = (21 *21 *depth);

    avgVolume = avgVolumeTemp / i;
    i++;
  

    lcdStat.print(0,0,"Status");
    if(depth <=7){
        lcdStat.print(0,1,"Rendah");
      }else if(depth>7 && depth<=14){
        lcdStat.print(0,1,"Sedang");
      }else{
        lcdStat.print(0,1,"Tinggi");
      }
    
    Blynk.virtualWrite(V5, depth);
    Blynk.virtualWrite(V2, depth);
    Blynk.virtualWrite(V2, avgVolume);
    Blynk.virtualWrite(V6, avgVolume);
      
    lcd.print(0, 0, "Depth: ");
    lcd.print(8, 0, depth);
    lcd.print(10, 0, " cm");
    lcd.print(0, 1, "Avg: ");
    lcd.print(6, 1, avgVolume); 
    lcd.print(8, 1, " cm3");

    Serial.print("Distance: ");
    Serial.println(depth);
    delay(2000);
}
