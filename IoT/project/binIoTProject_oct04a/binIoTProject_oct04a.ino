
#include "arduino_secrets.h"
/* 
  
  Arduino IoT Cloud Variables

  float occupancy_rate;
  CloudColoredLight LedStatus;
  CloudColoredLight yellowLedStatus;
  int noUsage;

*/

#include "thingProperties.h"
#include <ESP32Servo.h>  // Include the servo library
#include <EMailSender.h> // For sending emails


// Define pins and thresholds

const int trigPinCapacity = 25;
const int echoPinCapacity = 26;
const int trigPinHand = 13;
const int echoPinHand = 14;
const int servoPin = 12;
const int redLedPin = 4;
const int yellowLedPin = 16;
const int greenLedPin = 17;


// Thresholds for determining LED color and servo control
const int redThreshold = 6;
const int yellowThreshold = 12;
const int greenThreshold = 20;
const int handDistanceThreshold = 20;
const int errorThreshold = -1;



#define SENDER_EMAIL "pythonista.imran@gmail.com"
#define SENDER_PASSWORD "olvk xheq jhvu qeai"
#define RECEIVER_EMAIL "kaziimran072@gmail.com"

EMailSender emailSend(SENDER_EMAIL, SENDER_PASSWORD);


// Servo motor object and state tracker
Servo myservo;
bool servoAttached = false;
bool emailAlert = false;
 
void setup() {
  initializePins();
  initializeSerial();
  
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

//float distance_capacity;

void loop() {
  ArduinoCloud.update();
  int distance_capacity = measureDistance(trigPinCapacity, echoPinCapacity);
  //float o_rate = measureOccupancy(distance_capacity);
  // Control LEDs based on distance measured by the first ultrasonic sensor
  controlLEDs(distance_capacity);
  // Calcualte occupancy rate based on inside-distance of the bi
  measureOccupancy(distance_capacity);
  // Control the Servo motor based on distance measured by the second ultrasonic sensor
  controlServo(measureDistance(trigPinHand, echoPinHand));
  // Pause for a short while before the next iteration
  delay(500);
}

void initializePins() {
  // Set pin modes for ultrasonic sensors and LEDs
  pinMode(trigPinCapacity, OUTPUT);
  pinMode(echoPinCapacity, INPUT);
  pinMode(trigPinHand, OUTPUT);
  pinMode(echoPinHand, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  // Attach the servo motor
  myservo.attach(servoPin);
}

void initializeSerial() {
  // Start serial communication for debugging
  Serial.begin(9600);
}

float measureOccupancy(float distance) {
  Serial.print("Inside Distance: ");
  Serial.println(distance);
  occupancy_rate = (1-distance / 23) * 100; // Calculate occupancy_rate
  Serial.print("occupancy_rate: ");
  Serial.println(occupancy_rate);
  return occupancy_rate;
}

int measureDistance(int trigPin, int echoPin) {
  // Trigger the ultrasonic sensor to send a pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Measure how long the returned pulse takes
  long duration = pulseIn(echoPin, HIGH, 30000);
  // Log an error if measurement times out
  if (duration == 0) {
    Serial.println("Error: Distance measurement timeout");
    return errorThreshold;
  }
  // Convert pulse duration to distance in cm
  int distance = duration * 0.034 / 2;
  // Log the measured distance
  Serial.print("Measured Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}



void controlLEDs(int distance) {
  int o_rate = measureOccupancy(distance);;
  //measureOccupancy(measureDistance(trigPinCapacity, echoPinCapacity));
  // Turn off all LEDs initially
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  // Control LEDs based on measured distance, or log error if distance is invalid
  if (distance == errorThreshold) {
    Serial.println("Error: Invalid distance for LED control");
    activateAllLEDs();
  } else if (distance < redThreshold) {
    digitalWrite(redLedPin, HIGH);
    LedStatus.setSwitch(true);
    LedStatus.setHue(0);           // Set hue to 0 (red)
    LedStatus.setSaturation(100);  // Set saturation to 100%
    LedStatus.setBrightness(o_rate);  // Set brightness to 100%
    emailAlert = true;
    //sendEmail(recipient, "Red Light Alert", "The red light is on!");
    
    EMailSender::EMailMessage message;
    message.subject = "Bin Status Alert";
    message.message = "Hello, the bin is almost full. Please replace it!";

    EMailSender::Response resp = emailSend.send(RECEIVER_EMAIL, message);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
    
  } else if (distance < yellowThreshold) {
    digitalWrite(yellowLedPin, HIGH);
    LedStatus.setSwitch(true);
    LedStatus.setHue(60);           // Set hue to 60 (yellow)
    LedStatus.setSaturation(100);  // Set saturation to 100%
    LedStatus.setBrightness(o_rate);
    emailAlert = false;
  } else {
    digitalWrite(greenLedPin, HIGH);
    LedStatus.setSwitch(true);
    LedStatus.setHue(120);          // Set hue to 120 (green)
    LedStatus.setSaturation(100);  // Set saturation to 100%
    LedStatus.setBrightness(o_rate);
    emailAlert = false;
  }
}

void activateAllLEDs() {
  // Turn on all LEDs (e.g., to signal an error)
  digitalWrite(redLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
}

void controlServo(int distance) {
  // Control the servo based on measured distance, or do nothing if distance is invalid
  if (distance < handDistanceThreshold && distance != errorThreshold) {
    if (!servoAttached) {
      myservo.attach(servoPin);
      servoAttached = true;
    }
    // Open the servo, wait, close it, and wait again
    myservo.write(90);
    delay(3000);
    myservo.write(0);
    noUsage++;
    delay(1000);
  } else if (servoAttached) {
    // Detach the servo to save energy when not in use
    myservo.detach();
    servoAttached = false;
  }
}















