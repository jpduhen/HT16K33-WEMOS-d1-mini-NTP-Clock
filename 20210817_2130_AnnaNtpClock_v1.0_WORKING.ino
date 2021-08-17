/*
 * Simple NTP client
 * https://www.mischianti.org/
 *
 * The MIT License (MIT)
 * written by Renzo Mischianti <www.mischianti.org>
 * 
 * edit by JP Duhen 2021-08-17:
 * WiFi manager to set the WiFi credentials via an AP interface
 * Display Local time on a HT16k33 4 x 7 seg LED-display
 */
 
 

// change next line to use with another board/shield
#include <ESP8266WiFi.h> // for ESP8266 based boards
//#include <WiFi.h>      // for WiFi shield
//#include <WiFi101.h>   // for WiFi 101 shield or MKR1000
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <time.h>
#include <Timezone.h>    // https://github.com/JChristensen/Timezone
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <HT16K33.h>     // https://github.com/lpaseen/ht16k33/blob/master/src/ht16k33.h
 
/**
 * Input time in epoch format and return tm time format
 * by Renzo Mischianti <www.mischianti.org> 
 */
static tm getDateTimeByParams(long time){
    struct tm *newtime;
    const time_t tim = time;
    newtime = localtime(&tim);
    return *newtime;
}
/**
 * Input tm time format and return String with format pattern
 * by Renzo Mischianti <www.mischianti.org>
 */
static String getDateTimeStringByParams(tm *newtime, char* pattern = (char *)"%d/%m/%Y %H:%M:%S"){
    char buffer[30];
    strftime(buffer, 30, pattern, newtime);
    return buffer;
}
 
/**
 * Input time in epoch format format and return String with format pattern
 * by Renzo Mischianti <www.mischianti.org> 
 */
static String getEpochStringByParams(long time, char* pattern = (char *)"%d/%m/%Y %H:%M:%S"){
//    struct tm *newtime;
    tm newtime;
    newtime = getDateTimeByParams(time);
    return getDateTimeStringByParams(&newtime, pattern);
}
 
WiFiUDP ntpUDP;
HT16K33 seg(0x70); // create an instance of 7 segment display

int GTMOffset = 0; // SET TO UTC TIME, correction via TimeZone.h

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
// NTPClient timeClient(ntpUDP);
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", GTMOffset*60*60, 60*60*1000);
 
// Daylight Saving Time (DST) settings for Central European Time (Amsterdam, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

String myTime,myHours,myMins;

void setup(){
  
  Serial.begin(115200);
  
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    WiFiManager wm; //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    //wm.resetSettings(); //reset settings - wipe credentials for testing

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res; // Chose one of the three options to access your Access Point
    // res = wm.autoConnect();                      // auto generated AP name from chipid
    // res = wm.autoConnect("NTP-klok_AP");         // anonymous ap
    res = wm.autoConnect("NTP-klok_AP","NTP-klok"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have a connection to the WiFi    
        Serial.println("connected to WiFi...YeeHah :)");
    }
 
  timeClient.begin();
  delay ( 1000 );
  if (timeClient.update()){
     Serial.print ( "Adjusting local clock" );
     unsigned long epoch = timeClient.getEpochTime();
     setTime(epoch);
  }else{
     Serial.print ( "NTP Update failed!!" );
  }
  
  // set-up the HT16k33 display
  seg.begin();
  Wire.setClock(100000);
  seg.displayOn();
  seg.setDigits(4);   // number of digits (4)
  seg.brightness(1);  // display brightness min(0) max(15)

}
 
void loop() {
  // I print the time from local clock but first I check DST 
  // to add/substract hours if needed
  myTime = getEpochStringByParams(CE.toLocal(now()));
  Serial.println(myTime); // check myTime content on serial port
  //extract the hour and minute as a substring from the myTime string
  myHours = myTime.substring(11, 13);
  myMins = myTime.substring(14, 16);
  //display the local time with DST correction on the display 
  //converting the String to Int
  seg.displayTime(myHours.toInt(), myMins.toInt());
  //blink the colon every second (500 ms on 500 ms off)
  seg.displayColon(1);
  delay(500);
  seg.displayColon(0); 
  delay(500);
}
