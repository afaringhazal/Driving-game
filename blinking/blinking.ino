#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 1
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10

#define SCROLL_DELAY 100


MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


byte out =1 ;
byte out1 =1 ;
byte out2 =1 ;
int state = 3;
#define left 4
#define right 5
int input= 0;
int score = 0;
bool death_=0;
int idle = 100;
 byte res[8];



int lable = 7;
byte matrix[8][8] = { {1, out2,out2, 0, 0, 0, 0, 1}, 
                      {1, 0, 0, 0, 0, 0, 0, 1},
                      {1, 0, 0, 0, 0, 0, 0, 1},
                      {1, 0, 0, out1, out1, 0, 0, 1},
                      {1, 0, 0, 0, 0, 0, 0, 1},
                      {1, 0, 0, 0, 0, out, out, 1},
                      {1, 0, 0, 0, 0, 0, 0, 1},
                      {1, 0, 0, 0, 0, 0, 0, 1},

};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // open the serial port at 9600 bps:
  mx.begin();
  pinMode(right, INPUT); //INPUT_PULLUP
  pinMode(left, INPUT); //
}
void show_matrix(){
//state car
byte last_state = matrix[lable][state];
matrix[lable][state]= 1;

for(int i =0; i<=7;i++){
  byte result = 0;
  for(int j=0;j<8;j++){
   byte shifted = matrix[i][j] << 7-j;
   result = result|shifted;
    }
    res[i] = result;
    }
  for(int i=0;i<7-lable;i++){
      for(int j=7; j>0;j--){
        res[j] = res[j-1];
      }
      res[0]=B10000001;      
      }

    for(int i=0;i<8;i++){
      byte b = res[i];
       mx.setRow(i, b);
    }
    matrix[lable][state]= last_state;
    delay(200);

}

void show_death_matrix(){
      for(int i=0;i<8;i++){
       mx.setRow(i, B11111111);
    }
    delay(200);
  }


void death(){
 Serial.print("called death!");
  death_ = 1;
  show_death_matrix();
  }

void validate_car(){
  if(state == 0 || state ==7){
    death();
    }
   if(matrix[lable][state] == 1){
    death();
    }
  }
void update_car(){
  int new_state = 0;
  if(input == right){
    new_state = state +1;
    if(new_state == 7){death();}
    state = new_state;
    }
    else if(input == left){
      new_state = state - 1;
      if(new_state ==0){death();}
      state = new_state;
      }
    else {
      new_state = state;
      }
}

void shift_lable(){
  if(lable != 0){
    lable = lable -1;
    }
 }



void update_total(){
  
 for(int i = 0; i<8; i++){
  if(death_ != 1){
  if(digitalRead(left)==LOW) {
    input = left;
    Serial.print("left");
    }
  else if(digitalRead(right)==LOW){
    input = right;
    Serial.print("right");
    }
  else{
    input= idle;
    }
  }
  if(death_!= 1){ update_car();}
  if(death_!= 1){ validate_car();}
  if(death_!= 1){ shift_lable();}
  if(death_!= 1){ show_matrix();}
  if(death_ !=1){delay(1000);}
 }
 
 if(death_!= 1){ score = score+1;}
 }

void loop() {
  
  death_ = 0;
  lable = 7;
  state = 3;
  out = random(0, 2); //0 or 1
  out1 = random(0, 2); //0 or 1
  out2 = random(0, 2); //0 or 1
  Serial.print("----");  // prints a label
  Serial.print(out);  
  Serial.print(out1); 
  Serial.print(out2); 


  for(int i =0;i<8;i++){
    for(int j=0;j<8;j++){
      matrix[i][j] = 0;
    }
  }
  for(int i=0;i<8;i++){
    matrix[i][0]=1;
    matrix[i][7]=1;
  }
  matrix[0][1]= out2; 
  matrix[0][2]=out2;
  matrix[3][3]=out1;
  matrix[3][4]=out1;
  matrix[5][5]=out;
  matrix[5][6]=out;

  show_matrix();
  update_total();
  Serial.println("end_updat total");
  delay(1000);
}
