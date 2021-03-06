// Steps through the states of the SEQ
void nextState(){
    // Uses the SEQ_MAP to lookup the SEQ location on the joystick mapping
    int seq = SEQ_MAP[SEQ];

  // TODO name the sequences
  switch(seq){
    case 0:
      invertStates();
      printStates();
      break;
      
    case 1: 
      chase(PRIME_MODE);
      printStates();
      break;
      
    case 2:
      invertStates();
      printStates();
      break;

    case 3:
      chase(PRIME_MODE);
      printStates();
      break;

    case 4:
      chase(PRIME_MODE);
      printStates();
        break;
      
    case 5:
      chase(PRIME_MODE);
      printStates();
        break;

    case 6:
        fill(PRIME_MODE);
        printStates();
        break;

    case 7:
        //AUTO_LIGHTS_FLAG = true;
        //SEQ = random(1,7);
        //nextState();
        break;

    default:
        break;

  }
}

// Does the setup for when we change the SEQ type
void changeSEQ(){
    int seq = SEQ_MAP[SEQ];

  // TODO name the sequences
  switch(seq){
    case 0:
      alternate(0);
      break;
      
    case 1:
      fill(0);
      STATES[0] = 1;
      break;
    
    case 2:
      fill(1);
      break;

    case 3:
      fill(1);
      STATES[0] = 0;
      break;

    case 4:
        fill(0);
        STATES[0] = 1;
        STATES[1] = 1;
        break;
      
    case 5:
        fill(1);
        STATES[0] = 0;
        STATES[1] = 0;
        break;

    case 6:
        fill(PRIME_MODE);
        break;

    case 7:
       AUTO_LIGHTS_FLAG = true;
       SEQ = random(1,7);
       PRIME_MODE = random(0,2);
       changeSEQ();
       return;

    case 8:
        break;

    default:
      break;
  }
  nextState;
}
