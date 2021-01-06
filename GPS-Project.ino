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

const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0xc0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x0f, 0xf0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x1f, 0xf8, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x3f, 0xfc, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x7e, 0x7e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x78, 0x1e, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0xf8, 0x1f, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xf0, 0x0f, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0xf0, 0x0f, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0xf8, 0x1f, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x7c, 0x3e, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x7f, 0xfe, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x3f, 0xfc, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x1f, 0xf8, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x0f, 0xf0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x01, 0xf0, 0x0f, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xf8, 0x1f, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x7c, 0x3f, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x7e, 0x3e, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x3e, 0x7c, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x1f, 0xf8, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0xf8, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x0f, 0xf0, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x07, 0xe0, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x83, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x0f, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00
};

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
  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE);
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

    //Beep if close, in the same direction and above speed limit
    while (distanceMt < 100) {//and direction TODO
      smartDelay(200);
      distanceMt = gps.distanceBetween(
                          gps.location.lat(),
                          gps.location.lng(),
                          RadarInstance.lat,
                          RadarInstance.lon);      
      displayinfo();

      if ( gps.speed.kmph() >= RadarInstance.speed && gps.course.deg() >= RadarInstance.direction - 22 && gps.course.deg() <= RadarInstance.direction + 22 ){
        if ( distanceMt < 25 ) {
        sound_alert(50);
      } else if ( distanceMt < 50 ) {
        sound_alert(100);
      } else if ( distanceMt < 75 ) {
        sound_alert(200);
      } else if ( distanceMt < 100 ) {
        sound_alert(300);
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
