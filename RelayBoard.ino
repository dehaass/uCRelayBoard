/*
Arduino Relay Board Code

Code for driving an 8 output mechanical relay board with AC christmas lights.

Created: 2020/07/05
Stuart de Haas

Last Edit: 2020/07/07
*/

// Global variables for holding information about outputs
const int NUM_CH = 8; // number of chanels to use. 
int STATES[] = {0,1,0,1,1,1,1,1}; // holds the state of the relay outputs
int PTR = STATES[8]; // points at the array of states (TODO I don't think I need this anymore...)
const int OUTPUT_PINS[] = {9,8,7,6,5,4,3,A5}; // Maps physical pins to the STATES array
const int BUTTON_PIN = 2; // Pin used for the button (PIN 2 has an interupt thats why I'm using it)
const int POT_PIN = A4;
const int SWITCH_PIN = A3;
const int JOYSTICK_HORZ_PIN = A0;
const int JOYSTICK_VERT_PIN = A2;
const int JOYSTICK_BUTT_PIN = A1;


// Global info for sequences/patterns
bool MODE = 0; // 0 -> manual or 1 -> auto
int SEQ = 0; // The current sequence
bool PRIME_MODE = 0; // Sub mode
int SEQ_MAP[] = {7,1,2,3,4,5,6,0}; // Map SEQ to joystick positions
unsigned int BEATS[5] = {500,500,500,500,500}; // holds temporary values from button to calculate period
int INDEX = 0; // Used to index the BEATS array
int PERIOD = 500; // Sequence period. The speed we move through the pattern.
unsigned long BUTT_NEXT_STEP_TIME = 500; // When to make the next change. Compared to the system clock.
int PRESCALER = 1; // Can be used to slow the period
bool AUTO_LIGHTS_FLAG = false;

// Global variables related to the button
unsigned long NEXT_CHECK_TIME = 0; // Next time to check the button state
bool BUTTON_INTERUPT_FLAG = true; // Has an interupt fired?
int BUTTON_HOLD_TIME = 1500; // How long to hold the button to perform a different action
int DEBOUNCE_TIME = 10; // Button software debounce time
unsigned long LAST_PRESS_TIME = 0; // Last time the button was pressed. Based on system clock

void setup() {
  //Serial.begin(115200); // Serial is just used for debugging
  int i;
  for(i=0; i<NUM_CH; i++){
    pinMode(OUTPUT_PINS[i], OUTPUT);
  }
  pinMode(BUTTON_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(JOYSTICK_HORZ_PIN, INPUT);
  pinMode(JOYSTICK_VERT_PIN, INPUT);
  pinMode(JOYSTICK_BUTT_PIN, INPUT_PULLUP);
  // Attach an interrupt to the button pin. When a rising edge is detected, sends the program to buttonCatch_ISR
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonCatch_ISR, RISING);
  //fill(1);
}

// Outputs the STATES array to the physical OUTPUT_PINS
void printStates(){
  int i;
  for(i=0; i<NUM_CH; i++){
    digitalWrite(OUTPUT_PINS[i], STATES[i]);
  }

}

// Fill the STATES array with 'val' (0 or 1)
void fill(bool val){
  int i;
  for(i=0; i<NUM_CH; i++){
    STATES[i] = val;
  }
}

// Alternates output values (0,1,0,1, etc...)
// 'invert' set if it starts with 0 or 1 or toggles
void alternate(int invert){
  int i;
  if(invert == 2){
    invert = STATES[0] ? 0: 1;
  }
  for(i=0; i<NUM_CH; i++){
    STATES[i] = i%2;
    if(invert) STATES[i] = STATES[i] ? 0 : 1;
  }
}

// Shift the current 'STATES' one over
// dir = 0 shifts right
// dir = 1 shifts left
void chase(int dir){
  int i;

  if(dir == 0){
    int temp = STATES[NUM_CH - 1];
    for(i=NUM_CH-1; i>0; i--){
      STATES[i] = STATES[i-1];
    }
    STATES[0] = temp;
  }else{
    int temp = STATES[0];
    for(i=0; i<NUM_CH-1; i++){
      STATES[i] = STATES[i+1];
    }
    STATES[NUM_CH-1] = temp;
  }
  
}

void invertStates(){
  int i;
  for(i=0; i<NUM_CH; i++){
    STATES[i] = STATES[i] ? 0: 1;
  }
}

// Button interrupt function
void buttonCatch_ISR(){
  NEXT_CHECK_TIME = millis() + DEBOUNCE_TIME;
  BUTTON_INTERUPT_FLAG = true;
}

