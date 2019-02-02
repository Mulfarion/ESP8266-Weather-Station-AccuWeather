// New color definitions use for all my libraries
//#define TFT_BLACK       0x0000      /*   0,   0,   0 */
//#define TFT_NAVY        0x000F      /*   0,   0, 128 */
//#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
//#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
//#define TFT_MAROON      0x7800      /* 128,   0,   0 */
//#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
//#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
//#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
//#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
//#define TFT_BLUE        0x001F      /*   0,   0, 255 */
//#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
//#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
//#define TFT_RED         0xF800      /* 255,   0,   0 */
//#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
//#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
//#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
//#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
//#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
//#define TFT_PINK        0xFC9F
//These enumerate the text plotting alignment (reference datum point)
//#define TL_DATUM 0 // Top left (default)
//#define TC_DATUM 1 // Top centre
//#define TR_DATUM 2 // Top right
//#define ML_DATUM 3 // Middle left
//#define CL_DATUM 3 // Centre left, same as above
//#define MC_DATUM 4 // Middle centre
//#define CC_DATUM 4 // Centre centre, same as above
//#define MR_DATUM 5 // Middle right
//#define CR_DATUM 5 // Centre right, same as above
//#define BL_DATUM 6 // Bottom left
//#define BC_DATUM 7 // Bottom centre
//#define BR_DATUM 8 // Bottom right
//#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
//#define C_BASELINE 10 // Centre character baseline
//#define R_BASELINE 11 // Right character baseline
/*****************************
 * Important: see settings.h to configure your settings!!!
 * ***************************/
#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <JsonListener.h>

#define TFT_GREY 0x5AEB

#include "settings.h"
#include "Adafruit_GFX.h"
// Additional UI functions
#include "GfxUi.h"
#include "AccuWeatherLibrary.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "TimeClient.h"

AccuweatherDailyData dataD[5];
AccuweatherHourlyData dataH;
AccuweatherCurrentData dataC;
Accuweather aw(ACCUWEATHER_API_KEY, ACCUWEATHER_CITY_ID, ACCUWEATHER_LANGUAGE, IS_METRIC);

#define LOAD_GFXFF
#define GFXFF 1
#define GLCD  0

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
GfxUi ui = GfxUi(&tft);

// Fonts created by http://oleddisplay.squix.ch/
#include "ArialRoundedMTBold_36.h"

// Images weather
#include "images_1.h" //img day & logo
#include "images_2.h" //img weather
#include "images_3.h" //img weather
#include "images_4.h" //img weather
#include "images_5.h" //img mini
#include "images_6.h" //img weather
#include "images_7.h" //img weather
#include "images_8.h" //img weather
// Images moon phases
#include "moonphases.h"
#include "functions.h"

simpleDSTadjust dstAdjusted(StartRule, EndRule);

void updateData();
void drawProgress(uint8_t percentage, String text);
void drawWifiQuality();
void drawTime();
void drawCurrentWeather();
void drawForecast();
void drawAstronomy();
void drawCurrentWeatherDetail();

long lastDownloadUpdate = millis();
String leftTime;
String getTime(time_t *timestamp);
time_t dstOffset = 0;
bool NO_DATA = false;
String MSG_TEXT[25];
String WDAY_NAMES[7];
String MONTH_NAMES[12];
String SUN_MOON_TEXT[8];
String MOON_PHASES[8];
int touchScreen = 0;
int screenCount = 10;
uint16_t x, y;

