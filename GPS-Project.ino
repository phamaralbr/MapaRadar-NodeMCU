//OLED DISPLAY - SCK/D1 SDA/D2 VDD/3V
//GPS - TX/D6 RX/D7? pq so foi desconectado?
//BUZZER - POSITIVE/D5

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FS.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const int speakerPin = 14; //D4
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ss.begin(GPSBaud);
  Serial.begin(115200);

  if (SPIFFS.begin()) {
    Serial.println("SPIFFS Started");
  } else {
    Serial.println("SPIFFS Failed");
  }

  drawBoot();

}

void loop() {
  findCloseRadar();
  smartDelay(500);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


void drawBoot(void) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0);
  display.setTextSize(2);
  display.println(F("Alerta"));
  display.setCursor(30, 20);
  display.println(F("Radar"));
  display.setTextSize(1);
  display.setCursor(1, 50);
  display.println(F("Powered by MapaRadar"));
  display.display();
  delay(3000);
}

char* spinner = "/-\\|";
byte screenRefreshSpinnerPos = 0;

void displayinfo()
{
  screenRefreshSpinnerPos = (screenRefreshSpinnerPos + 1) % 4;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(spinner[screenRefreshSpinnerPos]);
  display.print("LAT: ");
  display.println(gps.location.lat(), 6);
  display.print("LON: ");
  display.println(gps.location.lng(), 6);
  //TIME GMT-3
  if (gps.time.hour() <= 3) {
    display.print(21 + gps.time.hour());
  } else {
    display.print(gps.time.hour() - 3);
  }
  display.print(":");
  display.print(gps.time.minute());
  display.print(":");
  display.println(gps.time.second());
  display.print("SATS: ");
  display.println(gps.satellites.value());
  display.print("VEL: ");
  display.print(gps.speed.kmph(), 0);
  display.println(" km/h");
  display.print("DIR: ");
  display.print(gps.course.deg(), 2);
  display.println(" deg.");
  display.display();

}

struct RadarData {
  double lat;
  double lon;
  byte type;
  byte speed;
  byte dirtype;
  short direction;
};

void findCloseRadar() {

  displayinfo();

  File file = SPIFFS.open("/" + String(int(gps.location.lng())) + String(int(gps.location.lat())) + ".txt", "r");
  if (!file) {
    Serial.println("Failed to open /" + String(int(gps.location.lng())) + String(int(gps.location.lat())) + ".txt");
  }

  while (file.available()) {
    String radar = file.readStringUntil('\n');
    int str_len = radar.length() + 1;
    char char_array[str_len];
    radar.toCharArray(char_array, str_len);
    struct RadarData RadarInstance = parseLine(char_array);

    double distanceMt = gps.distanceBetween(
                          gps.location.lat(),
                          gps.location.lng(),
                          RadarInstance.lat,
                          RadarInstance.lon);

    Serial.println(distanceMt);

    //Beep if close, in the same direction and above speed limit

    while (distanceMt < 100) {
      smartDelay(500);
      distanceMt = gps.distanceBetween(
                          gps.location.lat(),
                          gps.location.lng(),
                          RadarInstance.lat,
                          RadarInstance.lon);
                          
      displayinfo();

      if ( gps.speed.kmph() >= RadarInstance.speed ){
        if ( distanceMt < 25 ) {
        sound_alert(100);
      } else if ( distanceMt < 50 ) {
        sound_alert(200);
      } else if ( distanceMt < 75 ) {
        sound_alert(300);
      } else if ( distanceMt < 100 ) {
        sound_alert(400);
      } else {
      }
      }

      
    }


  }
  file.close();
}

void sound_alert(int pitch) {
  tone(speakerPin, 900);
  delay(pitch);
  noTone(speakerPin);
  delay(pitch);
  noTone(speakerPin);
}


struct RadarData parseLine( char *line )
{

  struct RadarData RadarInstance;
  char *field;

  field       = strtok( line, "," );
  RadarInstance.lon = atof( field );
  field       = strtok( nullptr, "," );
  RadarInstance.lat    = atof( field );
  field       = strtok( nullptr, "," );
  RadarInstance.type      = atof( field );
  field       = strtok( nullptr, "," );
  RadarInstance.speed     = atof( field );
  field       = strtok( nullptr, "," );
  RadarInstance.dirtype   = atof( field );
  field       = strtok( nullptr, "," );
  RadarInstance.direction   = atof( field );

  return RadarInstance;
}
