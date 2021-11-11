#include <Arduino.h>
int data = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(22, INPUT);
}

void loop()
{
    int sensor = digitalRead(22);
    Serial.print("DATA SENSOR :");
    Serial.println(sensor);

    // put your main code here, to run repeatedly:
    if (sensor == 0)
    {
        if (data == 10)
        {
            Serial.print("SUDAH MELEBIHI BATAS");
        }
        else
        {
            data = data + 1;
        }
    }
    Serial.println(data);

    delay(500);
}