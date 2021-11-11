

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

const int pinPengunjungMasuk = 19;
const int pinPengunjungKeluar = 21;
const int pinAntrianMasuk = 22;
const int pinAntrianKeluar = 23;
bool signupOK = false;
int dataLocal = 0;
int jumlahPengunjung = 0;
int dataPengunjung = 0;
int dataAntrian = 0;
int batas = 0;
int data_firebase = 0;

void handleGetData()
{
    if (Firebase.RTDB.getInt(&fbdo, "pengunjung/jumlahSaatIni/total"))
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

    pinMode(pinPengunjungMasuk, INPUT);
    pinMode(pinPengunjungKeluar, INPUT);
    pinMode(pinAntrianMasuk, INPUT);
    pinMode(pinAntrianKeluar, INPUT);

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
    delay(1000);
}
void loop()
{

    Serial.println();
    Serial.println();
    Serial.println();
    if (Firebase.ready() && signupOK)
    {

        int sensorPengunjungMasuk = digitalRead(pinPengunjungMasuk);
        int sensorPengunjungKeluar = digitalRead(pinPengunjungKeluar);
        int sensorAntrianMasuk = digitalRead(pinAntrianMasuk);
        int sensorAntrianKeluar = digitalRead(pinAntrianKeluar);

        Serial.print("BATAS JUMLAH         :");
        Serial.println(batas);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.print("NILAI SENSOR1         :");
        Serial.println(digitalRead(sensorPengunjungMasuk));

        Serial.print("NILAI SENSOR2         :");
        Serial.println(digitalRead(sensorPengunjungKeluar));

        Serial.print("NILAI SENSOR3         :");
        Serial.println(digitalRead(sensorAntrianMasuk));

        Serial.print("NILAI SENSOR4         :");
        Serial.println(digitalRead(sensorAntrianKeluar));

        Serial.println();
        Serial.println();
        Serial.println();

        Serial.print("DATA PRE FIREBASE 1      :");
        Serial.print(dataPengunjung);
        Serial.print(dataAntrian);

        // PENGUNJUNG MASUK
        if (dataPengunjung < batas)
        {
            if (sensorPengunjungMasuk == LOW)
            {
                dataPengunjung = dataPengunjung + 1;
                jumlahPengunjung = jumlahPengunjung + 1;

                if (Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/jumlahSaatIni/total", dataPengunjung))
                {
                    Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/jumlahPengunjung/total", jumlahPengunjung);
                    // Serial.println("BERHASUL pengunjungMasuk");
                }
                else
                {
                    Serial.println("GAGAL pengunjungMasuk");
                    Serial.println(fbdo.errorReason());
                }
            }
        }
        else
        {
            Serial.println("PENGUNJUNG TELAH MENCAPAI BATAS");
            Serial.println(batas);
        }

        // PENGUNJUNG KELUAR
        if (sensorPengunjungKeluar == LOW)
        {

            dataPengunjung = dataPengunjung - 1;
            if (dataPengunjung < 0)
            {
                dataPengunjung = 0;
            }

            if (Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/jumlahSaatIni/total", dataPengunjung))
            {
                Firebase.RTDB.setIntAsync(&fbdo, "pengunjung/triggerNotif", dataPengunjung);
                // Serial.println("BERHASIL pengunjungKeluar");
            }
            else
            {
                Serial.println("GAGAL pengunjungKeluar");
                Serial.println(fbdo.errorReason());
            }
        }

        // ANTRIAN MASUK
        if (dataPengunjung > 0)
        {
            if (sensorAntrianMasuk == LOW)
            {
                if (dataAntrian < dataPengunjung)
                {
                    dataAntrian = dataAntrian + 1;
                    Firebase.RTDB.setIntAsync(&fbdo, "antrian/jumlahSaatIni/total", dataAntrian);
                    // Serial.println("BERHASIL antrianMasuk");
                }
                else
                {
                    Serial.println("GAGAL antrianMasuk");
                    Serial.println(fbdo.errorReason());
                }
            }
        }

        // ANTRIAN KELUAR
        if (sensorAntrianKeluar == LOW)
        {

            dataAntrian = dataAntrian - 1;
            if (dataAntrian < 0)
            {
                dataAntrian = 0;
            }
            if (Firebase.RTDB.setIntAsync(&fbdo, "antrian/jumlahSaatIni/total", dataAntrian))
            {
                // Serial.println("BERHASIL antrianKeluar");
            }
            else
            {
                Serial.println("GAGAL antrianKeluar");
                Serial.println(fbdo.errorReason());
            }
        }
    }
    Serial.println();
    Serial.println("=============================");
    delay(300);
}
