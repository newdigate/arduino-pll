#include <Bounce.h>
#define BUTTON1 10  //NEXT
Bounce debouncer = Bounce(BUTTON1, 50);

int freq_counter = 0;
long duration = 0, prev_duration = 0;
long start_time = -1, synthedsized_start_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    delay(5);
  }
  Serial.println("Frequency counter");

  pinMode(BUTTON1, INPUT_PULLUP);

}

bool periodExpired = false; 

void loop() {
  
  // put your main code here, to run repeatedly:
  long m = micros();
  
  debouncer.update();
  if (debouncer.risingEdge()) {
    if (start_time == -1) {
      start_time = 0;
    } else
    if (start_time == 0) {
      start_time = m;
      Serial.println(".");
    } else
    {
          
      long duration2 = m - start_time;
      if (duration2 > 2000000){
        // threshold period of time has elapsed without a trigger
        // discard, and start again
        Serial.println("restart");
      } else {

        duration = duration2;
        Serial.println(duration/1000);
        float period_s = duration/1000000.0;
        //Serial.println(period_s);
        float bpm = 60 / period_s;
        Serial.println(bpm);
      }
      start_time = m;
    }
  } else if (!periodExpired) {
    if (duration > 0 && m >= start_time + duration) {
      Serial.print("=");
      //Serial.print(duration/1000, DEC);
      periodExpired = true;
      synthedsized_start_time = m;
    }
  } else if (periodExpired) {
    if (m >= synthedsized_start_time + duration) {
      Serial.print("+");
      //Serial.print(duration/1000, DEC);
      synthedsized_start_time = m;
    }    
  } 
}
