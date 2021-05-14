#include"driver/sc3d.h"

void setup() {

   Serial.begin(9600);

}

void loop() {

   if(count<10)
   {
     led.on();
     delay(300);
     Serial.print("LED state:");Serial.println(led.getState(),DEC);
     
     led.off();
     delay(300);
     Serial.print("LED state:");Serial.println(led.getState(),DEC);
     
     ++count;
     if(count==10)
        led.disattach();
   }
   
   

}
