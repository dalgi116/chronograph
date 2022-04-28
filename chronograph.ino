#include <LiquidCrystal.h>
#define sensorPin 3
#define btnDown 12
#define btnUp 11
#define btnMain 2

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
int projectileMass;
int projectileLenght;
bool sensorValue;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(sensorPin, INPUT);
  pinMode(btnDown, INPUT);
  pinMode(btnUp, INPUT);
  pinMode(btnMain, INPUT);

  int projectileMass;
  int projectileLenght;
}

void loop() {
  projectileMass = getMass(projectileMass);
  delay(500);
  projectileLenght = getLenght(projectileLenght);
  delay(500);
  clearDisplay();
  while (true) {
   sensorValue = digitalRead(sensorPin);
   orderToShoot();
   if (sensorValue == 0 || digitalRead(btnMain) == 1) {
     break;
   }
  }
  if (sensorValue == 0) {
    int flightTime = getFlightTime();
    writeOutput(projectileMass, projectileLenght, flightTime);
  }
  delay(500);
}


// calculates the flight time of the projectile through the laser
int getFlightTime() {
  int counter = 0;
  while (sensorValue == 0) {
    sensorValue = digitalRead(sensorPin);
    delayMicroseconds(1);
    counter++;
   }
   return counter;
}

void writeOutput(int inputMass, int inputLenght, int inputFlightTime) {
  float mass = convertToBasicUnits(inputMass);
  float lenght = convertToBasicUnits(inputLenght);
  float flightTime = convertToBasicUnits(convertToBasicUnits(inputFlightTime));

  float flightSpeed = getSpeed(lenght, flightTime);
  float energy = getEnergy(flightSpeed, mass);

  clearDisplay();
  while (digitalRead(btnMain) != 1) {
    lcd.setCursor(0, 0);
    lcd.print("Speed: "); 
    lcd.print(flightSpeed);
    lcd.print("m/s");

    lcd.setCursor(0, 1);
    lcd.print("Energy: ");
    lcd.print(energy);
    lcd.print("J");
  }
}

float convertToBasicUnits(float commonUnit) {
  float basicUnit = commonUnit / 1000;
  return basicUnit;
}

float getSpeed(float s, float t) {
  float v = s / t;
  return v;
  }

float getEnergy(float v, float m) {
  float E = m * v * v / 2;
  return E;
  }

void showMassSite(int mass) {
   lcd.setCursor(0, 0);
   lcd.print("Mass: ");
   lcd.print(mass);
   lcd.print("g ");
  }

void showLenghtSite(int lenght) {
   lcd.setCursor(0, 0);
   lcd.print("Lenght: ");
   lcd.print(lenght);
   lcd.print("mm ");
  }

int getMass(int mass) {
  clearDisplay();
  while (digitalRead(btnMain) != 1) {
    if (digitalRead(btnUp) == 1) {
      mass++;
    }
    else if (digitalRead(btnDown) == 1 && mass > 0) {
      mass--;
    }
    showMassSite(mass);
    delay(100);
  }
  return mass;
}

int getLenght(int lenght) {
  clearDisplay();
  while (digitalRead(btnMain) != 1) {
    if (digitalRead(btnUp) == 1) {
      lenght++;
    }
    else if (digitalRead(btnDown) == 1 && lenght > 0) {
      lenght--;
    }
    showLenghtSite(lenght);
    delay(100);
  }
  return lenght;
}

void orderToShoot() {
  lcd.setCursor(3, 0);
  lcd.print("SHOOT NOW!");
  lcd.setCursor(0, 1);
  lcd.print("or press button");
}

void clearDisplay() {
 lcd.setCursor(0, 0);
 lcd.print("                ");
 lcd.setCursor(0, 1);
 lcd.print("                "); 
}
