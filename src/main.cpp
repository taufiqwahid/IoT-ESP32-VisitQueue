

/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/

#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Not Wifi"
#define WIFI_PASSWORD "tidakada00"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBNLDRrSqo01NqJMrr7tO2h2fy-VswcLHA"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://visitqueue-c0d40-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
int dataLocal = 0;
int dataPengunjungMasuk = 0;
int dataPengunjungKeluar = 0;
int dataAntrianMasuk = 0;
int dataAntrianKeluar = 0;
int batas = 0;
int data_firebase = 0;

void handleGetData()
{
    if (Firebase.RTDB.getInt(&fbdo, "pengunjung/pengunjungMasuk/total"))
    {
        if (fbdo.dataType() == "int")
        {
            data_firebase = fbdo.intData();
            Serial.print("DATA FIRBASE             :");

            Serial.println(data_firebase);
        }
    }
    else
    {
        Serial.println(fbdo.errorReason());
        // handleGetData();
    }
}

void handleGetLimitData()
{
    if (Firebase.RTDB.getInt(&fbdo, "pengunjung/batas"))
    {
        if (fbdo.dataType() == "int")
        {
            batas = fbdo.intData();
            Serial.print("DATA BATAS           :");
            Serial.println(batas);
        }
    }
    else
    {
        Serial.println(fbdo.errorReason());
        // handleGetLimitData();
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(18, INPUT);
    pinMode(19, INPUT);
    pinMode(21, INPUT);
    pinMode(22, INPUT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Sign up */
    if (Firebase.signUp(&config, &auth, "", ""))
    {
        Serial.println("ok");
        signupOK = true;
    }
    else
    {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    handleGetLimitData();
    Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/status_batas", false);
}
void loop()
{

    Serial.println();
    Serial.println();
    Serial.println();
    if (Firebase.ready() && signupOK)
    {

        // Serial.print("NILAI SENSOR         :");
        // Serial.println(digitalRead(22));

        // Serial.print("DATA PRE FIREBASE 1      :");
        // Serial.println(dataLocal);

        // int sensorPengunjungMasuk = digitalRead(18);
        // int sensorPengunjungKeluar = digitalRead(19);
        // int sensorAntrianMasuk = digitalRead(21);
        int sensorAntrianKeluar = digitalRead(22);

        if (dataPengunjungMasuk <= batas)
        {
            // if (sensorPengunjungMasuk == 0)
            // {
            //     dataPengunjungMasuk = dataPengunjungMasuk + 1;
            //     if (Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/pengunjungMasuk/total", dataPengunjungMasuk))
            //     {
            //         Serial.println("BERHASUL pengunjungMasuk");
            //     }
            //     else
            //     {
            //         Serial.println("GAGAL pengunjungMasuk");
            //         Serial.println(fbdo.errorReason());
            //     }
            // }

            // if (sensorPengunjungKeluar == 0)
            // {
            //     dataPengunjungKeluar = dataPengunjungKeluar + 1;
            //     dataPengunjungMasuk = dataPengunjungMasuk - 1;
            //     if (Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/pengunjungKeluar/total", dataPengunjungKeluar))
            //     {
            //         Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/pengunjungMasuk/total", dataPengunjungMasuk);
            //         Serial.println("BERHASIL pengunjungKeluar");
            //     }
            //     else
            //     {
            //         Serial.println("GAGAL pengunjungKeluar");
            //         Serial.println(fbdo.errorReason());
            //     }
            // }

            // if (sensorAntrianMasuk == 0)
            // {
            //     dataAntrianMasuk = dataAntrianMasuk + 1;
            //     if (Firebase.RTDB.setIntAsync(&fbdo, "antrian/antrianMasuk/total", dataAntrianMasuk))
            //     {
            //         Serial.println("BERHASIL antrianMasuk");
            //     }
            //     else
            //     {
            //         Serial.println("GAGAL antrianMasuk");
            //         Serial.println(fbdo.errorReason());
            //     }
            // }

            if (sensorAntrianKeluar == 0 && dataAntrianMasuk < 0)
            {
                dataAntrianKeluar = dataAntrianKeluar + 1;
                dataAntrianMasuk = dataAntrianMasuk - 1;
                if (Firebase.RTDB.setIntAsync(&fbdo, "antrian/antrianKeluar/total", dataAntrianKeluar))
                {
                    Firebase.RTDB.setIntAsync(&fbdo, "antrian/antrianMasuk/total", dataAntrianMasuk);
                    Serial.println("BERHASIL antrianKeluar");
                }
                else
                {
                    Serial.println("GAGAL antrianKeluar");
                    Serial.println(fbdo.errorReason());
                }
            }
        }
    }

    Serial.print("DATA PRE FIREBASE 2      :");
    Serial.println(dataAntrianKeluar);
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println("=============================");
    delay(500);
}
