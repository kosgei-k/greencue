#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pirPin = 2;         
const int buttonPin = 3;      
const int ecoPin = 12;        
const int ledPin = 13;         
const int ldrPin = A0;        

const unsigned long timeoutDuration = 5000;         
const unsigned long overrideDuration = 10000;      
unsigned long motionDetectedTime = 0;
unsigned long overrideStartTime = 0;
unsigned long lastEcoCheckTime = 0;

bool isMotionActive = false;
bool manualOverrideActive = false;

const int lightThreshold = 500; 


float totalEnergySavedWh = 0.0;
const float baselineWattage = 60.0; 
void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(ledPin, OUTPUT);
  pinMode(ecoPin, OUTPUT);
  

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Energy Cue System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  
  Serial.begin(9600);
  lastEcoCheckTime = millis();
}

void loop() {
  int pirState = digitalRead(pirPin);
  int buttonState = digitalRead(buttonPin);
  int ldrValue = analogRead(ldrPin);
  unsigned long currentMillis = millis();

  
  if (buttonState == LOW) {
    manualOverrideActive = true;
    overrideStartTime = currentMillis;
    Serial.println("Manual override triggered by user!");
  }

  
  if (manualOverrideActive) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ecoPin, LOW);

    lcd.setCursor(0, 0);
    lcd.print("OVERRIDE MODE   ");
    lcd.setCursor(0, 1);
    lcd.print("Status: MANUAL ON");

    if (currentMillis - overrideStartTime >= overrideDuration) {
      manualOverrideActive = false;
      digitalWrite(ledPin, LOW);
      Serial.println("Manual override expired. Returning to sensor mode.");
    }
    
    lastEcoCheckTime = currentMillis; 
    delay(200);
    return; 
  }

  
  lcd.setCursor(0, 0);
  lcd.print("LDR:");
  lcd.print(ldrValue);
  lcd.print(" PIR:");
  lcd.print(pirState);
  lcd.print("   "); 

  
  if (pirState == HIGH) {
    if (ldrValue < lightThreshold) {
      // Room is dark: Turn on Red Light, turn off Green Eco light
      digitalWrite(ledPin, HIGH);
      digitalWrite(ecoPin, LOW);
      motionDetectedTime = currentMillis; 
      lastEcoCheckTime = currentMillis; // Reset eco timer
      
      lcd.setCursor(0, 1);
      lcd.print("Status: LIGHT ON ");
      
      if (!isMotionActive) {
        isMotionActive = true;
        Serial.println("Motion detected + Room is dark! Activating energy cue.");
      }
    } else {
      // Room is bright: Eco-Mode Active (Saving Energy!)
      digitalWrite(ledPin, LOW);
      digitalWrite(ecoPin, HIGH);
      
      // Calculate cumulative Watt-hours saved
      unsigned long elapsedEcoTime = currentMillis - lastEcoCheckTime;
      lastEcoCheckTime = currentMillis;
      
      if (elapsedEcoTime < 2000) { 
        // Wh = Watts * (Milliseconds / 3,600,000)
        totalEnergySavedWh += (baselineWattage * ((float)elapsedEcoTime / 3600000.0));
      }
      
      // Display running total on LCD Line 2
      lcd.setCursor(0, 1);
      lcd.print("Saved:");
      lcd.print(totalEnergySavedWh, 4); // Display 4 decimal places for visibility in sim
      lcd.print("Wh");
      
      Serial.print("Eco-mode active. Total Saved: ");
      Serial.print(totalEnergySavedWh, 4);
      Serial.println(" Wh");
    }
  } 
  else {
    lastEcoCheckTime = currentMillis; // Reset eco timer when idle
    
    // When no motion, handle timeout
    if (isMotionActive) {
      if (currentMillis - motionDetectedTime >= timeoutDuration) {
        digitalWrite(ledPin, LOW); 
        digitalWrite(ecoPin, LOW);
        isMotionActive = false;
        
        lcd.setCursor(0, 1);
        lcd.print("Status: IDLE     ");
        Serial.println("Timeout reached. Returning to idle state.");
      }
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Status: IDLE     ");
    }
  }

  delay(200);
}
