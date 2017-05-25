#include <Event.h>
#include <Timer.h>

 #define PIN_L0 3
#define PIN_L1 4

#define PIN_ERR 7
#define PIN_LZR 5

  uint16_t l0_val;
  uint16_t l1_val;

  volatile uint64_t cnt_0;
  volatile uint64_t cnt_1;

  uint8_t flg0;
  uint8_t flg1;
  uint8_t sfg;

  uint8_t location = 0;     //initialize location to chamber zero


void setup() {
  
  
  pinMode(PIN_ERR, OUTPUT);
  pinMode(PIN_LZR, OUTPUT);
  pinMode(PIN_L0, INPUT);
  pinMode(PIN_L1, INPUT);
  Serial.begin(9600);
}

void loop() {

//read up
 l0_val = digitalRead(PIN_L0);
 l1_val = digitalRead(PIN_L1);
//print state of each diode (for debugging)
 Serial.print(l0_val);
 Serial.print("   ");
 Serial.println(l1_val);



  //set flags


    //figure out where the vole is
  
 if(!l0_val && !flg0 && !sfg){
  flg0 = 1;
  if(flg1){
    location = 0;
    flg1 = 0;
    flg0 = 0;
    if(!l0_val && ! l1_val){
      while(!l0_val && !l1_val){
        l1_val = digitalRead(PIN_L1);
        l0_val = digitalRead(PIN_L0);
      }
      if(!l1_val){
        location = 1;
      }
      if(!l0_val){
        location = 0;
      }
    }
  }
 }
 if(!l1_val  && !flg1 && !sfg){
  flg1=1;
  if(flg0){
    location = 1;
    flg1=0;
    flg0=0;
        if(!l0_val && ! l1_val){
      while(!l0_val && !l1_val){
        l1_val = digitalRead(PIN_L1);
        l0_val = digitalRead(PIN_L0);
      }
      if(!l1_val){
        location = 1;
      }
      if(!l0_val){
        location = 0;
      }
    }
  }
 }


  //update ouputs based on location
 if(location == 1){
  digitalWrite(PIN_LZR, HIGH);
  digitalWrite(PIN_ERR,LOW);

 }
 else{
  digitalWrite(PIN_LZR, LOW);
  digitalWrite(PIN_ERR,HIGH);
 }



 //track time
 

}


void time_tracker_ISR(void){
  
  
}






