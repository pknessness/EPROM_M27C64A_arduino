int Q0 = A2;
int Q1 = A1;
int Q2 = A0;
int Q3 = A3;
int Q4 = A4;
int Q5 = A5;
int Q6 = A6;
int Q7 = A7;

int notP = 7;
int notG = 2;
int notE = 4;

int pState = 1;
int gState = 0;
int eState = 0;
int raw = 0;
int print = 1;

int Q[8] = {Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7};

void pinModeINPUT(){
  for(int i = 0; i < 8; i ++){
    pinMode(Q[i], INPUT);
  }
}

void pinModeOUTPUT(){
  for(int i = 0; i < 8; i ++){
    pinMode(Q[i], OUTPUT);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinModeINPUT();
  pinMode(notE, OUTPUT);
  pinMode(notG, OUTPUT);
  pinMode(notP, OUTPUT);
  Serial.begin(115200);
}

int threshold(int pin){
  return analogRead(pin) > 500 ? 1 : 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  char str[35];
  if(!raw)
    sprintf(str, "[%d%d%d%d%d%d%d%d] Program:%s Chip:%s Output:%s \n",threshold(Q7), threshold(Q6), threshold(Q5), threshold(Q4), threshold(Q3), threshold(Q2), threshold(Q1), threshold(Q0), pState ? "NO" : "YES", eState? "OFF" : "ON", gState? "OFF" : "ON");
  else{
    sprintf(str, "[%d-%d-%d-%d-%d-%d-%d-%d]\n",analogRead(Q7), analogRead(Q6), analogRead(Q5), analogRead(Q4), analogRead(Q3), analogRead(Q2), analogRead(Q1), analogRead(Q0));
  }
  if(print)
    Serial.print(str);
  digitalWrite(notE, eState);
  digitalWrite(notG, gState);
  digitalWrite(notP, pState);
  if(Serial.available()){
    char c = Serial.read();
    Serial.print(c);
    if(c == 'P' || c == 'p'){
      pState = !pState;
    }else if(c == 'G' || c == 'g'){
      gState = !gState;
    }else if(c == 'E' || c == 'e'){
      eState = !eState;
    }else if(c == 'R' || c == 'r'){
      raw = !raw;
    }else if(c == 'o' || c == 'O'){
      print = !print;
    }else if(c == 'I' || c == 'i'){
      int array[8] = {0,0,0,0,0,0,0,0};
      pinModeOUTPUT();
      digitalWrite(notG, HIGH);
      int index = 0;
      while(1){
        char h = '\0';
        if(Serial.available()){
          h = Serial.read();
          array[7-index] = (h == '1') ? 1 : 0;
          digitalWrite(Q[index], array[7-index]);
          Serial.print("[");
          Serial.print(7-index);
          Serial.print("-");
          Serial.print(array[7-index]);
          Serial.print("]");
          index ++;
        }
        if(index == 8 || h == '\n'){
          break;
        }
      }

      for(int i = 0; i < 8; i ++){
        Serial.print("{");
        Serial.print(i);
        Serial.print(".");
        Serial.print(Q[i]);
        Serial.print("-");
        Serial.print(digitalRead(Q[i]));
        Serial.print("}");
      }

      //digitalWrite(notP, LOW);
      digitalWrite(13, HIGH);

      delay(3);

      pinModeINPUT();
      //digitalWrite(notP, HIGH);
      digitalWrite(notG, LOW);
      digitalWrite(13, LOW);
    }
  }
}
