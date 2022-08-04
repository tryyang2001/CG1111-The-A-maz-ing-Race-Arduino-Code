/**
 * Code for Colour Sensor Calibration
 * 
 * To get values of whiteArray, blackArray and greyDiff 
 * 
 */


#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

MeRGBLed led(0, 30); //Inbuilt LED
MeLightSensor lightSensor(PORT_8); //Inbuilt Light Sensor

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 250 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

//floats to hold colour arrays, in RGB
float whiteArray[] = {0, 0, 0};
float blackArray[] = {0, 0, 0};
float greyDiff[] = {0, 0, 0};

//Find the average reading for the requested number of times of scanning LDR
int getAvgReading(int times) {
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++) {
    reading = lightSensor.read();
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total / times;
}

//Calibration of white and black balance
void setBalance() {

  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
  led.setColor(0, 0, 0); //turns LED off
  led.show();
  //scan the white sample.
  //go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  //set max red
  led.setColor(255, 0, 0);
  led.show();
  delay(RGBWait);
  whiteArray[0] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //set max green
  led.setColor(0, 255, 0);
  led.show();
  delay(RGBWait);
  whiteArray[1] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //set max blue
  led.setColor(0, 0, 255);
  led.show();
  delay(RGBWait);
  whiteArray[2] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  //set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready
  //go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  //set max red
  led.setColor(255, 0, 0);
  led.show();
  delay(RGBWait);
  blackArray[0] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //set max green
  led.setColor(0, 255, 0);
  led.show();
  delay(RGBWait);
  blackArray[1] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //set max red
  led.setColor(0, 0, 255);
  led.show();
  delay(RGBWait);
  blackArray[2] = getAvgReading(15);         //scan 15 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  //the difference between the maximum and the minimum gives the range
  for (int i = 0; i <= 2; i += 1)
  {
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }



  //Print whiteArray, blackArray, greyDiff
  for (int i = 0; i <= 2; i += 1)
  {
    Serial.print(whiteArray[i]);
    Serial.print(" ");
  }

  Serial.println("");
  for (int i = 0; i <= 2; i += 1)
  {
    Serial.print(blackArray[i]);
    Serial.print(" ");
  }
  
  Serial.println("");
  for (int i = 0; i <= 2; i += 1)
  {
    Serial.print(greyDiff[i]);
    Serial.print(" ");
  }



}

void setup() {
  led.setpin(13);
  Serial.begin(9600);
  led.setColor(0, 0, 0);
  led.show();
  setBalance();

}

void loop() {
  // put your main code here, to run repeatedly:

}