void setup() {
  Serial.begin(115200);
  Serial.println("Start..."); 

  if(SET_LANGUAGE == "ru"){
    memcpy(MSG_TEXT, MSG_RU, sizeof(MSG_RU));
    memcpy(WDAY_NAMES, WDAY_NAMES_RU, sizeof(WDAY_NAMES_RU));
    memcpy(MONTH_NAMES, MONTH_NAMES_RU, sizeof(MONTH_NAMES_RU));
    memcpy(SUN_MOON_TEXT, SUN_MOON_TEXT_RU, sizeof(SUN_MOON_TEXT_RU));
    memcpy(MOON_PHASES, MOON_PHASES_RU, sizeof(MOON_PHASES_RU));
  }else{
    memcpy(MSG_TEXT, MSG_EN, sizeof(MSG_EN));
    memcpy(WDAY_NAMES, WDAY_NAMES_EN, sizeof(WDAY_NAMES_EN));
    memcpy(MONTH_NAMES, MONTH_NAMES_EN, sizeof(MONTH_NAMES_EN));
    memcpy(SUN_MOON_TEXT, SUN_MOON_TEXT_EN, sizeof(SUN_MOON_TEXT_EN));
    memcpy(MOON_PHASES, MOON_PHASES_EN, sizeof(MOON_PHASES_EN));
  }
 
  tft.init();  
  tft.fillScreen(TFT_BLACK);
  
  // Swap the colour byte order when rendering
  tft.setSwapBytes(true);
  
  // Turn on the background LED
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  tft.drawString(utf8rus(MSG_TEXT[23]), 87, 15);
  tft.pushImage(70, 30, 100, 90, AWLogo);
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString(utf8rus(MSG_TEXT[0]), 120, 230);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.pushImage(84, 245, 72, 64, CrazyLogo);

  connectWifi();
  updateData();
}

void loop(void) {
  
  tft.getTouch(&x, &y);  
  if(x > 0 || y > 0){
    touchScreen = (touchScreen + 1) % screenCount;
    x = 0;
    y = 0;
  }

  if (touchScreen == 0) {
    drawWifiQuality();
    drawTime();
    if(!NO_DATA){
      drawCurrentWeather();
      drawForecast();
      drawAstronomy();
    }else{
      noData(utf8rus(MSG_TEXT[24]));
    }
  } else if (touchScreen == 1) {
    refreshScreen();
    touchScreen = 2;
  } else if (touchScreen == 2) {
    drawCurrentWeatherDetail();
  } else if (touchScreen == 3) {
    refreshScreen();
    touchScreen = 4;
  } else if (touchScreen == 4) {
    if(!NO_DATA){
      drawForecastDetail(5, 10, 1);
      drawForecastDetail(5, 170, 2);
    }else{
      noData(utf8rus(MSG_TEXT[24]));
    }
  } else if (touchScreen == 5) {
    refreshScreen();
    touchScreen = 6;
  } else if (touchScreen == 6) {
    if(!NO_DATA){
      drawForecastDetail(5, 10, 3);
      drawForecastDetail(5, 170, 4);
    }else{
      noData(utf8rus(MSG_TEXT[24]));
    }
  } else if (touchScreen == 7) {
    refreshScreen();
    touchScreen = 8;
  }else if (touchScreen == 8) {
    drawAbout();
  } else if (touchScreen == 9) {
    refreshScreen();
    touchScreen = 0;
  }
  
  // Check if we should update weather information
  if (millis() - lastDownloadUpdate > 60000 * UPDATE_INTERVAL) { //after 7200000 ms update
    tft.fillScreen(TFT_BLACK);
    updateData();
    lastDownloadUpdate = millis();
  }else{
    const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
    const uint32_t millis_in_hour = 1000 * 60 * 60;
    const uint32_t millis_in_minute = 1000 * 60;
    int timeUpdate = 60000 * UPDATE_INTERVAL - millis();
    uint8_t days = timeUpdate / (millis_in_day);
    uint8_t hours = (timeUpdate - (days * millis_in_day)) / millis_in_hour;
    uint8_t minutes = (timeUpdate - (days * millis_in_day) - (hours * millis_in_hour)) / millis_in_minute;
    char time_str[6];
    sprintf(time_str, "%2dh:%2dm", hours, minutes);
    leftTime = time_str;
  }
}


void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  //Manual Wifi
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  Serial.print("/");
  Serial.println(WIFI_PASS);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (i>80) i=0;
    drawProgress(i,utf8rus(MSG_TEXT[1]) + String(WIFI_SSID) + "'");
    i+=10;
    Serial.print(".");
  }
  drawProgress(100,utf8rus(MSG_TEXT[2]) + String(WIFI_SSID) + "'");
  Serial.println("Connected...");
}


void drawWifiQuality() {
  int8_t quality = getWifiQuality();
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextFont(0);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(" " + String(quality) + "%", 218, 9);
  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (quality > i * 25 || j == 0) {
        tft.drawPixel(222 + 2 * i, 15 - j, TFT_WHITE);
      }
    }
  }
}


