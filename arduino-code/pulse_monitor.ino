#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>

// ESP8266 configuration
SoftwareSerial esp8266(9, 10);
#define SSID "YOUR_WIFI_SSID"        // Replace with WiFi SSID
#define PASS "YOUR_WIFI_PASSWORD"    // Replace with WiFi Password
#define IP "184.106.153.149"         // ThingSpeak IP
String msg = "GET /update?key=YOUR_API_KEY"; // Replace with your ThingSpeak API key

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pulse sensor pins
int pulsePin = 0;  
int blinkPin = 7;  
int fadePin = 5;   
int fadeRate = 0;

// Heartbeat-related variables
volatile int BPM;                
volatile int Signal;             
volatile int IBI = 600;          
volatile boolean Pulse = false;  
volatile boolean QS = false;     
volatile int rate[10];           

// Timing variables
volatile unsigned long sampleCounter = 0;
volatile unsigned long lastBeatTime = 0;

// Peak/trough detection
volatile int P = 512;
volatile int T = 512;
volatile int thresh = 525;
volatile int amp = 100;

// Flags
volatile boolean firstBeat = true;
volatile boolean secondBeat = false;

// Error handling
int error;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Connecting...");

  Serial.begin(9600);
  esp8266.begin(9600);

  esp8266.println("AT");
  delay(5000);

  if (esp8266.find("OK")) {
    connectWiFi();
  }

  interruptSetup(); // Timer2 for heartbeat interrupts
}

void loop() {
  lcd.clear();
start:
  error = 0;

  lcd.setCursor(0, 0);
  lcd.print("BPM = ");
  lcd.print(BPM);

  lcd.setCursor(0, 1);
  if (BPM < 100 && BPM > 60) {
    lcd.print(" Normal");
  } else {
    lcd.print(" Abnormal");
  }

  delay(1000);
  updatebeat();

  if (error == 1) {
    goto start; // Retry if WiFi error
  }

  delay(1000);
}

// Send BPM data to ThingSpeak
void updatebeat() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";

  esp8266.println(cmd);
  delay(200);

  if (esp8266.find("Error")) return;

  cmd = msg;
  cmd += "&field1=";
  cmd += BPM;
  cmd += "\r\n";

  Serial.println(BPM);

  esp8266.print("AT+CIPSEND=");
  esp8266.println(cmd.length());

  if (esp8266.find(">")) {
    esp8266.print(cmd);
  } else {
    esp8266.println("AT+CIPCLOSE");
    error = 1;
  }
}

// Connect ESP8266 to WiFi
boolean connectWiFi() {
  esp8266.println("AT+CWMODE=1");
  delay(2000);

  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";

  esp8266.println(cmd);
  delay(5000);

  return esp8266.find("OK");
}

// Timer2 interrupt setup
void interruptSetup() {
  TCCR2A = 0x02;  
  TCCR2B = 0x06;  
  OCR2A = 0x7C;   
  TIMSK2 = 0x02;  
  sei();          
}

// Timer2 ISR for pulse detection
ISR(TIMER2_COMPA_vect) {
  cli(); 

  Signal = analogRead(pulsePin);
  sampleCounter += 2;
  int N = sampleCounter - lastBeatTime;

  if (Signal < thresh && N > (IBI / 5) * 3) {
    if (Signal < T) T = Signal;
  }

  if (Signal > thresh && Signal > P) {
    P = Signal;
  }

  if (N > 250) {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3)) {
      Pulse = true;
      digitalWrite(blinkPin, HIGH);

      IBI = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;

      if (secondBeat) {
        secondBeat = false;
        for (int i = 0; i <= 9; i++) rate[i] = IBI;
      }

      if (firstBeat) {
        firstBeat = false;
        secondBeat = true;
        sei();
        return;
      }

      word runningTotal = 0;
      for (int i = 0; i <= 8; i++) {
        rate[i] = rate[i + 1];
        runningTotal += rate[i];
      }
      rate[9] = IBI;
      runningTotal += rate[9];
      runningTotal /= 10;
      BPM = 60000 / runningTotal;
      QS = true;
    }
  }

  if (Signal < thresh && Pulse == true) {
    digitalWrite(blinkPin, LOW);
    Pulse = false;
    amp = P - T;
    thresh = amp / 2 + T;
    P = thresh;
    T = thresh;
  }

  if (N > 2500) {
    thresh = 512;
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;
    firstBeat = true;
    secondBeat = false;
  }

  sei();
}
