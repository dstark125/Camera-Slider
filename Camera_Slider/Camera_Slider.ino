#include <AccelStepper.h>
#include <LiquidCrystal.h>

//************* LCD Stuff *************

#define PIN_SCE   11 // SCE - Chip select, pin 3 on LCD.
#define PIN_RESET 10 // RST - Reset, pin 4 on LCD.
#define PIN_DC    9 // DC - Data/Command, pin 5 on LCD.
#define PIN_SDIN  8 // DN(MOSI) - Serial data, pin 6 on LCD.
#define PIN_SCLK  7 // SCLK - Serial clock, pin 7 on LCD.

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48

#define LOWER_BUTTON_PIN 13
#define UPPER_BUTTON_PIN 12

#define FULL4WIRE 4

// Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1


char lcdCharArr[73] = "Starting...";

static const byte ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};

typedef enum screens_e{
  SCREEN_STARTSTOP,
	SCREEN_STATUS,
	SCREEN_SPEED,
	SCREEN_POS,
	SCREEN_SAVE,
} screens_t;

typedef enum buttonPress_e{
  BUT_NONE,
  BUT_UP,
  BUT_DOWN,
  BUT_BOTH
} buttonPress_t;

//************* MOTOR Stuff *************

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(FULL4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

//************* Status Stuff *************
bool isRunning = false;
unsigned long cycleTime = 600;
unsigned long lengthInSteps =  10000; //36mm dia, 600mm rods, 4096 steps/rotation
unsigned long curPos = 0; 
unsigned long startTime;
float stepsPerSecond = lengthInSteps / cycleTime;

//************* Other Pins *************

volatile unsigned long diff = 0;
volatile uint8_t timerFlag = 0;
//************* Setup *************

void setup() {
  pinMode(LOWER_BUTTON_PIN, INPUT);
  pinMode(UPPER_BUTTON_PIN, INPUT);

  stepper.setMaxSpeed(500.0);
  
  Serial.begin(115200);
  delay(300);

  LcdInitialise();
  LcdClear();
  delay(100);
  LcdPrint(lcdCharArr);
  delay(100);
}//--(end setup )---

//************* Loop *************

void loop() {
  static screens_t screen = SCREEN_STATUS;

  //LcdClear();
  //_ClearLcdCharArr();
  Serial.println("Showing menu");
  screen = _ShowMenu();
  _Debounce();
  
  switch (screen){
	case(SCREEN_STATUS):
		Serial.println("Showing status");
		_ShowStatusScreen();
		break;
  case(SCREEN_STARTSTOP):
    Serial.println("Toggling");
    _ToggleStartStop();
    if (isRunning){
      _ShowStatusScreen();
    }
    break;
  case(SCREEN_POS):
    Serial.println("Setting new position");
    _SetNewPos();
    break;
  case(SCREEN_SPEED):
    Serial.println("Setting new position");
    _SetNewDuration();
    break;
  }  
}

//************* Button Helpers *************

buttonPress_t _GetButtonPressType(uint16_t frame){
  unsigned long strtT = millis();
  bool lowBPressed = digitalRead(LOWER_BUTTON_PIN);
  bool uppBPressed = digitalRead(UPPER_BUTTON_PIN);
  bool bothPressed = false;
  buttonPress_t stat = BUT_NONE;
  
  if(lowBPressed && uppBPressed){
    return BUT_BOTH;
  }
  else if(lowBPressed){
    stat = BUT_DOWN;
    while ((millis() - strtT) < frame){
      if (digitalRead(UPPER_BUTTON_PIN)){
        stat = BUT_BOTH;
        break;
      }
      else if (digitalRead(LOWER_BUTTON_PIN) == LOW){ //Check for release
        break;
      }
    }
  }
  else if(uppBPressed){
    stat = BUT_UP;
    while ((millis() - strtT) < frame){
      if (digitalRead(LOWER_BUTTON_PIN)){
        stat = BUT_BOTH;
        break;
      }
      else if (digitalRead(UPPER_BUTTON_PIN) == LOW){ //Check for release
        break;
      }
    }
  }
  return stat;
}

void _Debounce(){
    while (digitalRead(LOWER_BUTTON_PIN) || digitalRead(UPPER_BUTTON_PIN)){
      //Nothing
    }
    return;
}


//************* Stepper Helpers *************


//************* Screen Helpers *************

char _spinChar(){
  static char c = '|';
  if (c == '-'){
    c = '|';
  }
  else{
    c = '-';
  }
}

//----------_ToggleStartStop()----------
void _ToggleStartStop(){
  isRunning = !isRunning;
  if (isRunning){
    LcdPrint("Started!");
    stepper.reset();
    stepper.setSpeed(100.0);
    setTimer2Frequency(400);
    startTime = millis();
  }
  else{
    _disableTimer2Interrupts();
    LcdPrint("Stopped!");
  }
  delay(1000);
}

//----------_SetNewPos()----------
void _SetNewPos(){
  buttonPress_t pressType = BUT_NONE;
  long totalSteps = 0;
  long stepsThisTime = 0;
  char * buff = lcdCharArr;
  char lineBuff[13] = "";
  bool change = false;
  
  isRunning = false;
  _disableTimer2Interrupts();
  _ClearLcdCharArr();
  
  LcdPrint("Initializing");
    
  buff =  _PutHeaderInBuffer(buff, "Set Pos");
  buff = _PutLineInBuffer(buff, "Set starting");
  buff = _PutLineInBuffer(buff, "position.");
  sprintf(lineBuff,"Delta:%6i",totalSteps);
  _PutLineInBuffer(buff, lineBuff); //Don't increment the buffer so we can rewrite to the same spot
  LcdPrint(lcdCharArr);
  
  _Debounce();
  
  while(pressType != BUT_BOTH){
    pressType = _GetButtonPressType(500);
    switch(pressType){
      case(BUT_UP):
        stepper.reset();
        stepsThisTime = 0;
        stepper.setSpeed(400.0);
        while(_GetButtonPressType(0) == BUT_UP){
          if (stepper.runSpeed()){
            stepsThisTime++;
          }
        }
        totalSteps += stepsThisTime;
        change = true;
        break;
      case(BUT_DOWN):
        stepper.reset();
        stepsThisTime = 0;
        stepper.setSpeed(-400.0);
        while(_GetButtonPressType(0) == BUT_DOWN){
          if (stepper.runSpeed()){
            stepsThisTime--;
          }
        }
        totalSteps += stepsThisTime;
        change = true;
        break;
    }
    if(change){
      change = false;
      sprintf(lineBuff,"Delta:%6i",totalSteps);
      _PutLineInBuffer(buff, lineBuff); //Don't increment the buffer so we can rewrite to the same spot
      LcdPrint(lcdCharArr);
    }
  }

  Serial.println("Got start, getting end");

  curPos = 0; //We are now at the starting position
  pressType = BUT_NONE;
  totalSteps = 0;
  stepsThisTime = 0;
  
  buff = lcdCharArr; //Start over
  buff =  _PutHeaderInBuffer(buff, "Set Pos");
  buff = _PutLineInBuffer(buff, "Set ending");
  buff = _PutLineInBuffer(buff, "position.");
  sprintf(lineBuff,"Delta:%6i",totalSteps);
  _PutLineInBuffer(buff, lineBuff); //Don't increment the buffer so we can rewrite to the same spot
  LcdPrint(lcdCharArr);
  
  Serial.println("Debouncing");
  
  _Debounce();
  
  while(pressType != BUT_BOTH){
    pressType = _GetButtonPressType(500);
    switch(pressType){
      case(BUT_UP):
        stepper.reset();
        stepsThisTime = 0;
        stepper.setSpeed(400.0);
        while(_GetButtonPressType(0) == BUT_UP){
          if (stepper.runSpeed()){
            stepsThisTime++;
          }
        }
        totalSteps += stepsThisTime;
        change = true;
        break;
      case(BUT_DOWN):
        stepper.reset();
        stepsThisTime = 0;
        stepper.setSpeed(-400.0);
        while(_GetButtonPressType(0) == BUT_DOWN){
          if (stepper.runSpeed()){
            stepsThisTime--;
          }
        }
        totalSteps += stepsThisTime;
        change = true;
        break;
    }
    if(change){
      change = false;
      sprintf(lineBuff,"Delta:%6i",totalSteps);
      _PutLineInBuffer(buff, lineBuff); //Don't increment the buffer so we can rewrite to the same spot
      LcdPrint(lcdCharArr);
    }
  }

  lengthInSteps = totalSteps; //Save total steps
  
  //Now, wind backwards
  LcdPrint("Rewinding..");
  stepper.reset();
  stepper.setSpeed(-400.0);
  stepsThisTime = totalSteps;
  Serial.print("Rewinding ");
  Serial.print(stepsThisTime);
  Serial.println(" steps");
  while (stepsThisTime != 0){
    if (stepper.runSpeed()){
        stepsThisTime--;
        
    }
  }
}

//----------_SetNewDuration()----------
void _SetNewDuration(){
  char * buff = lcdCharArr;
  int8_t index = 0;
  buttonPress_t but;
  char curPointerBuff[13] = "";
  const char *menuItems[4] = {" Hours", " Minutes", " Seconds", " Done"};
  //char *menuBuffer[3] = {"            ", "            ", "            "};
  char menuBuffer[4][13];
  uint16_t curHour = 0;
  uint16_t curMin = 0;
  uint16_t curSec = 0;
  uint16_t *modPtr;
  bool canReturn = false;
  bool adjusting = true;
  
  Serial.println("Clearing LCD");
  _ClearLcdCharArr();

  //Serial.println("Filling buff");
  buff =  _PutHeaderInBuffer(buff, "Duration");
  sprintf(menuBuffer[0], "%s:%5u", menuItems[0], curHour);
  _SetCursorOnMenuItem(curPointerBuff, menuBuffer[0], '>');
  buff = _PutLineInBuffer(buff, curPointerBuff);
  sprintf(menuBuffer[1], "%s:%3u", menuItems[1], curMin);
  buff = _PutLineInBuffer(buff,menuBuffer[1]);
  sprintf(menuBuffer[2], "%s:%3u", menuItems[2], curSec);
  buff = _PutLineInBuffer(buff,menuBuffer[2]);
  sprintf(menuBuffer[3], "%s", menuItems[3]);
  buff = _PutLineInBuffer(buff,menuBuffer[3]);
  
  LcdPrint(lcdCharArr);
  _Debounce();

  while (!canReturn){
    but = _GetButtonPressType(500);
    if (but != BUT_NONE){
      _Debounce();
    }
    switch(but){
      case(BUT_UP):
        if (index > 0){
          buff = lcdCharArr + ((index + 1) * 12);
          _PutLineInBuffer(buff, menuBuffer[index]);
          index--;
          Serial.println(index);
          Serial.println(menuBuffer[index]);
          _SetCursorOnMenuItem(curPointerBuff, menuBuffer[index], '>');
          buff -= 12;
          _PutLineInBuffer(buff, curPointerBuff);
          LcdPrint(lcdCharArr);
        }
        break;
      case(BUT_DOWN):
        if (index < 3){
          buff = lcdCharArr + ((index + 1) * 12);
           _PutLineInBuffer(buff, menuBuffer[index]); 
           index++;
           Serial.println(index);
           Serial.println(menuBuffer[index]);
          _SetCursorOnMenuItem(curPointerBuff, menuBuffer[index], '>');
          buff += 12;
          _PutLineInBuffer(buff, curPointerBuff);
          LcdPrint(lcdCharArr);
        }
        break;
      case(BUT_BOTH):
        //START OF BUT_BOTH CASE
        if (index == 3){
          canReturn = true;
        }
        else{
          buff = lcdCharArr + ((index + 1) * 12);
          while(!canReturn && adjusting){
            but = _GetButtonPressType(500);
            switch(but){
            case(BUT_UP):
              switch(index){
              case(0):
                curHour++;
                sprintf(menuBuffer[index], "%c%s:%5u", _spinChar(), menuItems[index]+1, curHour);
                break;
              case(1):
                curMin++;
                sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curMin);
                break;
              case(2):
                curSec++;
                sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curSec);
                break;
              }
              _PutLineInBuffer(buff,menuBuffer[index]);
              LcdPrint(lcdCharArr);
              while(_GetButtonPressType(0) == BUT_UP){
                switch(index){
                case(0):
                  if (curHour < 96){
                    curHour++;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%5u", _spinChar(), menuItems[index]+1, curHour);
                  break;
                case(1):
                  if (curMin < 60){
                    curMin++;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curMin);
                  break;
                case(2):
                  if (curSec < 60){
                    curSec++;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curSec);
                  break;
                }
                delay(100);
                _PutLineInBuffer(buff,menuBuffer[index]);
                LcdPrint(lcdCharArr);
              }
              break;
            case(BUT_DOWN):
              switch(index){
              case(0):
                curHour--; 
                sprintf(menuBuffer[index], "%c%s:%5u", _spinChar(), menuItems[index]+1, curHour);
                break;
              case(1):
                curMin--; 
                sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curMin);
                break;
              case(2):
                curSec--;
                sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curSec);
                break;
              }
              _PutLineInBuffer(buff,menuBuffer[index]);
              LcdPrint(lcdCharArr);
              while(_GetButtonPressType(0) == BUT_DOWN){
                switch(index){
                case(0):
                  if (curHour != 0){
                    curHour--;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%5u", _spinChar(), menuItems[index]+1, curHour);
                  break;
                case(1):
                  if (curMin != 0){
                    curMin--;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curMin);
                  break;
                case(2):
                  if (curSec != 0){
                    curSec--;  
                  }
                  sprintf(menuBuffer[index], "%c%s:%3u", _spinChar(), menuItems[index]+1, curSec);
                  break;
                }
                delay(100);
                _PutLineInBuffer(buff,menuBuffer[index]);
                LcdPrint(lcdCharArr);
              }
              break;
            case(BUT_BOTH):
                Serial.println("Not adjusting");
                if (index == 0){
                  sprintf(menuBuffer[index], "%s:%5u", menuItems[index], curHour);
                }
                else if(index == 1 || index == 2){
                  sprintf(menuBuffer[index], "%s:%3u", menuItems[index], curHour);
                }
                _SetCursorOnMenuItem(curPointerBuff, menuBuffer[index], '>');
                _PutLineInBuffer(buff, curPointerBuff);
                LcdPrint(lcdCharArr);
                adjusting = false;
                _Debounce();
              break;
            case(BUT_NONE):
              //Nothing
              break;
            }
          }
          adjusting = true;
        }
        Serial.println("Breaking out of both button press case");
        break;
        //END OF BUT_BOTH CASE
      case(BUT_NONE):
        //Nothing
        break;
    }
  }
}

