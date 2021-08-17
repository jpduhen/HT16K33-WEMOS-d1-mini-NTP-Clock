![IMG_4231](https://user-images.githubusercontent.com/45515609/129792238-06c8491b-76ce-45f6-a7cc-871d7b2ecc30.jpeg)
# HT16K33-WEMOS-d1-mini-NTP-Clock
A "simple" internet synchronized clock using a WEMOS D1 mini and a HT16K33 based 4 digit 7 segment LED display

![IMG_4232](https://user-images.githubusercontent.com/45515609/129792336-91454d5d-c514-484f-914a-4b51ec5663d5.jpeg)

Display | WEMOS
------------ | -------------
'+' | 5V    
'-' | GND   
'D' | D2
'C' | D1
  
Just bending the pins (look out for shorting!) will get you a nice sturdy triangle-standing clock

After flashing the WEMOS d1 mini, connect to the NTP-klok_AP with password: NTP-klok
Select the desired WiFi network you would like to connect to and enter the appropiate credentials.
Very shortly the local time adjusted for Daylight saving will be displayed.

See scetch for adjusting the DST settings for your locality.