// Progress bar helper
void drawProgress(uint8_t percentage, String text) {
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString(utf8rus(text), 120, 160);
  ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, TFT_WHITE, TFT_GREEN);
  tft.setTextPadding(0);
}


// Update the internet based information and update screen
void updateData() {
  drawProgress(30, MSG_TEXT[7]);
  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);  
  while(!time(nullptr)) {
    Serial.print("#");
    delay(100);
  }
  dstOffset = UTC_OFFSET * 3600 + dstAdjusted.time(nullptr) - time(nullptr);
  drawProgress(60, MSG_TEXT[8]);
  int timeStart = millis();
  int ret = aw.getCurrent(&dataC);
  if (ret != 0){
    Serial.println("ERROR_1");
    Serial.println(ret);
    tft.fillScreen(TFT_BLACK);
    NO_DATA = true;
    return;
  }
  while (aw.continueDownload() > 0){
  }
  timeStart = millis() - timeStart;
  Serial.printf("Downloaded and parsed in %d ms\n", timeStart);
  delay(1000);
  drawProgress(80, MSG_TEXT[9]);
  int timeStart2 = millis();
  int cast = aw.getDaily(dataD,5);
  if (cast != 0){
    Serial.println("ERROR_2");
    Serial.println(cast);
    tft.fillScreen(TFT_BLACK);
    NO_DATA = true;
    return;
  }
  while (aw.continueDownload() > 0){
  }
  timeStart2 = millis() - timeStart2;
  Serial.printf("Downloaded and parsed in %d ms\n", timeStart2);
  drawProgress(100, MSG_TEXT[21]);
  delay(1000);
  touchScreen = 9;
}


// draws the clock
void drawTime() {
  
  tft.setTextDatum(MC_DATUM);
  char time_str[11];
  char *dstAbbrev;
  time_t now = dstAdjusted.time(&dstAbbrev);
  struct tm * timeinfo = localtime (&now);

  String date = WDAY_NAMES[timeinfo->tm_wday] + " " + String(timeinfo->tm_mday) + " " + MONTH_NAMES[timeinfo->tm_mon] + " " + String(1900 + timeinfo->tm_year);
  
  tft.setTextFont(0);
  tft.setTextSize(3);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    
  if (IS_STYLE_12HR) {
    int hour = (timeinfo->tm_hour+11)%12+1;  // take care of noon and midnight
    tft.drawNumber(hour, 60, 33, 1);
    tft.drawNumber(timeinfo->tm_min, 117, 33, 1);
    tft.drawNumber(timeinfo->tm_sec, 172, 33, 1);
  } else {
    tft.drawString(addZero(timeinfo->tm_hour), 60, 33, 1);
    tft.drawString(addZero(timeinfo->tm_min), 117, 33, 1);
    tft.drawString(addZero(timeinfo->tm_sec), 172, 33, 1);
  }
  
  if (timeinfo->tm_sec % 2) {          // Flash the colons on/off
     tft.setTextColor(TFT_BLACK);      // Set colour to grey to dim colon
     tft.drawChar(':', 81, 19, 1);     // Hour:minute colon
     tft.drawChar(':', 136, 19, 1);    // Seconds colon
  }else{
     tft.setTextColor(TFT_WHITE);      // Set colour back to yellow
     tft.drawChar(':', 81, 19, 1);     // Hour:minute colon
     tft.drawChar(':', 136, 19, 1);    // Seconds colon
  }

  tft.setTextFont(0);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLUE);
  if (IS_STYLE_12HR) {
    sprintf(time_str, "%s\n%s", dstAbbrev, timeinfo->tm_hour>=12?"PM":"AM");
    tft.drawString(time_str, 205, 27);
  } else {
    sprintf(time_str, "%s", dstAbbrev);
    tft.drawString(time_str, 205, 27);  // Known bug: Cuts off 4th character of timezone abbreviation
  }

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(utf8rus(date), 120, 15);
}


// draws current weather information
void drawCurrentWeather() {
  int WeatherIcon = dataC.WeatherIcon;
  printIco(0, 49, WeatherIcon, String("norm"));
  tft.setTextDatum(MR_DATUM);
  tft.setTextFont(0);
  tft.drawString(utf8rus(ACCUWEATHER_CITY_NAME), 220, 57);
  tft.setTextColor(TFT_WHITE);
  drawTemp(dataC.Temperature, dataD[0].TempMin, dataD[0].TempMax, dataC.RealFeelTemperature, IS_METRIC, utf8rus(MSG_TEXT[22]));
  tft.setTextColor(TFT_ORANGE);
  tft.drawString(utf8rus(dataC.WeatherText), 220, 113);
}


