#include <Keypad.h>
#include <Timer.h>
#include <LiquidCrystal.h>
#include <pitches.h>
LiquidCrystal lcd(31,33,35,37,39,41);
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad
Keypad cusomKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char PIN[6]={'1','2','A','D','5','6'}; // our secret (!) number
char attempt[6]={0,0,0,0,0,0}; // used for comparison
int z=0;
int v=1;
unsigned int EchoPin = 4;           // connect Pin 2(Arduino digital io) to Echo/RX at US-100
unsigned int TrigPin = 5;           // connect Pin 3(Arduino digital io) to Trig/TX at US-100
unsigned long Time_Echo_us = 0;
unsigned long Len_mm  = 0;
Timer t2;
int counting;

void setup(){
  pinMode(3,OUTPUT);
  t2.oscillate(3,1000,HIGH);
  pinMode(EchoPin, INPUT);                    //Set EchoPin as input, to receive measure result from US-100
  pinMode(TrigPin, OUTPUT);                   //Set TrigPin as output, used to send high pusle to trig measurement (>10us)
  lcd.begin(16,2);
  lcd.noAutoscroll();
  Serial.begin(9600);
  lcd.setCursor(0,0);
  Serial.print("Set the password ");
  cusomKeypad.setDebounceTime(20);
  counting=0;
}
  
void loop(){ 
  
Start:   ultrasonic();
        readKeypad();

}
void readKeypad(){
  char key = cusomKeypad.getKey();
  if (key != NO_KEY)
  {
    switch(key)
    {
    case 'C':
      z=0;
      break;
    case 'F':
      delay(100); // for extra debounce
      lcd.clear();
      checkPIN();
      break;
    default:
      attempt[z]=key;
      z++;    }
  }
  else{
    t2.update();
    counting++;
    
    if(counting==10){
      
      Serial.println("++");
      Serial.println(counting);
     // break;*/
      
  }
}
}
void ultrasonic(){
  while(v==1){
        digitalWrite(TrigPin, HIGH);              //begin to send a high pulse, then US-100 begin to measure the distance
    delayMicroseconds(50);                    //set this high pulse width as 50us (>10us)
    digitalWrite(TrigPin, LOW);               //end this high pulse
    
    Time_Echo_us = pulseIn(EchoPin, HIGH);               //calculate the pulse width at EchoPin, 
    if((Time_Echo_us < 60000) && (Time_Echo_us > 1))     //a valid pulse width should be between (1, 60000).
    {
      Len_mm = (Time_Echo_us*34/100)/2;      //calculate the distance by pulse width, Len_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm)
      Serial.print("Present Distance is: ");  //output result to Serial monitor
      Serial.print(Len_mm, DEC);            //output result to Serial monitor
      Serial.println("mm");                 //output result to Serial monitor
      if(Len_mm <= 100){
        v--;
        lcd.print("  Enter PIN...");
      }
    }
    delay(1000);                            //take a measurement every second (1000ms)
  }
}
  
void checkPIN(){
  int correct=0;
  for (int q=0; q<6; q++)
 {
   if(attempt[q]==PIN[q]) {
      correct++;
    }
 } 
   if(correct==6){
    correctPIN();
  } 
  else {
    incorrectPIN();
  }
  for(int zz=0; zz<6; zz++){ // wipe attempt
    attempt[zz]=0;
  }
}
void correctPIN() // do this if correct PIN entered
{
  lcd.print("* Correct PIN *");
  delay(1000);
  v++;
  z=0;
  char attempt[6]={0,0,0,0,0,0};
  lcd.clear();
 // lcd.print("  Enter PIN...");
}

void incorrectPIN() // do this if incorrect PIN entered
{
  lcd.print(" * Try again *");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}
