/* Program : IOT - PV HMI in Node-RED 
 * Membaca V,I dan P dari PLTA serta V1 dan V2
 * Mengatur hidup/mati LED ESP32
 * Mengatur kecerahan LED 
 * © Eko M.B
 * © Abdul Ghani
 */

// pustaka
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TFPeriodic.h>
#include <Button.h>

#define BAUD 500000
#define PROG_NAME "IOT - Photo Voltage"
#define PROG_VERSION  "1.0"

#define NODE "K1-03" //Ganti dengan Regu/NIM anda