//----------_ShowStatusScreen()----------
void _ShowStatusScreen(){
	char * buff = lcdCharArr;
	char lineBuff[12] = "";
	unsigned long elapsedTime;
  int stepsToGo;
  unsigned long lastPrint = 0;
	buff =  _PutHeaderInBuffer(buff, "Status");
	if (isRunning){
		buff = _PutLineInBuffer(buff, "Running");
	}
	else{
		buff = _PutLineInBuffer(buff, "Stopped");
	}
  buff = _PutLineInBuffer(buff, "stp/s:------");
  buff = _PutLineInBuffer(buff, "To go:------");
  buff = _PutLineInBuffer(buff, "Elap:-------");
  buff = _PutLineInBuffer(buff, "Rem:--------");

  LcdPrint(lcdCharArr);

  _Debounce();
  
  if(isRunning){
    while (_GetButtonPressType(500) != BUT_BOTH){
      if ((millis() - lastPrint) > 1000){
        lastPrint = millis();
        
        buff = lcdCharArr + 24;
        sprintf(lineBuff, "stp/s:%6f", stepsPerSecond);
        buff = _PutLineInBuffer(buff, lineBuff);
    
        stepsToGo = lengthInSteps - curPos;
        sprintf(lineBuff,"To go:%6i",stepsToGo);
        buff = _PutLineInBuffer(buff, lineBuff);
    
        elapsedTime = millis() - startTime;
        sprintf(lineBuff, "Elap:%7i", (elapsedTime)/1000);
        buff = _PutLineInBuffer(buff, lineBuff);
        
        sprintf(lineBuff, "Rem:%8i", (int)(((float)elapsedTime/1000.0)*((float)stepsToGo/(float)(lengthInSteps-stepsToGo))));
        buff = _PutLineInBuffer(buff, lineBuff);
        
        LcdPrint(lcdCharArr);
      }
    }
  }
  else{
    Serial.println("Showing blank status");
    while (_GetButtonPressType(500) != BUT_BOTH){
      delay(100);
    }
  }
}

