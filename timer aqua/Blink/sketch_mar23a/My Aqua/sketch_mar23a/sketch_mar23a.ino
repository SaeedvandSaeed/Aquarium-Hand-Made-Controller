 #include <TimerOne.h>
 #include <EEPROM.h>
// #include "SevenSegmentTM1637.h"
 #include "SevenSegmentExtended.h"

const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
const byte Yellow_LED = 9;
const byte Buzzer = 10;

int addr = 0;

int i=0;
volatile int h=0; 
volatile int m=01; 
volatile int s=10; 
volatile int ms=99; 
volatile int store=0; 
volatile int days=0; 
volatile int Resolution=4; 
//volatile int LED_Res=0; 

//int TIME; 
//const int hs=8; 
//const int ms=9; 
//const int ledPin=LED_BUILTIN;

const int EX_Status = 5;
const int L1=6;
const int L2=7;
const int PWM=9;
volatile byte state = LOW;
//----------------------------
const byte PIN_CLK = 4;   // define CLK pin (any digital pin)
const byte PIN_DIO = 5;   // define DIO pin (any digital pin)
//SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);
SevenSegmentExtended    display(A4, A5);
//***********************************************************
void setup() 
{
 // Serial.begin(9600);  
 
  noInterrupts();
  m = EEPROM.read(0);
  h = EEPROM.read(1);
  days = EEPROM.read(2);
  interrupts();
  
  delay(500);                // wait 1000 ms
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PWM, OUTPUT);

  digitalWrite(L1, LOW); 
  digitalWrite(L2,  LOW); 
  digitalWrite(Buzzer,  HIGH); 
 
  pinMode(EX_Status, INPUT_PULLUP);
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin1), PB_UP, FALLING );
  attachInterrupt(digitalPinToInterrupt(interruptPin2), PB_DOWN, FALLING );

  for(i=0; i< 2;i++)
  {
    digitalWrite(Yellow_LED, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(Buzzer, LOW);  
    delay(250);
    digitalWrite(Yellow_LED, LOW);
    digitalWrite(LED_BUILTIN, LOW);  
    digitalWrite(Buzzer, HIGH);  
    delay(250); 
  }
  
  Timer1.initialize(200);
  Timer1.attachInterrupt(Tick);
  Timer1.setPeriod(10000); //10000 is for 1s

  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
 // display.print("INIT");      // display INIT on the display
  display.clear();  
}
//***********************************************************
void loop() 
{

}
//***********************************************************
void Tick()
{
    ms--;
    if(ms < 0)
    {        
      s--;
      ms=99;

      if(h == 0 && m == 0)
        display.printTime(m, s, true);  
      else
        display.printTime(h, m, true);
        
      state = !state; 
      digitalWrite(LED_BUILTIN, state);
      
    /*  display.setColonOn(false); 
        display.print(222);
        display.print( display.getColonOn());
        display.printDualCounter(h, s);           
        display.blink();    
        display.print(s);  /*
    /*  
      Serial.print(h);  Serial.print(": ");
      Serial.print(m);  Serial.print(": ");
      Serial.print(s);  Serial.print(": Day:");
      Serial.print(days);  Serial.print(".");
      Serial.println("");*/
    }
    if(s < 0)
    { 
       s=59; 
       m--; 

       EEPROM.write(0, m);
       EEPROM.write(1, h);
       EEPROM.write(2, days);

    } 
    if(m < 0) 
    { 
     // display.clear(); 
       m=59; 
       h--; 
    } 
    if(h < 0) 
    { 
    //  display.clear(); 
       h=11; 
       days++;
    } 
    //----------------------------------------------------
  /*  if(digitalRead(EX_Status)==LOW) //test mode swithch
    {
      
      /*if(s%10 == 0 && ms == 1)
      { 
         store=s;
         noInterrupts();
         digitalWrite(L1, HIGH); 
         digitalWrite(PWM, HIGH); 
         digitalWrite(Yellow_LED, HIGH); 
         interrupts(); 
      }
      if(s%10 == 0 && ms == 65)
      {
         noInterrupts();
         digitalWrite(L1, LOW);
         digitalWrite(PWM, LOW);   
         digitalWrite(Yellow_LED, LOW);
         interrupts();  
      }
    }
    else
    {*/
      int act_h=0;
      int act_m=0;
      
      if(h == act_h && m== act_m && s == 10 && ms == 80) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(h == act_h && m== act_m && s == 10 && ms == 5) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 

      if(h == act_h && m== act_m && s == 9 && ms == 90) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(h == act_h && m== act_m && s == 9 && ms == 70) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 

      if(h == act_h && m== act_m && s == 9 && ms == 50) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(h == act_h && m== act_m && s == 9 && ms == 30) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 

      if(h == act_h && m== act_m && s == 9 && ms == 10) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(h == act_h && m== act_m && s == 8 && ms == 90) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 

      //---------------------------------------------
      if(h == act_h && m == act_m && s == 1 && ms == 65)
      { 
         noInterrupts();
         digitalWrite(L1, HIGH); 
         digitalWrite(PWM, HIGH); 
         digitalWrite(Yellow_LED, HIGH);  
         interrupts();    
      }
      if(h == act_h && m == act_m && s == 1 &&  ms == 1)
      { 
         noInterrupts();
         digitalWrite(L1, LOW);
         digitalWrite(PWM, LOW);   
         digitalWrite(Yellow_LED, LOW);
         interrupts();    
      }   
}
//***********************************************************
void PB_UP()  //its up side button (purs the feedings)
{ 
   if(digitalRead(EX_Status)==LOW) // to reset the time to day 1, 00:00 (if deep swich is ON)
   {
       noInterrupts();   
       digitalWrite(Yellow_LED, HIGH);
       while(digitalRead(interruptPin1)==LOW);
       digitalWrite(Yellow_LED, LOW);
       m++;
       if(m >= 60)
          m = 0;
       EEPROM.write(0, m); //m
       EEPROM.write(1, h); //h
       EEPROM.write(2, 1); //days
       interrupts();  
   }
   else
   {
       digitalWrite(Yellow_LED, HIGH);
       digitalWrite(L1, HIGH); 
       digitalWrite(PWM, HIGH); 
   
       while(digitalRead(interruptPin1)==LOW);
    
       digitalWrite(L1, LOW);
       digitalWrite(PWM, LOW);   
       digitalWrite(Yellow_LED, LOW);
   }


}
//***********************************************************
void PB_DOWN() 
{
   if(digitalRead(EX_Status)==LOW) // to reset the time to day 1, 00:00 (if deep swich is ON)
   {
       noInterrupts(); 
       digitalWrite(Yellow_LED, HIGH);
       while(digitalRead(interruptPin1)==LOW);
       digitalWrite(Yellow_LED, LOW);
       h++;
       if(h>12) 
          h=0;
       EEPROM.write(0, m); //m
       EEPROM.write(1, h); //h
       EEPROM.write(2, 1); //days

      // m = EEPROM.read(0);
     //  h = EEPROM.read(1);
     //  days = EEPROM.read(2);
       interrupts();  
   }
   else
   {
       digitalWrite(Yellow_LED, HIGH);
       digitalWrite(L2, HIGH); 
       digitalWrite(PWM, HIGH); 
       
       while(digitalRead(interruptPin2)==LOW);   
    
       digitalWrite(L2, LOW);
       digitalWrite(PWM, LOW);   
       digitalWrite(Yellow_LED, LOW);
   }
}
//***********************************************************
