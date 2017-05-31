//********************************************************
//    Botsford, Nelson  
//  Uses 2 IR beams and photodiodes to detect when an animal passes a threshold.
//    *Keeps track of where the animal is.
//    *outputs a digital high signal when in chamber 1
//    *logs time spent in each chamber
//      **for now prints to serial, will soon be updated w/ serial SSD
//    
//*********************************************************


#include <Event.h>
#include <Timer.h>

Timer t;
#define PIN_L0 3
#define PIN_L1 4

#define PIN_G 7
#define PIN_Y 5

#define PIN_BUT 2

#define PIN_ERR 9


  uint16_t l0_val;
  uint16_t l1_val;

  volatile uint32_t cnt_0;
  volatile uint32_t cnt_1;

  uint8_t flg0;
  uint8_t flg1;
  uint8_t sfg;
  uint8_t butflg;

  uint8_t location = 0;     //initialize location to chamber zero
  uint64_t err_cnt = 0;


void setup() {
  
  t.every(250, time_tracker_ISR,0xffffffff);
  
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_Y, OUTPUT);
  pinMode(PIN_L0, INPUT);
  pinMode(PIN_L1, INPUT);
  pinMode(PIN_BUT,INPUT_PULLUP);
  pinMode(PIN_ERR, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(PIN_BUT),button_ISR,FALLING);

  
  Serial.begin(9600);
}

void loop() {

//read up
 l0_val = digitalRead(PIN_L0);
 l1_val = digitalRead(PIN_L1);

  //required for time logging
t.update();

//check for buildup
if(!l0_val || !l1_val){
  err_cnt ++;
  if(err_cnt > 100000){
    digitalWrite(PIN_ERR,HIGH);
  }
}


    //figure out if the vole is on the move and update location
  
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
  digitalWrite(PIN_Y, HIGH);
  digitalWrite(PIN_G,LOW);

 }
 else{
  digitalWrite(PIN_Y, LOW);
  digitalWrite(PIN_G,HIGH);
 }



 //disp time
if(butflg == 1){
 Serial.print(cnt_0);
 Serial.print("   ");
 Serial.println(cnt_1);
 butflg = 0;
}

}

    //samples at 1 Hz, increments a counter depending on location of the animal.
void time_tracker_ISR(){
  if(location == 0){
    cnt_0 ++;
  }
  if(location == 1){
    cnt_1 ++;
  }
}


    //activates an event flag to signal the main loop to print time data to the serial monitor
      //will soon be obsolete after SSD integration
void button_ISR(){
  butflg = 1;
}








