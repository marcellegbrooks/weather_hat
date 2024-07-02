// full hat temp + rain


// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>
#include <Stepper.h>

#define STEPS_PER_MOTOR_REVOLUTION 32
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048  


int Steps2Take;
int steps2take;

int upordown = 1; // 0 down, 1 up

// initialize the stepper library on pins 8 through 11:
Stepper glasses_Stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);
Stepper hat_Stepper(STEPS_PER_MOTOR_REVOLUTION, 6, 5, 4, 3);

// Create an instance of the DHT11 class.
DHT11 dht11(7);

void setup()
{
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    delay(5000);
    hat_Stepper.setSpeed(900);
    Serial.println("setup()");
}

void loop()
{
  // raindrop sensor
  int sensorReading = analogRead(A0);
  Serial.println(sensorReading); // debugging 
  if (sensorReading <= 600){
    Serial.println("RAIN"); // debugging
    Steps2Take  =  STEPS_PER_OUTPUT_REVOLUTION * 6;
    glasses_Stepper.setSpeed(900);
    glasses_Stepper.step(Steps2Take);
    delay(1000);
  } else{
    Serial.println("No RAIN");
  }

    // Attempt to read the temperature value from the DHT11 sensor.
    int temperature = dht11.readTemperature();

    // Check the result of the reading.
    // If there's no error, print the temperature value.
    // If there's an error, print the appropriate error message.
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT)
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
        if (upordown == 0){ // if flaps up
          if (temperature <= 20){ // put flaps down
            delay(5000);
            steps2take  =  - STEPS_PER_OUTPUT_REVOLUTION * 6; 
            hat_Stepper.step(steps2take);
            Serial.println("working clockwise");
            upordown = 1;
          }
        } else { // if flaps down
          if (temperature > 20){ // put flaps up
            delay(5000);
            steps2take  =  STEPS_PER_OUTPUT_REVOLUTION * 6; 
            hat_Stepper.step(steps2take); // counterclockwise
            Serial.println("working counterclockwise");
            upordown = 0;
          }
        }
        
    }
    else
    {
        Serial.println(DHT11::getErrorString(temperature));
    }

    // Wait for 2 seconds before the next reading.
    delay(2000);
}
