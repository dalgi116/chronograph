#include <LiquidCrystal.h>
#define laserSensorPin 3
#define btnDown 12
#define btnUp 11
#define btnMain 2

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
int projectileMass;
int projectileLenght;
long unsigned int flightTime;
bool laserDetected;
bool btnDownPressed;
bool btnUpPressed;
bool btnMainPressed;

void setup() {
  lcd.begin(16, 2);
  pinMode(laserSensorPin, INPUT);
  pinMode(btnDown, INPUT);
  pinMode(btnUp, INPUT);
  pinMode(btnMain, INPUT);
}

void loop() {
  projectileMass = getMass(projectileMass);
  delay(500);
  projectileLenght = getLenght(projectileLenght);
  delay(500);
  flightTime = getFlightTime();
  if (flightTime) {
    writeOutput(projectileMass, projectileLenght, flightTime);
  }
  delay(500);
}



int getFlightTime() {
  long unsigned int flightTime = 0;
  clearDisplay();
  laserDetected = digitalRead(laserSensorPin);
  if (laserDetected) {
    showShootSite();
    btnMainPressed = digitalRead(btnMain);
    while (laserDetected) {
      if (btnMainPressed) {
        clearDisplay();
        showCancelSite();
        delay(1000);
        break;
      }
      laserDetected = digitalRead(laserSensorPin);
      btnMainPressed = digitalRead(btnMain);
    }
    if (!btnMainPressed) {
      long unsigned int startCountingTime = micros();
      while (!laserDetected) {
        laserDetected = digitalRead(laserSensorPin);
      }
      long unsigned int endCountingTime = micros();
      flightTime = endCountingTime - startCountingTime;
      }
  } else {
    showErrorSite();
    delay(3000);
  }
  return flightTime;
}

void writeOutput(int inputMass, int inputLenght, long unsigned int inputFlightTime) {
  float mass = convertToBasicUnits(inputMass);
  float lenght = convertToBasicUnits(inputLenght);
  float flightTime = convertToBasicUnits(convertToBasicUnits(inputFlightTime));

  float flightSpeed = getSpeed(lenght, flightTime);
  float energy = getEnergy(flightSpeed, mass);

  clearDisplay();
  btnMainPressed = digitalRead(btnMain);
  while (!btnMainPressed) {
    btnMainPressed = digitalRead(btnMain);
    showOutputSite(flightSpeed, energy);
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

void showOutputSite(float flightSpeed, float energy) {
    lcd.setCursor(0, 0);
    lcd.print("Speed: "); 
    lcd.print(flightSpeed);
    lcd.print("m/s");

    lcd.setCursor(0, 1);
    lcd.print("Energy: ");
    lcd.print(energy);
    lcd.print("J");
}

void showShootSite() {
  lcd.setCursor(3, 0);
  lcd.print("SHOOT NOW!");
  lcd.setCursor(0, 1);
  lcd.print("or press button");
}

void showErrorSite() {
  lcd.setCursor(0, 0);
  lcd.print("Laser can't hit");
  lcd.setCursor(0, 1);
  lcd.print("the sensor.");
}

void showCancelSite() {
  lcd.setCursor(4, 0);
  lcd.print("Canceled");
}

void clearDisplay() {
 lcd.setCursor(0, 0);
 lcd.print("                ");
 lcd.setCursor(0, 1);
 lcd.print("                "); 
}

int getMass(int mass) {
  clearDisplay();
  btnMainPressed = digitalRead(btnMain);
  while (!btnMainPressed) {
    btnMainPressed = digitalRead(btnMain);
    btnUpPressed = digitalRead(btnUp);
    btnDownPressed = digitalRead(btnDown);
    if (btnUpPressed) {
      mass++;
    }
    else if (btnDownPressed && mass > 0) {
      mass--;
    }
    showMassSite(mass);
    delay(100);
  }
  return mass;
}

int getLenght(int lenght) {
  clearDisplay();
  btnMainPressed = digitalRead(btnMain);
  while (!btnMainPressed) {
    btnMainPressed = digitalRead(btnMain);
    btnUpPressed = digitalRead(btnUp);
    btnDownPressed = digitalRead(btnDown);
    if (btnUpPressed) {
      lenght++;
    }
    else if (btnDownPressed && lenght > 0) {
      lenght--;
    }
    showLenghtSite(lenght);
    delay(100);
  }
  return lenght;
}
