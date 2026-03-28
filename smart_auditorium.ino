#include <DHT.h>
#include <LiquidCrystal.h>

// PIN MAPPING
#define DHTPIN 13
#define DHTTYPE DHT11
#define FLAME_PIN 9
#define BUZZER_PIN 10
#define IR_IN 8
#define IR_OUT A4
#define LDR_PIN A5
#define RELAY1 A0 // 2 Fans
#define RELAY2 A1 // 1 Fan
#define RELAY3 A2 // All LEDs (LDR Reversed)
#define RELAY4 A3 // Exhaust Fan (Temp)

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

int members = 0;
bool lastInState = HIGH;
bool lastOutState = HIGH;

void setup() {
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(IR_IN, INPUT);
  pinMode(IR_OUT, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Relays OFF initially (Active Low modules use HIGH for OFF)
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  dht.begin();
  lcd.begin(16, 2);

  lcd.print("SMART AUDITORIUM");
  lcd.setCursor(0, 1);
  lcd.print("POWER SAVING SYS");
  delay(3000);
  lcd.clear();
}

void loop() {
  // 1. EMERGENCY CHECK
  if (digitalRead(FLAME_PIN) == LOW) {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    digitalWrite(RELAY4, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.clear();
    lcd.print("!! FIRE ALERT !!");
    lcd.setCursor(0, 1);
    lcd.print("EVACUATE NOW");
    while (digitalRead(FLAME_PIN) == LOW)
      ;
    digitalWrite(BUZZER_PIN, LOW);
    lcd.clear();
  }

  // 2. BI-DIRECTIONAL COUNTING (Sequence Detection)
  bool inTriggered = (digitalRead(IR_IN) == LOW);
  bool outTriggered = (digitalRead(IR_OUT) == LOW);

  if (inTriggered && lastInState == HIGH) {
    if (outTriggered) { // Sequence: OUT then IN (EXIT)
       if (members > 0) members--;
       delay(500);
    } else { // Sequence: IN triggered first (ENTRY)
       members++;
       delay(500);
    }
  }
  lastInState = inTriggered;
  lastOutState = outTriggered;

  // 3. LDR LOGIC (Relay 3) - Fixed: OFF during day
  int lightVal = analogRead(LDR_PIN);
  bool isDaytime = (lightVal >= 500);
  // Active Low Relay: HIGH = OFF, LOW = ON
  digitalWrite(RELAY3, isDaytime ? HIGH : LOW); // Lights OFF if daytime

  // 4. NEW CAPACITY FAN LOGIC
  if (members >= 3) {
    digitalWrite(RELAY1, LOW); // 2 Fans ON
    digitalWrite(RELAY2, LOW); // 1 Fan ON (Total 3)
  } else if (members >= 1) {
    digitalWrite(RELAY1, LOW);  // 2 Fans ON
    digitalWrite(RELAY2, HIGH); // 1 Fan OFF
  } else {
    digitalWrite(RELAY1, HIGH); // All OFF
    digitalWrite(RELAY2, HIGH);
  }

  // 5. EXHAUST & ENVIRONMENT
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  digitalWrite(RELAY4, (temp > 30) ? LOW : HIGH);

  // 6. DASHBOARD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print((int)temp);
  lcd.print("C H:");
  lcd.print((int)hum);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("MEM:");
  lcd.print(members);
  lcd.print(isDaytime ? " L:DAY " : " L:NITE");
}
