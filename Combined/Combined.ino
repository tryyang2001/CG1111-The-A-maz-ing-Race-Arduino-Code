/** 
 * Main code for running of mBot 
 * 
 * Values of whiteArray, blackArray and greyDiff are pre-determined using "Colour_Sensor_Calibration".
 * Values of lower limit values for each colour used in decideColour() are pre-determined using "Colour_Range_Calibration".
 * 
 */
 
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

MeUltrasonicSensor ultraSensor(PORT_3); //Ultrasonic sensor
MeLineFollower lineFinder(PORT_2); //This is for detecting black line
MeRGBLed led(0, 30); //Inbuilt LED
MeLightSensor lightSensor(PORT_8); //Inbuilt Light Sensor
MeBuzzer buzzer; //Inbuilt buzzer

MeDCMotor MotorL(M1); //Motor 1 is left motor
MeDCMotor MotorR(M2); //Motor 2 is right motor

int analogInPin1 =  A0;// analog input pin for right  IR sensor
int analogInPin2 =  A1;// analog input pin for left IR sensor

double initialRight; // declaring global variable for initial right IR sensor voltage value
double initialLeft; // declaring global variable for initial left IR sensor voltage value

//Base colour arrays from pre-calibration (Order: RGB)
const float whiteArray[] = {496.00, 381.00, 438.00};
const float blackArray[] = {345.00, 255.00, 293.00};
const float greyDiff[] = {151.00, 126.00, 145.00};

//Variable array to hold detected colour arrays
float colourArray[] = {0, 0, 0}; 

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 150 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

//Define colours
#define RED 1
#define GREEN 2
#define YELLOW 3
#define PURPLE 4
#define BLUE 5
#define BLACK 6

//Define turning times
#define TURN90RIGHT 400
#define TURN90LEFT 400
#define TURN180DEGREE 960

//Change the value of movespeed of each motor to better ensure it starts off moving in straight line
//If it moves slightly left implies you need to increase movespeed of left wheel (M1), vice versa.
int M1_movespeed = 130;
int M2_movespeed = 129;

int sensorState; //for detecting black line

//Music
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define melodyPin 3
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

//Make the mBot move forward
void Forward()
{
  //Serial.println("Going forward");
  MotorL.run(-M1_movespeed); //since the motor oriented in different direction, the movespeed should be different direction
  MotorR.run(M2_movespeed);
}

//Change the speed of motors to make the mBot turn slightly left
void TurnLeft()
{
  long t = millis(); 
  long t2 = t;
  while (t - t2 < 7) { //t: current time, t2 is initial time, loop for 9 ms
    Serial.println("Turning left");
    MotorL.run(-M1_movespeed * 0.80); //decrease the left motor speed 
    MotorR.run(M2_movespeed * 1.2); //increase the right motor speed 
    t = millis(); //update current time
  }
}

//Change the speed of motors to make the mBot turn slightly right
void TurnRight()
{ 
  long t = millis();
  long t2 = t;
  while ( t - t2 < 7) { //t: current time, t2 is initial time, loop for 9 ms
    Serial.println("Turning right");
    MotorL.run(-M1_movespeed * 1.2); //increase the left motor speed
    MotorR.run(M2_movespeed * 0.80); //decrease the right motor speed
    t = millis(); //update current time
  }
}

//Turn times are all determined via trial and error.
//Turn 90 degrees left
void Turn90Left()
{
  Serial.println("Turning 90 left");
  long t1 = millis();
  long t2 = t1;
  while (t1 - t2 < TURN90LEFT ) {
    MotorL.run(M1_movespeed * 0.8); //movespeed has same sign implies the motor rotates in different direction
    MotorR.run(M2_movespeed * 0.8);
    t1 = millis();
  }
  delay(200);
}

//Turn 90 degrees right
void Turn90Right()
{
  Serial.println("Turning 90 right");
  long t1 = millis();
  long t2 = t1;
  while (t1 - t2 < TURN90RIGHT ) {
    MotorL.run(-M1_movespeed * 0.8);
    MotorR.run(-M2_movespeed * 0.8);
    t1 = millis();
  }
  delay(200);
}

//Two successive left turns in two grids
void TurnLeft2() 
{ 
  Serial.println("Double turn left");
  Turn90Left();
  while (ultraSensor.distanceCm() > 8.5) { //Detects distance from front wall and decides when to take 2nd left turn
    Forward();
  }
  Turn90Left();
}

//Two successive right turns in two grids
void TurnRight2() 
{
  Serial.println("Double turn right");
  Turn90Right();
  while (ultraSensor.distanceCm() > 8.5) { //Detects distance from front wall and decides when to take 2nd right turn
    Forward();
  }
  Turn90Right();
}

//Turn 180 degrees
void Turn180() 
{
  Serial.println("Turn 180");
  long t = millis();
  long t2 = t;
  while (t - t2 < TURN180DEGREE) {
    MotorL.run(-M1_movespeed * 0.8);
    MotorR.run(-M2_movespeed * 0.8);
    t = millis();
  }
  delay(200);
}

//Stop the mBot
void Stop()
{
  Serial.println("Stop");
  MotorL.stop();
  MotorR.stop();
}

//Find the average reading for the requested number of times of scanning LDR
int getAvgReading(int times) {
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i += 1) {
    reading = lightSensor.read();
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total / times;
}

