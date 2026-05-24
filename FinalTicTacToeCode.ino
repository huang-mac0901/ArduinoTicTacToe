//Mackenzie Huang and Kyle Lind
//Arduino Project - Tic Tac Toe

//this code will light up each led in the 3x3 grid based on user input

//wiring
//Q0 -> #1, long leg
//Q1 -> #4, long leg
//Q2 -> #7, long leg
//Q3 -> #9, short leg
//Q4 -> #8, short leg
//Q5 -> #7, short leg

//Grid Numbers
// 1  2  3
// 4  5  6
// 7  8  9

//pin connections
int latchPinRed = A1;
int clockPinRed = A0;
int dataPinRed = A2;
int latchPinGreen = A4;
int clockPinGreen = A3;
int dataPinGreen = A5;

//pin connections for buttons;
int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;
int button5 = 6;
int button6 = 7;
int button7 = 8;
int button8 = 9;
int button9 = 10;

//pin connections for leds to let players know whose turn it is
int redLED = 11;
int greenLED = 12;

//looping variables
byte i;
byte j;
int dt = 1000;


//storage variable
byte dataToSendRed;
byte dataToSendGreen;

//storage for led states, 3 bytes for 3 rows
byte ledDataRed[] = {0,0,0};
byte ledDataGreen[] = {0,0,0};

//user input variables
int val = 0;

//variables to track player turn, players' moves on the board, and winner
int turn = 0;
String board[3][3] = {{"_", "_", "_"}, {"_", "_", "_"}, {"_", "_", "_"}};
String playGame = "Y";
int numTurns = 0;

//button variables
int button1Read;
int button2Read;
int button3Read;
int button4Read;
int button5Read;
int button6Read;
int button7Read;
int button8Read;
int button9Read;


void setup() {
  //set pins as output
  pinMode(latchPinRed, OUTPUT);
  pinMode(clockPinRed, OUTPUT);
  pinMode(dataPinRed, OUTPUT);
  pinMode(latchPinGreen, OUTPUT);
  pinMode(clockPinGreen, OUTPUT);
  pinMode(dataPinGreen, OUTPUT);
  //set pins as input
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);
  pinMode(button7, INPUT_PULLUP);
  pinMode(button8, INPUT_PULLUP);
  pinMode(button9, INPUT_PULLUP);
  //set pins for individual LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  Serial.begin(9600);
  Serial.println();
}


