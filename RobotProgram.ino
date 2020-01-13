#include <Wire.h>
#include <L3G.h>

#define WATCHLED    10        
#define STARTBUTTON 9
// Setting pins / variables
int in1 = 2;
int in2 = 7;
int in3 = 6;
int in4 = 4;
int enA = 3;
int enB = 5;
int pwmOutput = 180;
int loopDelay = 45;
// Rotation in x
int gyrox;
// Sum of rotation in x
int gyroSum = 0;
int i;
boolean startbutton;

// Constructing gyroscope
L3G gyro;

void setup() {
  Serial.begin(9600);
  Wire.begin();
// Checks if gyro is enabled, if not wait
  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

// Set default values on gyroscope
  gyro.enableDefault();

// Set outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(WATCHLED, OUTPUT);    // sets the LED pin as output
  pinMode(STARTBUTTON,INPUT);   // sets the button pin as input
}

void loop()
{
 startbutton=false;

//-------------Watchdog code
  while (! startbutton) {        // stay in this loop until button pressed
    digitalWrite(WATCHLED,HIGH); // LED on
    for(i=0;i<10;i++) {
      delay(25);                 // wait a bit
      if (digitalRead(STARTBUTTON) == HIGH)  // check button
        startbutton=true;        // button is pressed
    }
    digitalWrite(WATCHLED,LOW);  // LED off
    for(i=0;i<30;i++) {
      delay(25);
      if (digitalRead(STARTBUTTON) == HIGH)
        startbutton=true;
    }
  } // end of while loop

 digitalWrite(WATCHLED,LOW);    // LED off
 robotcode();             // go to your robot code

  // Motors off after loop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  int gyroSum = 0;

} // return to top
//

void robotcode()
{
 delay(2000);
 
 // Loops function for 18 seconds
 for(int i=0;i<(18000/loopDelay);i++)
 {
  gyro.read();
  // Converts gyro to degrees
  gyrox = int(gyro.g.x*.00875);
  // Sums gyro in the x, and subtracts gyro offset
  gyroSum += gyrox - .5;
  // Converts gyro value to pwm out for motors, starting value 150, maxes at 255
//  pwmOutput = pow(gyrox, 1/2)*25 + 140;
  analogWrite(enA, pwmOutput);
  analogWrite(enB, pwmOutput);
  Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x*.00875);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y*.00875);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z*.00875);
  Serial.println(gyroSum);
  // Motors drive opposite of falling direction
  if(gyrox <= 0)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
  }else  // now change motor directions
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH); 
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH); 
  }
  // if the robot is falling a lot in one direction adds movement to that direction 
  if(abs(gyroSum >= 300)){
      if(gyroSum <= 0)
        {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in4, HIGH);
          digitalWrite(in3, LOW);
        }else  // now change motor directions
        {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH); 
          digitalWrite(in4, LOW);
          digitalWrite(in3, HIGH); 
        }
    delay(150);

  }
  delay(loopDelay);
}
}