//----------_ClearLcdCharArr()----------
void _ClearLcdCharArr(){
  for(int i = 0; i < 72; i++){
    lcdCharArr[i] = 0;
  }
}

//************* Menu Helpers *************

//----------_ShowMenu()----------
screens_t _ShowMenu(void){
  char * buff = lcdCharArr;
  char startStopBuff[13] = "";
  char curPointerBuff[13] = "";
  int8_t index = -1;
  buttonPress_t but;
  const char *menuItems[4] = {" Status", " Set Time", " Set Limits", " Save Config"};
 
  Serial.println("Clearing LCD");
  _ClearLcdCharArr();

  Serial.println("Making start stop string");
  if (isRunning){
    sprintf(startStopBuff, " Stop");
  }
  else{
    sprintf(startStopBuff, " Start");
  }

  //Serial.println("Filling buff");
  buff =  _PutHeaderInBuffer(buff, "Menu");
  _SetCursorOnMenuItem(curPointerBuff, startStopBuff, '>');
  buff = _PutLineInBuffer(buff,curPointerBuff);
  buff =_PutLineInBuffer(buff, menuItems[0]);
  buff =_PutLineInBuffer(buff, menuItems[1]);
  buff =_PutLineInBuffer(buff, menuItems[2]);
  buff =_PutLineInBuffer(buff, menuItems[3]);
  //Serial.println("Printing");
  LcdPrint(lcdCharArr);
  _Debounce();
  //Serial.println("While loop");
  while (1){
    but = _GetButtonPressType(500);
    //Serial.println(but);
    if (but != BUT_NONE){
      _Debounce();
    }
    //Serial.println("Checking button press");
    //delay(500);
    switch(but){
      case(BUT_UP):
        if (index > 0){
          buff = lcdCharArr + ((index + 2) * 12);
          _PutLineInBuffer(buff, menuItems[index]);
          index--;
          _SetCursorOnMenuItem(curPointerBuff, menuItems[index], '>');
          buff -= 12;
          buff = _PutLineInBuffer(buff,curPointerBuff);
          LcdPrint(lcdCharArr);
        }
        else if (index == 0){ //Need to use the different buffer
          buff = lcdCharArr + ((index + 2) * 12);
          _PutLineInBuffer(buff, menuItems[index]);
          index--;
          _SetCursorOnMenuItem(curPointerBuff, startStopBuff, '>');
          buff -= 12;
          buff = _PutLineInBuffer(buff,curPointerBuff);
          LcdPrint(lcdCharArr);
        }
        break;
      case(BUT_DOWN):
        if (index < 3){
          buff = lcdCharArr + ((index + 2) * 12);
          if (index == -1){
            _PutLineInBuffer(buff, startStopBuff);
          }
          else{
           _PutLineInBuffer(buff, menuItems[index]); 
          }
          index++;
          _SetCursorOnMenuItem(curPointerBuff, menuItems[index], '>');
          buff += 12;
          buff = _PutLineInBuffer(buff,curPointerBuff);
          LcdPrint(lcdCharArr);
        }
        break;
      case(BUT_BOTH):
        switch(index){
  		  case(-1):
          return SCREEN_STARTSTOP;
        case(0):
  				return SCREEN_STATUS;
  			case(1):
  				return SCREEN_SPEED;
  			case(2):
  				return SCREEN_POS;
  			case(3):
  				return SCREEN_SAVE;
		}
      case(BUT_NONE):
        //Nothing
        break;
    }
  }
}

