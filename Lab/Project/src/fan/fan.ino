/*
  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
*/

// include the library
#include <LiquidCrystal.h>
#include "pitches.h"

// notes in the melody : super mario

    int melodyMario[] = {

    NOTE_E4, NOTE_E4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G3,  
    NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3,
    NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3,
    NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_GS3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_GS3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_C4,
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_G3

    };

    // note durations: 4 = quarter note, 8 = eighth note, etc.:

    int noteDurationsMario[] = {
    8,4,4,8,4,2,2,
    3,3,3,4,4,8,4,8,8,8,4,8,4,3,8,8,3,
    3,3,3,4,4,8,4,8,8,8,4,8,4,3,8,8,2,
    8,8,8,4,4,8,8,4,8,8,3,8,8,8,4,4,4,8,2,
    8,8,8,4,4,8,8,4,8,8,3,3,3,1,
    8,4,4,8,4,8,4,8,2,8,4,4,8,4,1,
    8,4,4,8,4,8,4,8,2

    };

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buzzer = 3;//蜂鳴器
const int potPin = 0; //定義類比端口0 連接LM35 溫度感測器
const int IRPin = 3; //紅外線訊號

const int motor[] = {10, 11};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);//設置Baud rate
  pinMode(IRPin, INPUT);//紅外線
  for(int i = 0; i < 2; i++){
    pinMode(motor[i], OUTPUT);
  }
  music();
}

void loop() {
  // set the cursor to column 0, line 0
  // (note: line 0 is the first row, since counting begins with 0):
  lcd.setCursor(0, 0);

  int val;//定義變數
  int dat;//定義變數

  val=analogRead(potPin);// 讀取感測器的模擬值並傳值給val

  dat=(125*val)>>8;   //溫度計算公式     125/2^8 = 125/256=0.4882812
dat -= 15;
  //Serial.print(dat);          //輸出顯示dat 的值
  //Serial.println(" Celsius");    //輸出顯示Celsius字串*/
  lcd.print(dat);
  lcd.print(" Celsius");

  if(dat>40 || digitalRead(IRPin)==LOW) {
    fan_run();
  }
  else {
    motor_stop();
  }
  
  delay(1000);//延時1秒
}

void motor_stop(){
  digitalWrite(motor[0], LOW); 
  digitalWrite(motor[1], LOW); 
  delay(25);
}

void fan_run(){
  digitalWrite(motor[0], LOW); 
  digitalWrite(motor[1], HIGH); 
}

void music(){
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 20; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurationsMario[thisNote];
    tone(2, melodyMario[thisNote],noteDuration);


    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    noTone(2);
    }
}

/*
Tone generator
v1  use timer, and toggle any digital pin in ISR
   funky duration from arduino version
   TODO use FindMckDivisor?
   timer selected will preclude using associated pins for PWM etc.
    could also do timer/pwm hardware toggle where caller controls duration
*/


// timers TC0 TC1 TC2   channels 0-2 ids 0-2  3-5  6-8     AB 0 1
// use TC1 channel 0 
#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn

// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
//  piano 27Hz to 4KHz

static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
    const uint32_t rc = VARIANT_MCK / 256 / frequency; 
    tone_pin = ulPin;
    toggle_count = 0;  // strange  wipe out previous duration
    if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
     else toggle_count = -1;

    if (!TCChanEnabled) {
      pmc_set_writeprotect(false);
      pmc_enable_periph_clk((uint32_t)TONE_IRQ);
      TC_Configure(chTC, chNo,
        TC_CMR_TCCLKS_TIMER_CLOCK4 |
        TC_CMR_WAVE |         // Waveform mode
        TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
  
      chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
      chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
       NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
    }
    if (!pinEnabled[ulPin]) {
      pinMode(ulPin, OUTPUT);
      pinEnabled[ulPin] = 1;
    }
    TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);    // set frequency
    TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
  TC_Stop(chTC, chNo);  // stop timer
  digitalWrite(ulPin,LOW);  // no signal on pin
}

// timer ISR  TC1 ch 0
void TC3_Handler ( void ) {
  TC_GetStatus(TC1, 0);
  if (toggle_count != 0){
    // toggle pin  TODO  better
    digitalWrite(tone_pin,pin_state= !pin_state);
    if (toggle_count > 0) toggle_count--;
  } else {
    noTone(tone_pin);
  }
}
