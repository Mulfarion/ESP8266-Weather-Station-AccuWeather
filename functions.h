/* Recode russian fonts from UTF-8 to Windows-1251 */
String utf8rus(String source) {
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
  return target;
}

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  else if (dbm >= -50) return 100;
  else return 2 * (dbm + 100);
}

String addZero(int n) {
  String zero = "0";
  if(n < 10) return zero + String(n);
  else return String(n);
}

String getTime(time_t *timestamp) {
  struct tm *timeInfo = gmtime(timestamp);
  char buf[6];
  sprintf(buf, "%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min);
  return String(buf);
}

void drawTemp(float TempC, float TempMin, float TempMax, float TempF, bool metric, String feelT) {
  String metricSys = " F";
  if (metric) metricSys = " C";
  tft.setFreeFont(CF_Y32);
  tft.drawString(String(TempC, 0), 160, 75, GFXFF);
  tft.drawString(String(metricSys), 200, 75, GFXFF);
  tft.setFreeFont(FSSB9);
  tft.drawString("o", 174, 67, GFXFF);
  tft.setTextFont(0);
  tft.drawString(String(TempMax, 0), 223, 70);
  tft.drawString(String(TempMin, 0), 223, 80);
  tft.drawString("o", 230, 68);
  tft.drawString("o", 230, 78);
  tft.drawString(feelT + String(TempF, 0), 214, 100);
  tft.drawString("o", 221, 98);
}

void drawTempForecast(int x, int y, float TempMin, float TempMax, float TempFeel, float SunRise, float SunSet, int Offset, bool metric, String SUN_MOON_TEXT[]) {
  tft.setTextDatum(MR_DATUM);
  tft.setFreeFont(CF_Y32);
  tft.drawString(String(TempMax, 0), 150, y + 25, GFXFF);
  tft.setFreeFont(FSSB9);
  tft.drawString("o", 163, y + 17, GFXFF);
  tft.setTextFont(0);
  tft.drawString(String(TempMin, 0), 187, y + 20);
  tft.drawString("o", 193, y + 18);

  tft.drawString(utf8rus(SUN_MOON_TEXT[7]) + String(TempFeel, 0), 170, y + 48);
  tft.drawString("o", 176, y + 46);
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString(utf8rus(SUN_MOON_TEXT[1]), 30, y + 20);
  tft.drawString(utf8rus(SUN_MOON_TEXT[2]), 218, y + 20);

  tft.setTextColor(TFT_WHITE);
  time_t time = SunRise + Offset;
  tft.drawString(getTime(&time), 25, y + 32);
  time = SunSet + Offset;
  tft.drawString(getTime(&time), 215, y + 32);
}

void refreshScreen() {
  for (int n=0; n <= 320; n++){     
    tft.drawFastHLine(0, n, tft.width(), TFT_BLACK); // tft.width() returns the pixel width of the display
  }
}

void printIco(int x, int y, int n, String type) {
  int w, h;
  if(type == "norm") {
    w=100;h=60; 
  }else{
    w=45;h=27;
  }
  if(n == 1){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img01);
    }else{
      tft.pushImage(x, y, w, h, img01m);
    }
  }else if(n == 2){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img02);
    }else{
      tft.pushImage(x, y, w, h, img02m);
    }
  }else if(n == 3){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img03);
    }else{
      tft.pushImage(x, y, w, h, img03m);
    }
  }else if(n == 4){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img04);
    }else{
      tft.pushImage(x, y, w, h, img04m);
    }
  }else if(n == 5){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img05);
    }else{
      tft.pushImage(x, y, w, h, img05m);
    }
  }else if(n == 6){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img06);
    }else{
      tft.pushImage(x, y, w, h, img06m);
    }
  }else if(n == 7){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img07);
    }else{
      tft.pushImage(x, y, w, h, img07m);
    }
  }else if(n == 8){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img08);
    }else{
      tft.pushImage(x, y, w, h, img08m);
    }
  }else if(n == 11){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img11);
    }else{
      tft.pushImage(x, y, w, h, img11m);
    }
  }else if(n == 12){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img12);
    }else{
      tft.pushImage(x, y, w, h, img12m);
    }
  }else if(n == 13){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img13);
    }else{
      tft.pushImage(x, y, w, h, img13m);
    }
  }else if(n == 14){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img14);
    }else{
      tft.pushImage(x, y, w, h, img14m);
    }
  }else if(n == 15){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img15);
    }else{
      tft.pushImage(x, y, w, h, img15m);
    }
  }else if(n == 16){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img16);
    }else{
      tft.pushImage(x, y, w, h, img16m);
    }
  }else if(n == 17){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img17);
    }else{
      tft.pushImage(x, y, w, h, img17m);
    }
  }else if(n == 18){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img18);
    }else{
      tft.pushImage(x, y, w, h, img18m);
    }
  }else if(n == 19){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img19);
    }else{
      tft.pushImage(x, y, w, h, img19m);
    }
  }else if(n == 20){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img20);
    }else{
      tft.pushImage(x, y, w, h, img20m);
    }
  }else if(n == 21){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img21);
    }else{
      tft.pushImage(x, y, w, h, img21m);
    }
  }else if(n == 22){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img22);
    }else{
      tft.pushImage(x, y, w, h, img22m);
    }
  }else if(n == 23){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img23);
    }else{
      tft.pushImage(x, y, w, h, img23m);
    }
  }else if(n == 24){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img24);
    }else{
      tft.pushImage(x, y, w, h, img24m);
    }
  }else if(n == 25){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img25);
    }else{
      tft.pushImage(x, y, w, h, img25m);
    }
  }else if(n == 26){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img26);
    }else{
      tft.pushImage(x, y, w, h, img26m);
    }
  }else if(n == 29){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img29);
    }else{
      tft.pushImage(x, y, w, h, img29m);
    }
  }else if(n == 30){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img30);
    }else{
      tft.pushImage(x, y, w, h, img30m);
    }
  }else if(n == 31){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img31);
    }else{
      tft.pushImage(x, y, w, h, img31m);
    }
  }else if(n == 32){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img32);
    }else{
      tft.pushImage(x, y, w, h, img32m);
    }
  }else if(n == 33){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img33);
    }else{
      tft.pushImage(x, y, w, h, img33m);
    }
  }else if(n == 34){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img34);
    }else{
      tft.pushImage(x, y, w, h, img34m);
    }
  }else if(n == 35){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img35);
    }else{
      tft.pushImage(x, y, w, h, img35m);
    }
  }else if(n == 36){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img36);
    }else{
      tft.pushImage(x, y, w, h, img36m);
    }
  }else if(n == 37){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img37);
    }else{
      tft.pushImage(x, y, w, h, img37m);
    }
  }else if(n == 38){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img38);
    }else{
      tft.pushImage(x, y, w, h, img38m);
    }
  }else if(n == 39){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img39);
    }else{
      tft.pushImage(x, y, w, h, img39m);
    }
  }else if(n == 40){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img40);
    }else{
      tft.pushImage(x, y, w, h, img40m);
    }
  }else if(n == 41){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img41);
    }else{
      tft.pushImage(x, y, w, h, img41m);
    }
  }else if(n == 42){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img42);
    }else{
      tft.pushImage(x, y, w, h, img42m);
    }
  }else if(n == 43){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img43);
    }else{
      tft.pushImage(x, y, w, h, img43m);
    }
  }else if(n == 44){
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img44);
    }else{
      tft.pushImage(x, y, w, h, img44m);
    }
  }else{
    if(type == "norm"){
      tft.pushImage(x, y, w, h, img45); //no img
    }else{
      tft.pushImage(x, y, w, h, img45m); //no img mini
    }
  }
}