// All the smarts behind that nice button
void buttonLogic(){

  if(digitalRead(BUTTON_PIN) == HIGH && BUTTON_INTERUPT_FLAG == true){
    // button just pressed
    unsigned long recentPressTime = (NEXT_CHECK_TIME - DEBOUNCE_TIME);
    unsigned int duration = recentPressTime - LAST_PRESS_TIME;
    LAST_PRESS_TIME = recentPressTime;
    
    if(duration > 2000){ // If it's been too long between pulses, reset the array and discard
        INDEX = 0; 
    }else if(duration > 50){ // discard pulses that are too fast (mechanical relays ya know?)

      BEATS[INDEX] = duration;
      INDEX++;
      if(INDEX >= 5){ // if we have 5 good duration values
        int i;
        PERIOD = 0;
        for(i = 0; i<5; i++){
          PERIOD += BEATS[i];
        }
        PERIOD = PERIOD/5; // Set the system period to the average of the 5 readings
        INDEX = 0;
        BUTT_NEXT_STEP_TIME = millis() + PERIOD*PRESCALER - DEBOUNCE_TIME;
        //Serial.print("Tempo: ");
        //Serial.println(60.0/(float(PERIOD)/1000.0));
      }
    }
  }else if(digitalRead(BUTTON_PIN) == HIGH){
    // Button held high
    MODE = MODE ? 0: 1;
    //changeSEQ();
  }
  BUTTON_INTERUPT_FLAG = false;
  NEXT_CHECK_TIME = millis() + BUTTON_HOLD_TIME;
}

int readJoystick(){
  int joyHorz, joyVert;
  byte joyState = 0b00000000;
  
  joyHorz = analogRead(JOYSTICK_HORZ_PIN);
  joyVert = analogRead(JOYSTICK_VERT_PIN);
  if(joyHorz > 768){
    joyState = joyState | 0b00000001;
  }else if(joyHorz < 256){
    joyState = joyState | 0b00000010;
  }
  
  if(joyVert > 768){
    joyState = joyState | 0b00000100;
  }else if(joyVert < 256){
    joyState = joyState | 0b00001000;
  }

  switch(joyState){
    case 0:
      joyState = 13; // joystick in center position
      break;
    case 4:
      joyState = 0;
      break;
    case 5:
      joyState = 1;
      break;
    case 1:
      joyState = 2;
      break;
    case 9:
      joyState = 3;
      break;
    case 8:
      joyState = 4;
      break;
    case 10:
      joyState = 5;
      break;
    case 2:
      joyState = 6;
      break;
    case 6:
      joyState = 7;
      break;
    default:
      joyState = 0;
      break;
  }
  
  return joyState;
}

int joystickLogic(){
  if( digitalRead(JOYSTICK_BUTT_PIN) == LOW){
      PRIME_MODE = PRIME_MODE ? 0: 1;
      return 500;
  }

  int joyState = readJoystick();
  if(joyState == 13) return 50; // Joystick in center position
  static int prevJoyState = 0; 

  if(MODE == 0){ // in Auto Mode
      if(joyState != SEQ){
          AUTO_LIGHTS_FLAG = false;
          SEQ = joyState;
          changeSEQ();
      }
  }else{ // in Manual Mode
    if(PRIME_MODE){
        // toggle
        if(joyState != prevJoyState){
            STATES[joyState] = STATES[joyState] ? 0: 1;
            printStates();
            prevJoyState = joyState;
        }

    }else{
        // momentary
        if(digitalRead(SWITCH_PIN) == HIGH){
            fill(1);
            STATES[joyState] = 0;
        }else{
            fill(0);
            STATES[joyState] = 1;
        }
        printStates();
    }
    
  }
  return 50;
}

void loop() {
  static unsigned long POT_NEXT_STEP_TIME = 0;
  static unsigned long NEXT_JOY_READ = 0;
  static unsigned long NEXT_SEQ_TIME = 0;

  if(millis() > NEXT_CHECK_TIME) buttonLogic();
  if(millis() > NEXT_JOY_READ) NEXT_JOY_READ = joystickLogic() + millis();

  if(millis() > BUTT_NEXT_STEP_TIME){
    BUTT_NEXT_STEP_TIME = millis() + PERIOD*PRESCALER;
    if(MODE == 0 && digitalRead(SWITCH_PIN) == LOW) nextState();
  }

  if(millis() > POT_NEXT_STEP_TIME){
    int potVal = analogRead(A4);
    POT_NEXT_STEP_TIME = millis() + (potVal * (1950.0 / 1023.0)) + 50;
    if(MODE == 0 && digitalRead(SWITCH_PIN) == HIGH) nextState();
  }

  if(millis() > NEXT_SEQ_TIME){
    int potVal = analogRead(A4);
    NEXT_SEQ_TIME = millis() + (potVal * ((60.0*1000.0) / 1023.0)) + 1000;
    if(AUTO_LIGHTS_FLAG == true && MODE == 0){
        SEQ = random(0,6);
        PRIME_MODE = random(0,2);
        changeSEQ();
    }
  }

}