void drawForecast() {
  int x = 0;
  int y = 155;
  drawForecastDetailMini(x + 10, y, 1);
  drawForecastDetailMini(x + 65, y, 2);
  drawForecastDetailMini(x + 120, y, 3);
  drawForecastDetailMini(x + 175, y, 4);
}


// draw moonphase and sunrise/set and moonrise/set
void drawAstronomy() {
  printMoon(dataD[0].MoonAge, MOON_PHASES, SUN_MOON_TEXT);
  tft.setTextDatum(ML_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString(utf8rus(SUN_MOON_TEXT[0]), 10, 255);
  tft.setTextColor(TFT_WHITE);
  time_t time = dataD[0].SunRise + dstOffset;
  tft.drawString(utf8rus(SUN_MOON_TEXT[1]), 10, 276);
  tft.drawString(getTime(&time), 55, 276);
  time = dataD[0].SunSet + dstOffset;
  tft.drawString(utf8rus(SUN_MOON_TEXT[2]), 10, 291);
  tft.drawString(getTime(&time), 55, 291);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(MR_DATUM);
  tft.drawString(utf8rus(SUN_MOON_TEXT[3]), 230, 255);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(utf8rus(SUN_MOON_TEXT[4]), 210, 276);
  tft.drawString(utf8rus(SUN_MOON_TEXT[5]), 210, 291);
}


void drawCurrentWeatherDetail() {
  tft.setTextDatum(MC_DATUM);
  tft.drawString(utf8rus(MSG_TEXT[3]), 120, 15);
  String msg = "";
  String degreeSign = " F";
  if (IS_METRIC) {
    degreeSign = " C";
  }
  drawLabelValue(1, utf8rus(MSG_TEXT[12]), String(dataC.Temperature, 1) + degreeSign);
  drawLabelValue(2, utf8rus(MSG_TEXT[13]), String(dataC.WindSpeed, 1) + (IS_METRIC ? "km/h" : "mi/h") );
  drawLabelValue(3, utf8rus(MSG_TEXT[14]), String(dataC.WindDirection) + utf8rus(MSG_TEXT[4]) + " (" + (SET_LANGUAGE == "ru" ? utf8rus(String(dataC.WindDirectionLocalized)) : String(dataC.WindDirectionEnglish)) + ")");
  drawLabelValue(4, utf8rus(MSG_TEXT[15]), String(dataC.RelativeHumidity) + "%");
  drawLabelValue(5, utf8rus(MSG_TEXT[16]), String(dataC.Pressure) + (IS_METRIC ? "mb" : "inHg"));
  drawLabelValue(6, utf8rus(MSG_TEXT[17]), String(dataC.CloudCover) + "%");
  if(dataC.Visibility > 1000) {
    msg = dataC.Visibility / 1000;
    drawLabelValue(7, utf8rus(MSG_TEXT[18]), msg + (IS_METRIC ? "km" : "mi"));
  }else{
    drawLabelValue(7, utf8rus(MSG_TEXT[18]), String(dataC.Visibility) + "m");
  }
  drawLabelValue(8, utf8rus(MSG_TEXT[11]), String(dataC.UVIndex));
  drawLabelValue(9, utf8rus(MSG_TEXT[20]), utf8rus(String(dataC.UVIndexText)));
}


// helper for the forecast columns
void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex) {
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);
  time_t time = dataD[dayIndex].EpochDate;
  struct tm * timeinfo = localtime (&time);
  String date = WDAY_NAMES[timeinfo->tm_wday] + " " + String(timeinfo->tm_mday) + " " + MONTH_NAMES[timeinfo->tm_mon] + " " + String(1900 + timeinfo->tm_year);
  tft.drawString(utf8rus(date), 120, y + 5);
  
  int WeatherIcon = dataD[dayIndex].Day.WeatherIcon;
  printIco(50, y + 13, WeatherIcon, String("mini"));

  tft.setTextColor(TFT_WHITE); 

  drawTempForecast(x, y, dataD[dayIndex].TempMin, dataD[dayIndex].TempMax, dataD[dayIndex].RealFeelTempMax, dataD[dayIndex].SunRise, dataD[dayIndex].SunSet, dstOffset, IS_METRIC, SUN_MOON_TEXT);
  
  tft.setTextColor(TFT_ORANGE);
  tft.drawString(utf8rus(dataD[dayIndex].Day.LongPhrase), 120, y + 60);
  
  tft.setTextDatum(ML_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString(utf8rus(MSG_TEXT[5]), x + 40, y + 85);
  tft.drawString(utf8rus(MSG_TEXT[17]), x + 40, y + 98);
  tft.drawString(utf8rus(MSG_TEXT[6]), x + 40, y + 111);
  
  tft.setTextColor(TFT_WHITE);
  String WindDir = "";
  SET_LANGUAGE == "ru" ? WindDir = utf8rus(String(dataD[dayIndex].Day.WindDirectionLocalized)) : WindDir = String(dataD[dayIndex].Day.WindDirectionEnglish);
  tft.drawString(utf8rus(String(dataD[dayIndex].Day.WindSpeed)) + "km/h" + " (" + WindDir + ")", x + 115, y + 85);
  tft.drawString(utf8rus(String(dataD[dayIndex].Day.CloudCover)) + "%", x + 115, y + 98);
  tft.drawString(utf8rus(String(dataD[dayIndex].Day.TotalLiquid)) + "mm", x + 115, y + 111);

  tft.drawFastHLine(0, y + 147, tft.width(), TFT_DARKGREY);
}


