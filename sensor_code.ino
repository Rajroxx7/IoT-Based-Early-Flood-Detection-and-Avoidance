
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 7    // what digital pin we're connected to

#define DHTTYPE DHT11 

#define sensorPower 6
#define sensorPin A0 

int val = 0;

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}

volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 9; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  Serial.begin(9600);
  //20 by 4 character display
  //If you're using a 16x2 display, change it to lcd.begin(16,2);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
  lcd.begin(20,4); 
  Serial.println("DHT11 test!");
  dht.begin();  
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  int level = readSensor();
  

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
    }
  
  dht.read(h);
  dht.read(t);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0,1);
  lcd.print("Temp (C): ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0,2);
  lcd.print("Water level : ");
  lcd.print(level);
  lcd.setCursor(0,3);
  lcd.print("Water Flow : ");
  lcd.print(l_hour);
  lcd.print(" L/hour");

  //Serial monitor output
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\n");
  Serial.print("Temp (C): ");
  Serial.print(t);
  Serial.print(" C\n");
  Serial.print("Water level : ");
  Serial.print(level);
  Serial.print("\n");
  Serial.print("Water flow : ");
  Serial.print(l_hour);
  Serial.print("L/hour\n");
  Serial.print("___________________________\n");
 }

 
