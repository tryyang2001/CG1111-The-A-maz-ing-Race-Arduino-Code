/**
 * Code for Colour Range Calibration
 * 
 * To get the lower limit values for each colour.
 * Values of whiteArray, blackArray and greyDiff are pre-determined using "Colour_Sensor_Calibration".
 * 
 */

#include <MeMCore.h>

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 250 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 30 //in milliseconds 

//Define colours
#define RED 1
#define GREEN 2
#define YELLOW 3
#define PURPLE 4
#define BLUE 5
#define BLACK 6


MeRGBLed led(0, 30);
MeLightSensor lightSensor(PORT_8);

//Floats to hold colour arrays
float colourArray[] = {0, 0, 0}; //in RGB
const float whiteArray[] = {496.00, 381.00, 438.00};
const float blackArray[] = {345.00, 255.00, 293.00};
const float greyDiff[] = {151.00, 126.00, 145.00};


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

//Function to decide colour detected based on values of the colourArray. 
//Limits for each colour are determined experimentally.
int decideColour(float colourArray[]) {
  
   Serial.println("Deciding colour");

 if (colourArray[0] <= 15 && colourArray[1] <= 15 && colourArray[2] <= 15)
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

void setup()
{
  led.setpin(13);
  Serial.begin(9600);
  led.setColor(0, 0, 0);
  led.show();
}


void loop()
{
  led.setColor(255, 0, 0); //Set both LED to Red
  led.show();
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[0] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[0] = ((colourArray[0] - blackArray[0]) / (greyDiff[0])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  led.setColor(0, 255, 0); //Set both LED to Green
  led.show();
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[1] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[1] = ((colourArray[1] - blackArray[1]) / (greyDiff[1])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  led.setColor(0, 0, 255); //Set both LED to Blue
  led.show();
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[2] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[2] = ((colourArray[2] - blackArray[2]) / (greyDiff[2])) * 255;
  led.setColor(0, 0, 0); //Set both LED to White
  led.show();
  delay(RGBWait);

  for (int i = 0; i <= 2; i += 1)
  {
    Serial.print(int(colourArray[i]));
    Serial.print(" ");
  }
  
  Serial.print(decideColour(colourArray));
  Serial.println(" ");
  delay(200);

}