void _UpdateMenu(){
  char * buff = lcdCharArr;
  buff =  _PutHeaderInBuffer(buff, "Set Dist");
  sprintf(buff,"To go: %i",stepper.distanceToGo());
  LcdPrint(lcdCharArr);  
}

void _SetCursorOnMenuItem(char * buff, const char * item, char c){
  //strcpy(buff, "->");
  *buff = c;
  buff++; //Move buffer up past the >
  item++; //Move past the blank space on the item (overwrites first char)
  strncpy(buff, item, 11);
}

char * _PutHeaderInBuffer(char * buff, const char * msg){
  uint8_t len = strlen(msg);
  uint8_t startIndex  = (6 - (len/2));
  uint8_t i = 0;
  char thisBuff[] = "------------";

  strncpy(thisBuff+startIndex, msg, len);
  buff = _PutLineInBuffer(buff, thisBuff);

  return buff;
}


char * _PutLineInBuffer(char * buff, const char * msg){
  uint8_t len = strlen(msg);
  uint8_t i = 0;
  if (len > 12){
    len = 12; //Only 12 chars on string
  }
  for(i = 0; i < len; i++){
    *buff++ = *msg++;
  }
  for(i = 12-len; i > 0; i--){ //If space remaining, add spaces
    *buff++ = 32;
  }
  return buff;
}

