#include <simpleDSTadjust.h>

// Setup WiFi
#define WIFI_SSID "Fury"
#define WIFI_PASS "freedom4free"
#define WIFI_HOSTNAME "AccuWeather-informer"

const int UPDATE_INTERVAL = 3*60; // Update time interval in minutes

String SET_LANGUAGE = "ru"; //Russian language
//String SET_LANGUAGE = "en"; //English language

// AccuWeather Settings
// Sign up here to get an API key: https://developer.accuweather.com/getting-started
const char* ACCUWEATHER_API_KEY = "YOU_API_KEY"; //Limited Trial 50 calls/day
const int ACCUWEATHER_CITY_ID = 2452002;
const String ACCUWEATHER_CITY_NAME = "Снежногорск";
const char* ACCUWEATHER_LANGUAGE = "ru-ru";
const bool IS_METRIC = true;

// For the Adafruit shield, these are the default.
#define TFT_DC D2
#define TFT_CS D1
#define TFT_LED D8

// Adjust according to en language
const String WDAY_NAMES_EN[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES_EN[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
const String SUN_MOON_TEXT_EN[] = {"Sun", "Rise", "Set", "Moon", "Age", "Illum", "d", "Feels like: "};
const String MOON_PHASES_EN[] = {"New Moon", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
                              "Full Moon", "Waning Gibbous", "Third quarter", "Waning Crescent"};
                              
// Adjust according to rus language
const String WDAY_NAMES_RU[] = {"Вc", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
const String MONTH_NAMES_RU[] = {"Января", "Февраля", "Марта", "Апреля", "Мая", "Июня", "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря"};
const String SUN_MOON_TEXT_RU[] = {"Солнце", "Восход: ", "Заход: ", "Луна", "Рост: ", "Свет: ", "д", "По ощущению: "};
const String MOON_PHASES_RU[] = {"Молодая луна", "Растущий серп", "Первая четверть", "Растущая луна",
                              "Полная луна", "Убывающая луна", "Третья четверть", "Убывающий серп"};
                              
String MSG_RU[] = {
  "Создано: CrazyOwl", 
  "  Подключаемся к WiFi '",
  "  Подключено к WiFi '",
  "-== Текущая погода ==-",
  "град",
  "Ветер: ",
  "Осадки: ",
  "       Обновляем время...      ",
  " Обновляем погодные условия... ",
  "      Обновляем погоду...      ",
  "Обновление: ",
  "УВ индекс: ",
  "Температура: ",
  "Скорость ветра: ",
  "Направление ветра: ",
  "Влажность: ",
  "Давление: ",
  "Облачность: ",
  "Видимость: ",
  "Прогноз погоды",
  "УВ текст: ",
  "           Завершено           ",
  "Ощущается: ",
  "Работает на: ",
  "--Нет данных--"
};       

String MSG_EN[] = {
    "Created by: CrazyOwl",
    "Connecting to WiFi '",
    "Connected to WiFi '",
    "-== Current weather ==-",
    "deg",
    "Wind: ",
    "Liquid: ",
    "      Updating time...      ",
    "   Updating conditions...   ",
    "    Updating forecasts...   ",
    "Update: ",
    "UV Index: ",
    "Temperature: ",
    "Wind Speed: ",
    "Wind Dir: ",
    "Humidity: ",
    "Pressure: ",
    "Clouds: ",
    "Visibility: ",
    "Forecasts",
    "UV Index text: ",
    "          Done             ",
    "Feels like: ",
    "Powered by:",
    "--No data--"
}; 

// Change for 12 Hour/ 24 hour style clock
bool IS_STYLE_12HR = false;

// change for different ntp (time servers)
// #define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"
// #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
#define NTP_SERVERS "ru.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"

// Settings offset
#define UTC_OFFSET +3
struct dstRule StartRule = {"CET", Last, Sun, Oct, 2, 0}; // Central European Summer Time = UTC/GMT +2 hours
struct dstRule EndRule = {"CET", Last, Sun, Oct, 2, 0};       // Central European Time = UTC/GMT +1 hour
