#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  
#define OLED_RESET -1    

#define S0 9
#define S1 10
#define S2 A2
#define S3 A3
#define sensorOut A1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int RED_LED_1 = 7; //connects to LED1 to digital pin 7
const int RED_LED_2 = 6;
const int BUTTON_2 = 12;
const int BUZZER = 4; //connects to buzzer to digital pin 4
const int SERVO_PIN = 11;

Servo myServo;

int redMin = 20, redMax = 186;
int greenMin = 20, greenMax = 191;
int blueMin = 15, blueMax = 146;

int redValue, greenValue, blueValue;
int token_acc = 0;
int token_rej = 0;
const int total_tokens = 8;


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

   //digitalWrite(5, HIGH);
   //delay (100); //delay of 100ms
   //digitalWrite(5, LOW);
   

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Press button to start");
  display.display();
 
  Serial.begin(9600);

  myServo.attach(SERVO_PIN);
  myServo.write(90);
  delay(1000);
}

void loop() {
  // Wait for button press  
  if (digitalRead(BUTTON_2) == HIGH) {
    digitalWrite(RED_LED_1, HIGH);
    digitalWrite(RED_LED_2, HIGH);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Processing Tokens...");
    display.display();
    delay(500);

    while (token_acc + token_rej < total_tokens) {
      String color_output = determineColor();
      displayColorValues(color_output);
      handleBuzzer(color_output);
      delay(1000);
    }

    digitalWrite(RED_LED_1, LOW);
    digitalWrite(RED_LED_2, LOW);

    Serial.println("Token processing complete.");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Processing Complete!");
    display.display();
    while (true);
  }
}

String determineColor() {
  int redPW = getRedPW();
  redValue = map(redPW, redMin, redMax, 255, 0);

  int greenPW = getGreenPW();
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);

  int bluePW = getBluePW();
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);

  
  String color = "None";
  if (redValue >= 225 && greenValue >= 225 && blueValue >= 225) color = "White";
  else if (redValue > blueValue && redValue >= greenValue && redValue > 200) color = "Red";
  else if (blueValue > greenValue && blueValue > redValue && blueValue > 150) color = "Blue";
  else if (greenValue > redValue && greenValue > blueValue && greenValue >= 120) color = "Green";
  else if (redValue < 30 && greenValue < 30 && blueValue < 30) color = "Black";

  if (color == "Green") token_acc++;
  else if (color != "None") token_rej++;

  return color;
}

void displayColorValues(String color) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Red: "); display.println(redValue);
  display.setCursor(0, 10);
  display.print("Green: "); display.println(greenValue);
  display.setCursor(0, 20);
  display.print("Blue: "); display.println(blueValue);
  display.setCursor(0, 30);
  display.print("Detected Color: "); display.println(color);
  display.setCursor(0, 40);
  display.print("Accepted: "); display.println(token_acc);
  display.print("Rejected: "); display.println(token_rej);
  display.display();
}

void handleBuzzer(String color) {
  if (color == "Red") {
    tone(BUZZER, 4000, 200);
    delay(250);
    myServo.write(180);
    delay(400);
    myServo.write(90);
  } else if (color != "None") {
    myServo.write(0);
    delay(400);
    myServo.write(90);
  }
}

int getRedPW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  return pulseIn(sensorOut, LOW);
}

int getGreenPW() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

int getBluePW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}
