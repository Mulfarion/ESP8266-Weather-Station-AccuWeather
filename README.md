# ESP8266-Weather-Station-AccuWeather
![ESP8266-Weather-Station-AccuWeather](img/weather.JPG)

# RUS

ESP8266 AccuWeather Weather Station - это проект информера используйющий данные с сайта погоды AccuWeather. Имеет несколько экранов, которые переключаются по тапу по экрану. 

# Установка и запуск
1. Подключить экран к ESP8266 по схеме ниже.
2. Установить библиотеки из папки lib
3. Зарегистрироваться на сайте Accuweather и получить ключ от API (базовый ключ дает 50 запросов в день)

Link: https://developer.accuweather.com/getting-started
4. Настроить файл "settings.h". С учетом того, что прошивка использует два запроса для получения информации о текущей погоде и прогноза на 4 следующих дня, то рекомендуется ставить интервал обновления не менее 1 часа. По дефолту стоит 2 часа.

# EN

ESP8266 AccuWeather Weather Station is an informer project using data from the AccuWeather site. It has several screens that switch on tap on the screen.

# Installation and launch
1. Connect the screen to the ESP8266 as shown below.
2. Install libraries from the lib folder
3. Register on Accuweather website and get the key from the API (the basic key gives 50 requests per day)

Link: https://developer.accuweather.com/getting-started
4. Configure the file "settings.h". Given that the firmware uses two requests to obtain information about the current weather and the forecast for the next 4 days, it is recommended to set an update interval of at least 1 hour. By default it costs 2 hours.

![Wiring](img/PlaneSpotterWiring.png)

| Wemos D1 Mini | ILI9341      |
| ------------- |:-------------:|
| D4            | T_IRQ         |
| D6            | T_DO          |
| D7            | T_DIN         |
| D3            | T_CS          |
| D5            | T_CLK         |
| D6            | SDO           |
| D8            | LED           |
| D5            | SCK           |
| D7            | SDI           |
| D2            | D/C           |
| RST           | RESET         |
| D1            | CS            |
| GND           | GND           |
| 3V3           | VCC           |
