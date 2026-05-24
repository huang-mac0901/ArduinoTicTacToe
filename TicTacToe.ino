//this code will light up each led in the 4x4 matrix one at a time

//pin connections- the #define tag will replace all instances of "latchPin" in your code with A1 (and so on)
int latchPin = A1;
int clockPin = A0;
int dataPin = A2;


//extra variables for tic tac toe
int turn = 0;
char board[3][3];


//looping variables
byte i;
byte j;


//storage variable
byte dataToSend;


//storage for led states, 4 bytes
//always leave 1st element at zero
byte ledData[] = {0,0,0,0};
String user;
int val;


void setup() {
  //set pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
  Serial.println();
}


void loop() {
  if (turn == 0) {
    Serial.print("Player 1 Turn: [1-9]");
  }
  else {
    Serial.print("Player 2 Turn: [1-9]");
  }
  while (Serial.available() == 0) {
    for (i=0;i<4;i++){
   
      //send data from ledData to each row, one at a time
      byte dataToSend = (1 << (i+4)) | (15 & ~ledData[i]);
      // setlatch pin low so the LEDs don't change while sending in bits
      digitalWrite(latchPin, LOW);
   
      //shift out the bits of dataToSend to the 74HC595
      //shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
      //the code below is the equivalent of the two lines above
      for (j=0;j<8;j++){
        digitalWrite(clockPin,LOW);
        digitalWrite(dataPin,((dataToSend>>j)&1));
        digitalWrite(clockPin,HIGH);
      }


      //set latch pin high- this sends data to outputs so the LEDs will light up
      digitalWrite(latchPin, HIGH);  
    }
  }
  val = Serial.parseInt();
  Serial.println(val);
  int row = (val-1)/3 + 1;
  int col = (val-1)%3;
  int temp = 1 << col;
  ledData[row] += temp;
  // if (pow(2,col) == 4.0) {
  //   ledData[row] = ledData[row] + pow(2,col);
  //   int temp = ledData[row] + 1;
  //   Serial.println(temp);
  //   ledData[row] = temp;
  // }
  // else {
  //   ledData[row] = ledData[row] + pow(2,col);
  //   Serial.print("test");
  // }
  Serial.println(ledData[row]);
  if (turn == 0) {
    board[row-1][col] = 'X';
  }
  else {
    board[row-1][col] = 'O';
  }
}
