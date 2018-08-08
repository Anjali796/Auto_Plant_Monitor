int sensorPin=A2;

void setup() {
  // initialize serial plotter
  Serial.begin(9600);
}

void loop() {

  //code for photodiode
  int sensorValue = 0; // variable to store the value coming from the sensor
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println();
  Serial.print("Photodiode value: ");
  Serial.println(sensorValue); //prints the values coming from the sensor on the screen

delay(3000);
}