char * _PutStringInBuffer(char * buff, char * msg){
  uint8_t len = strlen(msg);
  
  while(*msg != 0){
    *buff++ = *msg++;
  }
  return buff;
}

//************* LCD Helpers *************

//Work in progress
char * LcdFixSpaces(char *characters){
  int lastSpace = 0, totalCount = 0, charCount;
  
  while (*characters && totalCount < 73)
  {
    if (*characters == 32){

    }
    totalCount++;
    charCount++;
  }
}

void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  LcdWrite(LCD_C, 0xB6 );  // Set LCD Vop (Contrast). 
  LcdWrite(LCD_C, 0x05 );  // Set Temp coefficent. //0x04
  LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_C, 0x20 );  // LCD Basic Commands
  LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.

}

void LcdPrint(const char *characters){
  int i = 0, stillChars = 1;

  for (i = 0; i < 72; i++){
    if (stillChars){
      if (*characters == '\0'){
        stillChars = 0;
        lcdCharArr[i] = 32;
      }
      else{
        lcdCharArr[i] = *characters++;
      }
    }
    else{
      lcdCharArr[i] = 32;
    }
    
  }
  lcdCharArr[i] = '\0';
  
  LcdString(lcdCharArr);
  
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdWrite(byte dc, byte data)
{
  /* 
   *  #define PIN_SCE   11 // SCE - Chip select, pin 3 on LCD.
      #define PIN_RESET 10 // RST - Reset, pin 4 on LCD.
      #define PIN_DC    9 // DC - Data/Command, pin 5 on LCD.
      #define PIN_SDIN  8 // DN(MOSI) - Serial data, pin 6 on LCD.
      #define PIN_SCLK  7 // SCLK - Serial clock, pin 7 on LCD.
   */
   //PORTB = 8-13 (high bits 6 and 7 unused)
  PORTB = PORTB | (dc << 1); //digitalWrite(PIN_DC, dc);
  PORTB = PORTB & B110111;   //digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  PORTB = PORTB | B001000;   //digitalWrite(PIN_SCE, HIGH);
}


//************* ISR *************

//Seems as though ~78hz is the min frequency
void setTimer2Frequency(uint16_t freq){
  uint16_t preScaler = 64; //Start here, frequency should be under a large amount of KHZ
  uint8_t count = 1;
  uint32_t valNeeded = 0;
  bool found = false;
  
  while(!found && (preScaler <= 1024)){
    //Real clock is 16Mhz, but dividing by 100 so that the math will fit in the registers
    valNeeded = 160000 / ((float)(preScaler/10) * (float)(freq/10));
    Serial.print("Trying: ");
    Serial.println(valNeeded);
    if (valNeeded < 20 || valNeeded > 255){ //If it is not in appropriate range for our counter
      preScaler *= 4;
    }
    else{
      found = true;
    }
  }
  if(found){
    Serial.print("Frequency: ");
    Serial.println(freq);
    Serial.print("Prescaler: ");
    Serial.println(preScaler);
    Serial.print("Counter: ");
    Serial.println(valNeeded);
    count = (uint8_t)valNeeded;
  }
  else{
    Serial.print("Could not find a value for frequency: ");
    Serial.println(freq);
    Serial.print("Prescaler: ");
    Serial.println(preScaler);
    Serial.print("Counter: ");
    Serial.println(valNeeded);
    return;
  }
  
  
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0

  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  
  // set compare match register for increments
  OCR2A = valNeeded;
  
  /*Prescalers:
   * PreScale | CS22 | CS21 | CS20
   * 1          0      0      1
   * 8          0      1      0
   * 64         0      1      1
   * 256        1      0      0
   * 1024       1      0      1
   */
  switch(preScaler){
    case(1):
      TCCR2B |= (1 << CS20);
      break;
    case(8):
      TCCR2B |= (1 << CS21);
      break;
    case(64):
      TCCR2B |= (1 << CS21);
      TCCR2B |= (1 << CS20);
      break;
    case(256):
      TCCR2B |= (1 << CS22);
      break;
    case(1024):
      TCCR2B |= (1 << CS22);
      TCCR2B |= (1 << CS20);
      break;
  }

  _enableTimer2Interrupts();
}

void _disableTimer2Interrupts(){
  // enable timer compare interrupt
  TIMSK2 &= ~(1 << OCIE2A);
}

void _enableTimer2Interrupts(){
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}
//*****************ISR********************
ISR(TIMER2_COMPA_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //static unsigned long lastT = millis();
  //unsigned long thisT = millis();
  if (isRunning){
    if (stepper.runSpeed()){ //If we actually stepped
      curPos++;
      if (curPos >= lengthInSteps){
        isRunning = false;
      }
    }
  }
  //Serial.println(thisT - lastT);
  //lastT = thisT;
}


