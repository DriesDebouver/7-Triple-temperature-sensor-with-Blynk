#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//OLED:
Adafruit_SSD1306 display(128, 64, &Wire, -1);

//ADS:
Adafruit_ADS1115 ads(0x48);
float temp = 0.0;
float temp1 = 0.0;

//Dallas:
OneWire oneWire(15); //Pin 15
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1 = {  };
DeviceAddress sensor2 = {  };
float tempCEen;
float tempCTwee;

//WiFi:
int pin = 19;
char auth[] = "";
char ssid[] = "";
char pass[] = "";

void setup() {
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  Serial.begin(9600);
  sensors.begin();
  ads.begin();

  //OLED:
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  //WiFi:
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  sensors.requestTemperatures();
  tempCEen = sensors.getTempC(sensor1);
  tempCTwee = sensors.getTempC(sensor2);
  int16_t adc;
  adc = ads.readADC_SingleEnded(0);
  temp = (adc * 0.1875) / 1000;
  temp1 = temp * 10;
  Blynk.virtualWrite(V1, tempCEen);
  Blynk.virtualWrite(V2, tempCTwee);
  Blynk.virtualWrite(V3, temp1);

  //Serial monitor:
  Serial.print("Sensor 1 (°C): ");
  Serial.println(tempCEen);
  Serial.print("Sensor 2 (°C): ");
  Serial.println(tempCTwee);
  Serial.print("ADS KTY81 (°C): ");
  Serial.println(temp1);

  //OLED:
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temp 1: ");
  display.print(tempCEen);
  display.print((char)247); // ' ° ' symbol
  display.print("C");
  display.setCursor(0, 20);
  display.print("Temp 2: ");
  display.print(tempCTwee);
  display.print((char)247); // ' ° ' symbol
  display.print("C");
  display.setCursor(0, 40);
  display.print("ADS KTY81: ");
  display.print(temp1);
  display.print((char)247); // ' ° ' symbol
  display.print("C");
  display.display();
  delay(1000);
}