// helper for the forecast columns
void drawForecastDetailMini(uint16_t x, uint16_t y, uint8_t dayIndex) {
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);
  
  time_t time = dataD[dayIndex].EpochDate;
  struct tm * timeinfo = localtime (&time);
  tft.drawString(utf8rus(WDAY_NAMES[timeinfo->tm_wday]), x + 25, y - 15);

  tft.setTextColor(TFT_WHITE);
  tft.drawString(String(dataD[dayIndex].TempMax, 0) + (IS_METRIC ? " C" : " F"), x + 25, y);
  tft.drawString(String(dataD[dayIndex].TempMin, 0) + (IS_METRIC ? " C" : " F"), x + 25, y+15);

  int WeatherIcon = dataD[dayIndex].Day.WeatherIcon;
  printIco(x, y + 30, WeatherIcon, String("mini"));
  
  tft.setTextColor(TFT_BLUE);
  tft.drawString(String(dataD[dayIndex].Day.TotalLiquid) + "mm", x + 25, y + 65);
}


void drawAbout() {
  tft.pushImage(84, 5, 72, 64, CrazyLogo);
  tft.setTextDatum(BC_DATUM);
  tft.drawString(utf8rus(MSG_TEXT[0]), 120, 95, 1);
  drawLabelValue(7, "Heap Mem:", String(ESP.getFreeHeap() / 1024)+"kb");
  drawLabelValue(8, "Flash Mem:", String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
  drawLabelValue(9, "WiFi Strength:", String(WiFi.RSSI()) + "dB");
  drawLabelValue(10, "Chip ID:", String(ESP.getChipId()));
  drawLabelValue(11, "VCC: ", String(ESP.getVcc() / 1024.0) +"V");
  drawLabelValue(12, "CPU Freq.: ", String(ESP.getCpuFreqMHz()) + "MHz");
  char time_str[15];
  const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
  const uint32_t millis_in_hour = 1000 * 60 * 60;
  const uint32_t millis_in_minute = 1000 * 60;
  uint8_t days = millis() / (millis_in_day);
  uint8_t hours = (millis() - (days * millis_in_day)) / millis_in_hour;
  uint8_t minutes = (millis() - (days * millis_in_day) - (hours * millis_in_hour)) / millis_in_minute;
  sprintf(time_str, "%2dd%2dh%2dm", days, hours, minutes);
  drawLabelValue(13, "Uptime: ", time_str);
  drawLabelValue(14, utf8rus(MSG_TEXT[10]), leftTime);
  tft.setTextDatum(BL_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString("Last Reset: ", 15, 255, 1);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(ESP.getResetReason(), 15, 270, 1);
}
