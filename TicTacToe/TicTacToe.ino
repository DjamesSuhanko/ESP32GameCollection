#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int fromTop = 60;
int fromLeft = 10;
int space = 5;
int boxSize = 35;
int playGround = 3 * boxSize + 2 * space;

int i  = 0;
int j  = 0;
int me = random(1, 10);
bool player              = 0;
bool choiced             = false;
bool myTime              = false;
bool defensed            = false;
int dd[3][3]             = {0};
char sign[2]             = {'x', 'o'};
uint8_t player_X         = 1;
uint8_t player_O         = 4;
int     matrix_values[9] = {0};
long timeout             = millis();
int last_pos             = 0;
enum places {A,D,G,B,E,H,C,F,I};
enum shifter {sA,sB,sC,sD,sE,sF,sG,sH,sI};
bool virtual_shift = false;
bool virtual_enter = false;

void checkWiner();
void ESPlay();

void ESPlay() {
    uint8_t k = 0;
    for (uint8_t ii=0;ii<3;ii++){
        for (uint8_t jj=0;jj<3;jj++){
            matrix_values[k] = dd[ii][jj];
            k++;
        }
    }
    //DEFESA
    //defesa linha 1
    if ((matrix_values[A] + matrix_values[B] + matrix_values[C]) == 2){
        if (matrix_values[A] == 0 && last_pos != sA ||matrix_values[B] == 0 && last_pos != sB || matrix_values[C] == 0 && last_pos != sC){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa linha 2
    else if ((matrix_values[D] + matrix_values[E] + matrix_values[F]) == 2){
        if (matrix_values[D] == 0 && last_pos != sD ||matrix_values[E] == 0 && last_pos != sE || matrix_values[F] == 0 && last_pos != sF){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa linha 3
    else if ((matrix_values[G] + matrix_values[H] + matrix_values[I]) == 2){
        if (matrix_values[G] == 0 && last_pos != sG ||matrix_values[H] == 0 && last_pos != sH || matrix_values[I] == 0 && last_pos != sI){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa coluna 1
    else if ((matrix_values[A] + matrix_values[D] + matrix_values[G]) == 2){
        if (matrix_values[A] == 0 && last_pos != sA ||matrix_values[D] == 0 && last_pos != sD || matrix_values[G] == 0 && last_pos != sG){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa coluna 2
    else if ((matrix_values[B] + matrix_values[E] + matrix_values[H]) == 2){
        if (matrix_values[B] == 0 && last_pos != sB ||matrix_values[E] == 0 && last_pos != sE || matrix_values[H] == 0 && last_pos != sH){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa coluna 3
    else if ((matrix_values[C] + matrix_values[F] + matrix_values[I]) == 2){
        if (matrix_values[C] == 0 && last_pos != sC ||matrix_values[F] == 0 && last_pos != sF || matrix_values[I] == 0 && last_pos != sI){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa diagonal A-I
    else if ((matrix_values[A] + matrix_values[E] + matrix_values[I]) == 2){
        if (matrix_values[A] == 0 && last_pos != sA ||matrix_values[E] == 0 && last_pos != sE || matrix_values[I] == 0 && last_pos != sI){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    //defesa diagonal C-G
    else if ((matrix_values[C] + matrix_values[E] + matrix_values[G]) == 2){
        if (matrix_values[C] == 0 && last_pos != sC ||matrix_values[E] == 0 && last_pos != sE || matrix_values[G] == 0 && last_pos != sG){
            virtual_shift = true;
        }
        else{
            virtual_enter = true;
            defensed      = true;
        }   
    }
    else {     
        if (myTime && !defensed){
            if (!choiced){
                int me = random(1, 10);
                 while (matrix_values[me] != 0){
                    //int picked = random(1, 10);
                    //me = picked > 8 ? 8 : picked;
                     me = random(1, 10);
                }
                 choiced = true;
            }
            
            Serial.print("my choice: ");
            Serial.println(me);
            
            if (me != last_pos){
                virtual_shift = true;
            }
            else{
                virtual_enter = true;
                myTime        = false;
                choiced       = false;
            }
            
            
        }
        else {
            defensed = false;
            myTime   = false;
        }
        //agora definir o shift
            
        
    }
    
}

void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("TicTacToe", 5, 12, 4);
  pinMode(0, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);

  tft.drawFastVLine(fromLeft + boxSize + space / 2, fromTop, playGround,TFT_WHITE);
  tft.drawFastVLine(1 + fromLeft + boxSize + space / 2, fromTop, playGround,TFT_WHITE);
  tft.drawFastVLine(fromLeft + boxSize * 2 + space + space / 2, fromTop,playGround, TFT_WHITE);
  tft.drawFastVLine(1 + fromLeft + boxSize * 2 + space + space / 2, fromTop,playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, fromTop + boxSize + space / 2, playGround,TFT_WHITE);
  tft.drawFastHLine(fromLeft, 1 + fromTop + boxSize + space / 2, playGround,TFT_WHITE);
  tft.drawFastHLine(fromLeft, fromTop + boxSize * 2 + space + space / 2,playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, 1 + fromTop + boxSize * 2 + space + space / 2,playGround, TFT_WHITE);

  tft.drawRect((fromLeft + (i * boxSize) + (i * space)),(fromTop + (j * boxSize) + (j * space)), boxSize, boxSize,TFT_RED);
  // Serial.println(random(1,9));
}

void loop() {
  if (millis() - timeout > 1000) {
    timeout = millis();
    //Serial.println(random(1, 10));
  }
  if (digitalRead(0) == 0 || virtual_shift) {  // shift
    last_pos = last_pos > 7 ? 0 : last_pos+1;
    Serial.println(last_pos);
    tft.drawRect((fromLeft + (i * boxSize) + (i * space)),(fromTop + (j * boxSize) + (j * space)), boxSize, boxSize,TFT_BLACK);
    i = i + 1;
    if (i > 2) {
      i = 0;
      j++;
    }
    if (j > 2) j = 0;
    tft.drawRect((fromLeft + (i * boxSize) + (i * space)),(fromTop + (j * boxSize) + (j * space)), boxSize, boxSize,TFT_RED);
    
    virtual_shift = false;
    delay(300);
  }

  if (digitalRead(35) == 0 || virtual_enter) {  // select
    
    if (dd[i][j] != 0) {
      Serial.println("busy");
      tft.setTextColor(TFT_RED);
      char who = player > 0 ? sign[0] : sign[1];
      tft.drawString(String(who),10 + fromLeft + (i * boxSize) + (space * i),6 + fromTop + (j * boxSize) + j * space, 4);
      delay(500);
      tft.setTextColor(TFT_WHITE);
      tft.drawString(String(who),10 + fromLeft + (i * boxSize) + (space * i),6 + fromTop + (j * boxSize) + j * space, 4);
    } 
    else {
      tft.drawString(String(sign[player]),10 + fromLeft + (i * boxSize) + (space * i),6 + fromTop + (j * boxSize) + j * space, 4);
      if (player == 0) dd[i][j] = 1;

      if (player == 1) dd[i][j] = 4;

      player = !player;
      checkWiner();
    }
    if (!virtual_enter){
        choiced = false;  
        myTime  = true;  
        Serial.println("esp joga");
    }
    virtual_enter = false;
    while (digitalRead(35) == 0);
  }
  ESPlay();
  choiced = false;
}

void checkWiner() {
  for (int m = 0; m < 3; m++) {
    int score = 0;
    int hscore = 0;
    for (int n = 0; n < 3; n++) {
      score = score + dd[m][n];
      hscore = hscore + dd[n][m];

      if (score == 3 || score == 12) {
        tft.drawFastVLine(fromLeft + (m * space) + (boxSize * m) + boxSize / 2,
                          fromTop, playGround, TFT_RED);
        tft.drawFastVLine(
            fromLeft + 1 + (m * space) + (boxSize * m) + boxSize / 2, fromTop,
            playGround, TFT_RED);
      }

      if (hscore == 3 || hscore == 12) {
        tft.drawFastHLine(fromLeft,
                          fromTop + (m * space) + (boxSize * m) + boxSize / 2,
                          playGround, TFT_RED);
        tft.drawFastHLine(
            fromLeft, fromTop + 1 + (m * space) + (boxSize * m) + boxSize / 2,
            playGround, TFT_RED);
      }
    }

    if (dd[0][0] + dd[1][1] + dd[2][2] == 3 ||
        dd[0][0] + dd[2][2] + dd[1][1] == 12) {
      tft.drawLine(fromLeft, fromTop, fromLeft + playGround,
                   fromTop + playGround, TFT_RED);
    }

    if (dd[0][2] + dd[1][1] + dd[2][0] == 3 ||
        dd[0][2] + dd[2][0] + dd[1][1] == 12) {
      tft.drawLine(fromLeft + playGround, fromTop, fromLeft,
                   fromTop + playGround, TFT_RED);
    }
  }
}
