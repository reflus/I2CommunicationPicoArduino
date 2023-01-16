//12C MASTER

#include <stdio.h>
#include <pico/stdlib.h>   // stdlib  
#include <pico/binary_info.h>
#include <hardware/i2c.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> bSin(SIN2048_DATA);
    
static int I2C_ADD = 0x50;
    uint8_t buffer[1];   

#define CONTROL_RATE 512

void setup()
{
	pinMode(25, OUTPUT);
  startMozzi(CONTROL_RATE);
    i2c_init(i2c_default, CONTROL_RATE<<1); //set i2c to 2xCONTROL_RATE
    Serial.begin(9600);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    i2c_set_slave_mode(i2c_default,true,I2C_ADD);
    aSin.setFreq(660); // set the frequency
    bSin.setFreq(440); // set the frequency    
	
}
int readResult=0;

void updateControl(){
       if(readResult>0){
        Serial.println(buffer[0]);
        digitalWrite(25, (boolean )buffer[0]);
        }
	    readResult = i2c_read_blocking(i2c_default, I2C_ADD, buffer, 1, false);
}

AudioOutput_t updateAudio(){

  // this would make more sense with a higher resolution signal
  // MonoOutput::from8Bit() (and it friends from16Bit() and fromNBit()) take care of scaling the output signal
  // as appropiate for the platform (to 14 bits on AVR with AUDIO_MODE HIFI).

  return MonoOutput::from8Bit((aSin.next()+bSin.next())>>1);
}


void loop(){
  audioHook(); // required here
}