void loop() {
  if (turn == 0) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  else {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  while (val == 0) {
    for (i=0;i<3;i++){
      //send data from ledData to each row, one at a time
      dataToSendRed = (~ledDataRed[i] << 3) | ((1 << i) & 7);
      dataToSendGreen = (~ledDataGreen[i] << 3) | ((1 << i) & 7);

      //setlatch pin low so the LEDs don't change while sending in bits
      digitalWrite(latchPinRed, LOW);
      digitalWrite(latchPinGreen, LOW);

      //shift out the bits of dataToSend to the 74HC595
      shiftOut(dataPinRed, clockPinRed, MSBFIRST, dataToSendRed);
      shiftOut(dataPinGreen, clockPinGreen, MSBFIRST, dataToSendGreen);

      //set latch pin high- this sends data to outputs so the LEDs will light up
      digitalWrite(latchPinRed, HIGH);  
      digitalWrite(latchPinGreen, HIGH);  
    }
    button1Read = digitalRead(button1);
    button2Read = digitalRead(button2);
    button3Read = digitalRead(button3);
    button4Read = digitalRead(button4);
    button5Read = digitalRead(button5);
    button6Read = digitalRead(button6);
    button7Read = digitalRead(button7);
    button8Read = digitalRead(button8);
    button9Read = digitalRead(button9);
    if (button1Read == 0) {
      val = 1;
    }
    else if (button2Read == 0) {
      val = 2;      
    }
    else if (button3Read == 0) {
      val = 3;
    }
    else if (button4Read == 0) {
      val = 4;
    }
    else if (button5Read == 0) {
      val = 5;
    }
    else if (button6Read == 0) {
      val = 6;
    }
    else if (button7Read == 0) {
      val = 7;
    }
    else if (button8Read == 0) {
      val = 8;
    }
    else if (button9Read == 0) {
      val = 9;
    }
  }
  //changes led state based on user input
  int row = (val-1)/3;
  int col = (val-1)%3;
  val = 0;
  if (board[row][col] == "_") {
    byte addVal = 1 << col;
    if (turn == 0) {
      ledDataRed[row] |= addVal;
      board[row][col] = "X";
    }
    else {
      ledDataGreen[row] |= addVal;
      board[row][col] = "O";
    }
    numTurns++;
    turn ^= 1;
  }
  if (checkWinner()) {
    //Green Wins!
    if (turn == 0) {
      for (int x = 0; x < 3; x++) {
        ledDataRed[x] = 0;
      }
    }
    //Red Wins!
    else {
      for (int x = 0; x < 3; x++) {
        ledDataGreen[x] = 0;
      }
    }
    for (int i = 0; i < dt; i++) {
      for (int x=0;x<3;x++){
        //send data from ledData to each row, one at a time
        dataToSendRed = (~ledDataRed[x] << 3) | ((1 << x) & 7);
        dataToSendGreen = (~ledDataGreen[x] << 3) | ((1 << x) & 7);

        //setlatch pin low so the LEDs don't change while sending in bits
        digitalWrite(latchPinRed, LOW);
        digitalWrite(latchPinGreen, LOW);

        //shift out the bits of dataToSend to the 74HC595
        shiftOut(dataPinRed, clockPinRed, MSBFIRST, dataToSendRed);
        shiftOut(dataPinGreen, clockPinGreen, MSBFIRST, dataToSendGreen);

        //set latch pin high- this sends data to outputs so the LEDs will light up
        digitalWrite(latchPinRed, HIGH);  
        digitalWrite(latchPinGreen, HIGH);  
      }
      delay(1);
    }
    roundEnds();
  }
  //Tie
  else if (numTurns == 9) {
    for (int i = 0; i < dt; i++) {
      for (int x=0;x<3;x++){
        //send data from ledData to each row, one at a time
        dataToSendRed = (~ledDataRed[x] << 3) | ((1 << x) & 7);
        dataToSendGreen = (~ledDataGreen[x] << 3) | ((1 << x) & 7);

        //setlatch pin low so the LEDs don't change while sending in bits
        digitalWrite(latchPinRed, LOW);
        digitalWrite(latchPinGreen, LOW);

        //shift out the bits of dataToSend to the 74HC595
        shiftOut(dataPinRed, clockPinRed, MSBFIRST, dataToSendRed);
        shiftOut(dataPinGreen, clockPinGreen, MSBFIRST, dataToSendGreen);

        //set latch pin high- this sends data to outputs so the LEDs will light up
        digitalWrite(latchPinRed, HIGH);  
        digitalWrite(latchPinGreen, HIGH);  
      }
      delay(1);
    }
    roundEnds();
  }
}


boolean checkWinner() {
  boolean isWinner = false;
  for (int r = 0; r<3; r++)
  {
    if (board[r][0] == "_" || board[r][1] == "_" || board[r][2] == "_") {
      continue;
    }
    if (board[r][0] == board[r][1] && board[r][0] == board[r][2])
    {
      Serial.println("row");
      return true;
    }
  }
  for (int c = 0; c<3; c++)
  {
    if (board[0][c] == "_" || board[1][c] == "_" || board[2][c] == "_") {
      continue;
    }
    if (board[0][c] == board[1][c] && board[0][c] == board[2][c])
    {
      Serial.println("col");
      return true;
    }
  }
  if ((board[0][0] != "_" && board[1][1] != "_" && board[2][2] != "_") && board[0][0] == board[1][1] && board[0][0] == board[2][2])
  {
    Serial.println("diag 1");
    return true;
  }
  if ((board[0][2] != "_" && board[1][1] != "_" && board[2][0] != "_") && board[0][2] == board[1][1] && board[0][2] == board[2][0])
  {
    Serial.println("diag 2");
    return true;
  }
  return false;
}


void roundEnds() {

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      board[r][c] = "_";
    }
  }
  for (int r = 0; r < 3; r++) {
    ledDataRed[r] = 0;
    ledDataGreen[r] = 0;
  }
  numTurns = 0;
}