//Enables the LED flashes to detect colour and stores the values inside colourArray
void detectColour()  
{
  Serial.println("Detecting colour");
  led.setColor(255, 0, 0); //Set both LED to Red
  led.show();
  delay(RGBWait);
  //Get the average of 5 consecutive readings for the current colour and return an average
  colourArray[0] = getAvgReading(5);
  //The average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[0] = ((colourArray[0] - blackArray[0]) / (greyDiff[0])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  led.setColor(0, 255, 0); //Set both LED to Green
  led.show();
  delay(RGBWait);
  //Get the average of 5 consecutive readings for the current colour and return an average
  colourArray[1] = getAvgReading(5);
  //The average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[1] = ((colourArray[1] - blackArray[1]) / (greyDiff[1])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  led.setColor(0, 0, 255); //Set both LED to Blue
  led.show();
  delay(RGBWait);
  //Get the average of 5 consecutive readings for the current colour and return an average
  colourArray[2] = getAvgReading(5);
  //The average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[2] = ((colourArray[2] - blackArray[2]) / (greyDiff[2])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  for (int i = 0; i <= 2; i += 1)
  {
    Serial.print(int(colourArray[i]));
    Serial.print(" ");
  }
}

//Decides colour detected based on values of the colourArray. 
//Limits for each colour are determined experimentally.
int decideColour() { 
  Serial.println("Deciding colour");

  if (colourArray[0] <= 40 && colourArray[1] <= 40 && colourArray[2] <= 40)
  {
    return BLACK;
  }

  if (colourArray[1] >= 190 && colourArray[2] >= 190)
  {
    return BLUE;
  }
  if (colourArray[0] >= 90 && colourArray[1] >= 70 && colourArray[2] >= 140)
  {
    return PURPLE;
  }

  if (colourArray[0] >= 150)
  {
    if (colourArray[1] >= 100)
    {
      return YELLOW;
    }
    return RED;
  }

  if (colourArray[1] >= 80)
  {
    return GREEN;
  }
}

//Plays music
void sing() 
{
  Serial.println("YEET");
  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {

    // To calculate the note duration, take one second
    // divided by the note type.
    //E.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / tempo[thisNote];

    buzzer.tone(melody[thisNote], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // The note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Stop the tone playing:
    buzzer.tone(0, noteDuration);
  }
}

void setup()
{
  led.setpin(13);
  Serial.begin(9600); //In case we need to print something for debugging
  led.setColor(0, 0, 0);
  led.show();
  delay(2000); 

  //Calibrate initial IR values everytime bot is turned on.
  //Make sure bot is in the middle of the walls for accurate calibration.
  initialRight = ((analogRead(analogInPin1)) / 1023.0) * 5; //Right sensor
  initialLeft = ((analogRead(analogInPin2)) / 1023.0) * 5; //Left sensor
  Serial.println("correct values set");
}


void loop()
{
  double rightValue1 = ((analogRead(analogInPin1)) / 1023.0) * 5; //Right sensor
  double leftValue2 = ((analogRead(analogInPin2)) / 1023.0) * 5; //Left sensor

  //Detects black line for waypoint challenge
  sensorState = lineFinder.readSensors();
  
  //When detected black line, S1 and S2 sensor state is 'IN' 
  if (sensorState == S1_IN_S2_IN) {
    Stop();
    delay(200);

    detectColour();
    int colour = decideColour();
    Serial.print("Colour: ");
    Serial.print(colour);

    if (colour == RED) {
      Turn90Left();
    }
    else if (colour == GREEN) {
      Turn90Right();
    }
    else if (colour == YELLOW) {
      //180 turn
      Turn180();
    }
    else if (colour == PURPLE) {
      TurnLeft2();
    }
    else if (colour == BLUE) {
      TurnRight2();
    }

    else if (colour == BLACK) {
      Stop();
      sing();
      exit(0); //Ends the program 
    }

  }

  else //Do not do colour detection if waypoint challenge is not initiated.
  {
    Forward();

    /*IR proximity detection system to make sure bot is not too close to either walls.
      The further the distance, the larger the value.
      Hence, if the difference between the initial value and the current value for a particular side is too big,
      it is an indication that the bot is too close to that side.
      Values are determined experimentally. */
    while (initialLeft - leftValue2 >= 0.18) { //If bot is too close to the left wall, turn right slightly
      Serial.println("turn right slightly");
      TurnRight();
      rightValue1 = ((analogRead(analogInPin1)) / 1023.0) * 5;
      leftValue2 = ((analogRead(analogInPin2)) / 1023.0) * 5;
      Serial.print(rightValue1);
      Serial.print(" ");
      Serial.println(leftValue2);
    } 
    
    while (initialRight - rightValue1 >= 0.2 ) { //If bot is too close to the right wall, turn left slightly
      Serial.println("turn left slightly");
      TurnLeft();
      rightValue1 = ((analogRead(analogInPin1)) / 1023.0) * 5; //Converts from analog input value to voltage value 
      leftValue2 = ((analogRead(analogInPin2)) / 1023.0) * 5;
      Serial.print(rightValue1);
      Serial.print(" ");
      Serial.println(leftValue2);
    }
  } 

}
