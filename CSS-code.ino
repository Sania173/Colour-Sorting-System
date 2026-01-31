#include <Servo.h> //libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  //OLED display configurations
#define SCREEN_HEIGHT 64  
#define OLED_RESET -1    

#define S0 9 // TCS230 colour sensor pin numbers according to my schematic
#define S1 10
#define S2 A2
#define S3 A3
#define sensorOut A1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //initialises OLED display

const int RED_LED_1 = 7; //connects to LED1 to digital pin 7
const int RED_LED_2 = 6;
const int BUTTON_2 = 12;
const int BUZZER = 4; //connects to buzzer to digital pin 4
const int SERVO_PIN = 11; //connects servo to digital pin 11

Servo myServo; //initialise servo motor

int redMin = 20, redMax = 186; // colour sensor RGB calibration values
int greenMin = 20, greenMax = 191;
int blueMin = 15, blueMax = 146;

int redValue, greenValue, blueValue; //variables to store RGB values
int token_acc = 0;//counts accepted tokens
int token_rej = 0;//counts rejected tokens
const int total_tokens = 8; //total tokens to process = 8


void setup() {
  pinMode(S0, OUTPUT); //setting sensor pin modes
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH); //configures colour sensor frequency scaling
  digitalWrite(S1, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //initialise OLED display
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  display.clearDisplay(); //displays startup message 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Press button to start");
  display.display();
 
  Serial.begin(9600);

  myServo.attach(SERVO_PIN); //sets servo position to neutral (the middle)
  myServo.write(90);
  delay(1000);
}

void loop() {
  // Wait for button press and 2 LED's turn on to indicate the system has started
  if (digitalRead(BUTTON_2) == HIGH) {
    digitalWrite(RED_LED_1, HIGH);
    digitalWrite(RED_LED_2, HIGH);

    display.clearDisplay(); //displays startup message
    display.setCursor(0, 0);
    display.println("Processing Tokens...");
    display.display();
    delay(500);

    while (token_acc + token_rej < total_tokens) {
      String color_output = determineColor();
      displayColorValues(color_output); //displays colour readings on OLED
      handleBuzzer(color_output);
      delay(1000);
    }

    digitalWrite(RED_LED_1, LOW); //turn off LED's when processing is complete
    digitalWrite(RED_LED_2, LOW);

    Serial.println("Token processing complete."); //displays final message
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Processing Complete!");
    display.display();
    while (true); //finish program
  }
}

String determineColor() { //function to determine the colour of the token
  int redPW = getRedPW(); //gets red pulse width
  redValue = map(redPW, redMin, redMax, 255, 0);

  int greenPW = getGreenPW();
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);

  int bluePW = getBluePW();
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);

  //determines the detected colour based on RGB values that I calibrated
  String color = "None";
  if (redValue >= 225 && greenValue >= 225 && blueValue >= 225) color = "White";
  else if (redValue > blueValue && redValue >= greenValue && redValue > 200) color = "Red";
  else if (blueValue > greenValue && blueValue > redValue && blueValue > 150) color = "Blue";
  else if (greenValue > redValue && greenValue > blueValue && greenValue >= 120) color = "Green";
  else if (redValue < 30 && greenValue < 30 && blueValue < 30) color = "Black";

  // counts accepted or rejected tokens
  if (color == "Green") token_acc++;
  else if (color != "None") token_rej++;

  return color;
}
//function to display colour values on OLED screen
void displayColorValues(String color) {
  display.clearDisplay();
  display.setCursor(0, 0); //sets where each line should be written on the OLED display
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
//function to control the buzzer and servo based on detected colour
void handleBuzzer(String color) {
  if (color == "Red") {
    tone(BUZZER, 4000, 200); //beep for red token
    delay(250);
    myServo.write(180); //move servo
    delay(400);
    myServo.write(90); //return servo to neutral
  } else if (color != "None") {
    myServo.write(0);
    delay(400);
    myServo.write(90);
  }
}

int getRedPW() { //created functions to get pulse width values for red, green and blue
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
//end of program
