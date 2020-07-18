void nextState(){
    //static NEXT_SEQ_TIME = 0;
    int seq = SEQ_MAP[SEQ];

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
        AUTO_LIGHTS_FLAG = true;
        SEQ = random(0,6);
        nextState();
        break;

    default:
        break;

  }
}

void changeSEQ(){
//  SEQ++;
//  if(SEQ >= 4) SEQ = 0;
    int seq = SEQ_MAP[SEQ];

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
        fill(0);
        break;

    case 8:
        break;

    default:
      break;
  }
  nextState;
}
