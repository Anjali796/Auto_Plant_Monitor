//Libraries
#include <DHT.h>; //for temp/hum
#include <SD.h>; //for SD card
#include <math.h> //for thermistor
#include <SPI.h> 

File myFile; //create a variable of file type 

const int chipSelect = 10; //CS digital port 

//Constants
//DHT22
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht (DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Sparkfun Soil Moisture
int sr = 0; //value for storing moisture value
int soilPin = A0; //Declare a variable for the soil moisture sensor (analog port)
int soilPower = 7; //Variable for Soil moisture Power

//Thermistor
#define SERIESRESISTOR 10000 //resistor value
#define THERMISTORPIN A1 //analog pin connected to the thermistor

//Photoresistor
int sensorPin = A2; // select the input pin for LDR, analog

//formula for converting thermistor value to Celsius reading
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  return Temp;
}


//SETUP

void setup() {
  Serial.begin(9600);  // initialize serial plotter
  dht.begin(); 
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  File dataFile = SD.open("logger.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Photodiode Value,Soil Moisture,DHT Temp,DHT Hum,Thermistor Temp"); //header file of csv to be written every time setup is run
    dataFile.close();
  }
}

//LOOP

void loop() {

  //DHT22
  float hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");

  //Sparkfun soil moisture sensor
  Serial.print("Soil Moisture = ");
  //get soil moisture value from the function below and print it
  int val = readSoil();
  Serial.println(val);


  //Thermistor
  float  T = Thermistor(analogRead(1));

  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.print(" Celsius");
  Serial.println();

  //Photoresistor
  int sensorValue = 0; // variable to store the value coming from the sensor
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println();
  Serial.print("Photodiode value: ");
  Serial.println(sensorValue); //prints the values coming from the sensor on the screen


  //Printing onto SD card
  // make a string for assembling the data to log:
  String dataString = "";
  dataString += String(sensorValue);
  dataString += String(",");
  dataString += String(val);
  dataString += String(",");
  dataString += String(temp);
  dataString += String(",");
  dataString += String(hum);
  dataString += String(",");
  dataString += String(T);
  dataString += String(",");

  /* We do not specify with text strings what the values are for as the header line is already written in the setup
   The information is comma separated and when opened the values will be in columns under the appropriate header */

  // open the file. note that only one file can be open at a time (you have to close this one before opening another)
  File dataFile = SD.open("logger.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.print("As written on file: "); //This shows what is written on the csv file on the COM port so you can see if it's reasonable 
    Serial.println();
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening logger.csv");
  }


  delay(2000); //delay two seconds 
}

//Formula to read soil moisture level (only worked when placed at the end here) 

int readSoil()
{

  digitalWrite(soilPower, HIGH);//turn D7 "On"
  delay(10);//wait 10 milliseconds
  sr = analogRead(soilPin);//Read the SIG value form sensor
  digitalWrite(soilPower, LOW);//turn D7 "Off"
  return sr;//send current moisture value

}
