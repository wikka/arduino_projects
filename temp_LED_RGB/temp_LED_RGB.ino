/*
 * Original modified by Leslie Wittig Quintanilla  
 * lquintanilla at gmail dot com
 *
 *     ---------------------------------------------------------
 *     |  Arduino Experimentation Kit Example Code             |
 *     |  CIRC-10 .: Temperature :. (TMP36 Temperature Sensor) |
 *     ---------------------------------------------------------
 *   
 *  A simple program to output the current temperature to the IDE's debug window 
 * 
 *  For more details on this circuit: http://tinyurl.com/c89tvd 
 */

//TMP36 Pin Variables
int temperaturePin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade 
                        //(500 mV offset) to make negative temperatures an option
int ledAnalogOne[] = {9, 10, 11}; //RGB 

/*
const byte DARKRED[] = {164, 8, 8}; 
const byte RED[] = {255, 0, 0}; 
const byte ORANGE[] = {83, 4, 0}; 
const byte YELLOW[] = {255, 255, 0}; 
const byte GREEN[] = {0, 255, 0}; 
const byte BLUESKY[] = {173, 175, 255}; 
const byte BLUE[] = {0, 0, 255}; 
const byte BLUEMOON[] = {0, 0, 100}; 
const byte INDIGO[] = {4, 0, 19}; 
const byte VIOLET[] = {23, 0, 22}; 
const byte CYAN[] = {0, 255, 255}; 
const byte MAGENTA[] = {255, 0, 255}; 
const byte WHITE[] = {255, 255, 255}; 
const byte BLACK[] = {0, 0, 0}; 
const byte PINK[] = {158, 4, 79}; 
*/
int MHex=255; //Last color number
int i=21; //initial temperature value
int f=25; //final temperature value
float M=(i+f)/2; //this value indicate the warm temperature.

byte tempCor[]={0,0,MHex};
byte tempCorAnt[]={tempCor[0],tempCor[1],tempCor[2]};
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  for(int i = 0; i < 3; i++){
   pinMode(ledAnalogOne[i], OUTPUT);   //Set the three LED pins as outputs
  }
  //setColor(ledAnalogOne, BLACK);
  Serial.begin(9600);  //Start the serial connection with the copmuter
                       //to view the result open the serial monitor 
                       //last button beneath the file bar (looks like a box with an antenae)
}

void loop()                     // run over and over again
{
  float temperature = getVoltage(temperaturePin);  //getting the voltage reading from the temperature sensor
  temperature = (temperature - .5) * 100;          //converting from 10 mv per degree wit 500 mV offset
                                                  //to degrees ((volatge - 500mV) times 100)
  Serial.print(temperature);                     //printing the result
  
  //lets make the color
  tempCor[0]=vermelho(temperature); //how red
  tempCor[1]=verde(temperature); //how green
  tempCor[2]=azul(temperature); //how blue

  fadeToColor(ledAnalogOne, tempCorAnt, tempCor, 10);

  //keep the last color to use in the fading
  tempCorAnt[0]=tempCor[0]; 
  tempCorAnt[1]=tempCor[1]; 
  tempCorAnt[2]=tempCor[2];
  
  delay(1000);
}

/*
 * getVoltage() - returns the voltage on the analog input defined by
 * pin
 */
float getVoltage(int pin){
 return (analogRead(pin) * .004882814); //converting from a 0 to 1023 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}

void setColor(int* led, byte* color){
 for(int i = 0; i < 3; i++){             //iterate through each of the three pins (red green blue)
   analogWrite(led[i], 255 - color[i]);  //set the analog output value of each pin to the input value (ie led[0] (red pin) to 255- color[0] (red input color)
                                         //we use 255 - the value because our RGB LED is common anode, this means a color is full on when we output analogWrite(pin, 0)
                                         //and off when we output analogWrite(pin, 255). 
 }
}

void setColor(int* led, const byte* color){
 byte tempByte[] = {color[0], color[1], color[2]};
 setColor(led, tempByte);
}

void fadeToColor(int* led, byte* startColor, byte* endColor, int fadeSpeed){
  int changeRed = endColor[0] - startColor[0];                            //the difference in the two colors for the red channel
  int changeGreen = endColor[1] - startColor[1];                          //the difference in the two colors for the green channel 
  int changeBlue = endColor[2] - startColor[2];                           //the difference in the two colors for the blue channel
  int steps = max(abs(changeRed),max(abs(changeGreen), abs(changeBlue))); //make the number of change steps the maximum channel change
  
  for(int i = 0 ; i < steps; i++){                                        //iterate for the channel with the maximum change
   byte newRed = startColor[0] + (i * changeRed / steps);                 //the newRed intensity dependant on the start intensity and the change determined above
   byte newGreen = startColor[1] + (i * changeGreen / steps);             //the newGreen intensity
   byte newBlue = startColor[2] + (i * changeBlue / steps);               //the newBlue intensity
   byte newColor[] = {newRed, newGreen, newBlue};                         //Define an RGB color array for the new color
   setColor(led, newColor);                                               //Set the LED to the calculated value
   delay(fadeSpeed);                                                      //Delay fadeSpeed milliseconds before going on to the next color
  }
  setColor(led, endColor);                                                //The LED should be at the endColor but set to endColor to avoid rounding errors
}

void fadeToColor(int* led, const byte* startColor, const byte* endColor, int fadeSpeed){
  byte tempByte1[] = {startColor[0], startColor[1], startColor[2]};
   byte tempByte2[] = {endColor[0], endColor[1], endColor[2]};
   fadeToColor(led, tempByte1, tempByte2, fadeSpeed);
}

//RED - Hot
int vermelho(float val){
  float x=val;
  if(val <= M){
    x=0;
  }else if(val >= f){
    x=255;
  }else{
    x=((val-M)*MHex)/(f-M);
  }
  return int(x);
}

//GREEN - warm
int verde(float val){
  float x=val;
  if(val <= i){
    x=0;
  }else if(val >= f){
    x=0;
  }else if(i < val && val <= M){
    x=((val-i)*MHex)/(M-i);
  }else{
    x=((val-f)*MHex)/(M-f);
  }
  return int(x);
}

//BLUE - cold
int azul(float val){
  float x=val;
  if(val <= i){
    x=255;
  }else if(val >= M){
    x=0;
  }else{
    x=((val-M)*MHex)/(i-M);
  }
  return int(x);
}

