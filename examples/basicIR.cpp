
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
const int pinIR1 = 18;
const int pinIR2 = 21;
const int pinIR3 = 22;
const int pinIR4 = 23;
const int pinLed = 19;
void setup()
{
    Serial.begin(115200);
    pinMode(pinIR1, INPUT);
    pinMode(pinIR2, INPUT);
    pinMode(pinIR3, INPUT);
    pinMode(pinIR4, INPUT);
    pinMode(pinLed, OUTPUT);
    Serial.println("Deteksi Sensor IR");
    delay(3000);
}
void loop()
{
    int sensorState1 = digitalRead(pinIR1);
    if (sensorState1 == LOW)
    {
        // Serial.println("Tedeteksi");
        digitalWrite(pinLed, HIGH);
    }
    else
    {
        digitalWrite(pinLed, LOW);
    }

    // int sensorState2 = digitalRead(pinIR2);
    // if (sensorState2 == LOW)
    // {
    //   // Serial.println("Tedeteksi");
    //   digitalWrite(pinLed, HIGH);
    // }
    // else
    // {
    //   digitalWrite(pinLed, LOW);
    // }

    // int sensorState3 = digitalRead(pinIR3);
    // if (sensorState3 == LOW)
    // {
    //   // Serial.println("Tedeteksi");
    //   digitalWrite(pinLed, HIGH);
    // }
    // else
    // {
    //   digitalWrite(pinLed, LOW);
    // }

    // int sensorState4 = digitalRead(pinIR4);
    // if (sensorState4 == LOW)
    // {
    //   // Serial.println("Tedeteksi");
    //   digitalWrite(pinLed, HIGH);
    // }
    // else
    // {
    //   digitalWrite(pinLed, LOW);
    // }
    Serial.println(sensorState1);
    // Serial.print(sensorState2);
    // Serial.print(sensorState3);
    // Serial.println(sensorState4);
    delay(300);
}