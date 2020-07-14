void nextState(){

  switch(SEQ){
    case 0:
      invertStates();
      printStates();
      break;
      
    case 1:
      chase(0);
      printStates();
      break;
      
    case 2:
      invertStates();
      printStates();
      break;

    case 3:
      break;
  }
}

void changeSEQ(){
  SEQ++;
  if(SEQ >= 4) SEQ = 0;

  switch(SEQ){
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
      fill(0);
      break;
      
    default:
      break;
  }
  nextState;
}
