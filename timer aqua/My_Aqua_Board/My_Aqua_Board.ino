

 #include <TimerOne.h>
 #include <EEPROM.h>
 #include "SevenSegmentExtended.h"
 #define Lights A3
 #define CO2 A2
 

const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
const byte Yellow_LED = 9;
const byte Buzzer = 10;

volatile short h=0; 
volatile short m=01; 
volatile short s=10; 
volatile short ms=99; 
//volatile short days=0; 

volatile byte Safty_counter=0; 
volatile byte Night;
volatile byte delay_seconds=3; 
volatile byte day_time_duration = 8;

const byte EX_Status = 5;
const byte L1=6;
const byte L2=7;
const byte PWM=9;
volatile byte state = LOW;
//----------------------------
SevenSegmentExtended    display(A4, A5); //(PIN_CLK, PIN_DIO)
//***********************************************************
void setup() 
{
 // Serial.begin(9600);  

  noInterrupts();
  m = EEPROM.read(0);
  h = EEPROM.read(1);
  //days = EEPROM.read(2);
  Night = EEPROM.read(3);
  
  interrupts();
  
  delay(500);                // wait 1000 ms
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Lights, OUTPUT);
  pinMode(CO2, OUTPUT);
  
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PWM, OUTPUT);

  digitalWrite(L1, LOW); 
  digitalWrite(L2,  LOW); 
  digitalWrite(Buzzer,  HIGH); 
  digitalWrite(Lights,  LOW); 
  digitalWrite(CO2,  LOW); 
     
  pinMode(EX_Status, INPUT_PULLUP);
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin1), PB_UP, FALLING );
  attachInterrupt(digitalPinToInterrupt(interruptPin2), PB_DOWN, FALLING );

  for(int i=0; i< 2;i++)
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
    
    // feed times   
    byte Hr1 = 0;    
    byte Min1 = 5; // feed five minutes before and after lighting
    
    byte Hr2 = day_time_duration - 1;   
    byte Min2 = 55;
       
    if(ms < 0) // each second
    {        
      s--;
      ms=99;

      //---------lights------------
      if(Night == HIGH)
      {   digitalWrite(Lights, LOW);    }
      else if(Night == LOW)
      {   digitalWrite(Lights, HIGH);   }
      //----------Co2--------------
      if(h == 0 && Night == LOW) //stop Co2 one our earlier 
      {   digitalWrite(CO2, LOW);       }    
      else if(h == 0 && Night == HIGH) //open Co2 one our earlier 
      {   digitalWrite(CO2, HIGH);      }  
      else if(Night == LOW) //stop Co2 one our earlier 
      {   digitalWrite(CO2, HIGH);       }  
      else if(Night == HIGH) //stop Co2 one our earlier 
      {   digitalWrite(CO2, LOW);       }  
      //---------------------------
      
      if(h == 0 && m == 0 && delay_seconds == 0)
        display.printTime(m, s, true);  
      else if(m == 0 && delay_seconds == 0)
        display.printTime(h, s, true);
      else if (delay_seconds == 0)
        display.printTime(h, m, true);
      else 
      {
        delay_seconds--;
      }
      state = !state; 
      digitalWrite(LED_BUILTIN, state);
  
      Safty_counter++;
      if(Safty_counter >= 4) // for safty and cheking puring is off
      { 
         noInterrupts();
         Safty_counter = 0;
         digitalWrite(L1, LOW);
         digitalWrite(PWM, LOW);   
       //  digitalWrite(Yellow_LED, LOW);
         interrupts();    
      }   
    }
    if(s < 0)
    { 
       s=59; 
       m--; 
     
      //if(Night == LOW && m > 0 && m < 59)
      //{ display.print("L On");  delay_seconds = 3;}
      //else if (m > 0 && m < 59)
      //{ display.print("LOFF");  delay_seconds = 3;}
      
       EEPROM.write(0, m);
       EEPROM.write(1, h);
       //EEPROM.write(2, days);
       EEPROM.write(3, Night); //Sun status
    } 
    if(m < 0) 
    { 
       m=59; 
       h--; 
    } 
    if(h < 0) 
    { 
      if(Night == HIGH)
        {h = day_time_duration - 1;}
      else
        {h = 24 - day_time_duration - 1;}
        
      Night = !Night;    
    //  days++;
    } 
    //----------------------------------------------------
  
    if( Night == LOW)
    {     
      //---------------------------------------------
      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 1 && ms == 23) //pour
      { 
         noInterrupts();
         digitalWrite(L1, HIGH); 
         digitalWrite(PWM, HIGH); 
         digitalWrite(Yellow_LED, HIGH);  
         interrupts();    
      }
      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 1 &&  ms == 1) //stop
      { 
         noInterrupts();
         digitalWrite(L1, LOW);
         digitalWrite(PWM, LOW);   
         digitalWrite(Yellow_LED, LOW);
         interrupts();    
      }  
      
      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 10 && ms == 80) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 10 && ms == 5) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 

      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 9 && ms == 90) //buzzer on
      {noInterrupts();          digitalWrite(Buzzer, LOW);          digitalWrite(Yellow_LED, HIGH);           interrupts(); }    
      if(((h == Hr1 && m== Min1) || (h == Hr2 && m== Min2)) && s == 9 && ms == 70) //buzzer off
      {noInterrupts();          digitalWrite(Buzzer, HIGH);          digitalWrite(Yellow_LED, LOW);           interrupts();} 


    } 
}
//***********************************************************
void PB_UP()  //its up side button (purs the feedings)
{ 
   int button_counter=0; 
   noInterrupts();   
   button_counter = s;
   digitalWrite(Yellow_LED, HIGH);
   button_counter = 0;
   while(digitalRead(interruptPin1)==LOW || button_counter < 32000) { digitalRead(0); button_counter++; };
   digitalWrite(Yellow_LED, LOW);
   interrupts();  
        
   if(digitalRead(EX_Status)==LOW && button_counter != s) // (if deep swich is ON)
   {
       m++;
       if(m >= 60)
          m = 0;     
   }
   else
   {
       m--;
       if(m <= 0)
          m = 60;
   }
  save_eeprom();
}
//***********************************************************
void PB_DOWN() 
{
   int button_counter=0; 
   
   noInterrupts(); 
   button_counter = s;
   digitalWrite(Yellow_LED, HIGH);
   
   button_counter = 0;
   while(digitalRead(interruptPin1)==LOW || button_counter < 32000) { digitalRead(0); button_counter++; };  
   digitalWrite(Yellow_LED, LOW);
   interrupts();
       
   if(digitalRead(EX_Status)==LOW && button_counter != s) // to reset the time to day 1, 00:00 (if deep swich is ON)
   {        
       h++;

       if((Night == HIGH && h > 24 - day_time_duration) || (Night == LOW && h > day_time_duration - 1)) 
       {
          h=0;
          Night = !Night;
          delay_seconds = 3;
       }

   }
   else // to manual pour
   { 
       h--;

       if(h<0)
       {
         if(Night == HIGH ) 
         {            h = day_time_duration - 1;        }
         else
         {            h = 24 - day_time_duration;         }
         Night = !Night;
         delay_seconds = 3;
         }
    }
}
void save_eeprom()
{
    noInterrupts(); 
    EEPROM.write(0, m); //m
    EEPROM.write(1, h); //h
    // EEPROM.write(2, 1); //days
    EEPROM.write(3, Night); //Sun status
    interrupts();  
}   

//***********************************************************
