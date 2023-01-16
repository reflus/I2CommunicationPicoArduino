//12C MASTER

#include <Wire.h>
#include <BoutonLib.h>

const byte boutonPin[]={2,3}; 
const byte nbrBoutons=sizeof(boutonPin)/sizeof(*boutonPin);
BoutonLib boutons(boutonPin,nbrBoutons);
  
const int I2C_ADD =0x50;

void setup() {
    Wire.begin();
    Serial.begin(9600);
  
    pinMode(LED_BUILTIN, OUTPUT);

}
  boolean blink;

void loop() {
blink=boutons.read(0);

    for(int i = 0;i<100;i++){
       Wire.beginTransmission(I2C_ADD );
       Wire.write(blink);
       Wire.endTransmission();
       
    }
    boutons.update();
}