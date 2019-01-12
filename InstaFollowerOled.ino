#include "InstagramStats.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "JsonStreamingParser.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);


char ssid[] = "XXX";       // your network SSID (name)
char password[] = "XXX";  // your network key

WiFiClientSecure client;
InstagramStats instaStats(client);

String userName = "maker.moekoe";
int last_followers = 0;



void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(100);

  PrintText("CONNECTING", "WIFI");
  delay(2000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  String dot = "";
  while (WiFi.status() != WL_CONNECTED) {
    dot += ".";
    PrintText("CONNECTING", dot);
    delay(300);
  }

  IPAddress ip = WiFi.localIP();
  PrintText("Connected!");

}

long t = millis();

void loop() {
  if (millis() > t + 1000 || millis() < t) {
    InstagramUserStats response = instaStats.getUserStats(userName);
    PrintText("INSTAGRAM", "FOLLOWER");
    delay(1500);
    if (last_followers < response.followedByCount) {
      for (int i = 0; i < 5; i++) {
        PrintText("NEW ONE");
        delay(150);
        display.clearDisplay();
        display.display();
        delay(100);
      }
      last_followers = response.followedByCount;
    }
    
    PrintText(String(response.followedByCount));
    t = millis();
  }

}





void PrintText(String txt1) {
  //Zeigt den angegebenen Text mittig auf dem OLED bildschirm an (eine Zeile)

  int g;
  int n = txt1.length();

  g = display.width() / (6 * n);
  if (g > 6) g = 6;

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(g);
  display.setCursor(X(g, n), Y(g, 0.5));
  display.println(txt1);
  display.display();
}//end void PrintText

void PrintText(String txt1, String txt2) {
  //Zeigt den angegebenen Text mittig auf dem OLED bildschirm an (zwei Zeilen)

  int g;
  int n1 = txt1.length();
  int n2 = txt2.length();

  if (n1 > n2) g = display.width() / (6 * n1);
  else g = display.width() / (6 * n2);

  if (g > 4) g = 4;

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(g);
  display.setCursor(X(g, n1), Y(g, 0.25));
  display.println(txt1);
  display.setCursor(X(g, n2), Y(g, 0.75));
  display.println(txt2);
  display.display();
}//end void PrintText

void PrintTextFor(String txt1, int time) {
  //Zeigt den angegebenen Text mittig auf dem OLED bildschirm an für eine bestimmte Zeit (eine Zeile)

  PrintText(txt1);
  delay(time * 1000);
}//end void PrintText

void PrintTextFor(String txt1, String txt2, int time) {
  //Zeigt den angegebenen Text mittig auf dem OLED bildschirm an für eine bestimmte Zeit (zwei Zeilen)

  PrintText(txt1, txt2);
  delay(time * 1000);
}//end void PrintText


int X(int textgroesse, int n) {
  //gibt die X koordinate aus, damit text mit n zeichen mittig ist

  return (0.5 * (display.width() - textgroesse * (6 * n - 1)));
}//end int X

int Y(int textgroesse, float f) {
  //gibt die Y koordinate aus, damit text mittig ist

  return (f * display.height() - (textgroesse * 4));
}//end int Y
