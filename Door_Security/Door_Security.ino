#include <Keypad.h>
#include <LiquidCrystal.h>
#include <pitches.h>
//#define melody 3
LiquidCrystal lcd(31,33,35,37,39,41);
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
//char customeKey;
//String buff;
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad
Keypad cusomKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char PIN[6]={'1','2','A','D','5','6'}; // our secret (!) number
char attempt[6]={
  0,0,0,0,0,0}; // used for comparison
int z=0;
void setup(){
  lcd.begin(16,2);
  lcd.noAutoscroll();
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("  Enter PIN...");
  Serial.print("Set the password ");
  cusomKeypad.setDebounceTime(20);

}
  
void loop(){
  //char customKey = cusomKeypad.getKey();
 //String buffer=customKey;
  //initLCDKeys();
 // char buff =customKey;
 // String KeKe = buff;
  //if (customKey != NO_KEY){
 // lcd.setCursor(0,1);
 // lcd.print(buff);
 // Serial.print(customKey);
 readKeypad();
 // }
}
void checkPIN()
{
  int correct=0;
  for (int q=0; q<6; q++)
  {
    if (attempt[q]==PIN[q])
    {
      correct++;
    }
  }
  if (correct==6)
  {
    correctPIN();
  } else
  {
    incorrectPIN();
  }
  for (int zz=0; zz<6; zz++) // wipe attempt
  {
    attempt[zz]=0;
  }
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
}
void correctPIN() // do this if correct PIN entered
{
  lcd.print("* Correct PIN *");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}

void incorrectPIN() // do this if incorrect PIN entered
{
  lcd.print(" * Try again *");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}