void printMoon(int moonAge, String MOON_PHASES[], String SUN_MOON_TEXT[]) {
  int n;
  int illumination;
  if((moonAge >= 0) && (moonAge <= 2)){
    n = 0;
    illumination = 0;
    tft.pushImage(95, 255, 60, 60, moonphase_L0);
  }else if((moonAge > 2) && (moonAge <= 5)){
    n = 1;
    illumination = 25;
    tft.pushImage(95, 255, 60, 60, moonphase_L1);
  }else if((moonAge > 5) && (moonAge <= 8)){
    n = 2;
    illumination = 50;
    tft.pushImage(95, 255, 60, 60, moonphase_L2);
  }else if((moonAge > 8) && (moonAge <= 11)){
    n = 3;
    illumination = 75;
    tft.pushImage(95, 255, 60, 60, moonphase_L3);
  }else if((moonAge > 11) && (moonAge <= 14)){
    n = 4;
    illumination = 100;
    tft.pushImage(95, 255, 60, 60, moonphase_L4);
  }else if((moonAge > 14) && (moonAge <= 17)){
    n = 5;
    illumination = 75;
    tft.pushImage(95, 255, 60, 60, moonphase_L5);
  }else if((moonAge > 17) && (moonAge <= 20)){
    n = 6;
    illumination = 50;
    tft.pushImage(95, 255, 60, 60, moonphase_L6);
  }else{
    n = 7;
    illumination = 25;
    tft.pushImage(95, 255, 60, 60, moonphase_L7);
  }
  
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(utf8rus(MOON_PHASES[n]), 125, 250);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MR_DATUM);
  tft.drawString(String(moonAge) + utf8rus(SUN_MOON_TEXT[6]), 230, 276);
  tft.drawString(String(illumination) + "%", 230, 291);
}

void drawLabelValue(uint8_t line, String label, String value) {
  const uint8_t labelX = 15;
  const uint8_t valueX = 150;
  tft.setTextDatum(BL_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString(label, labelX, 30 + line * 15);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(value, valueX, 30 + line * 15);
}
