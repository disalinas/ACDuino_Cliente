char data[80];
int delimiter = 59; // means: ; 
String readString;

//Encoder
int encoderA = LOW;
int encoderB = LOW;
int encoderPinALast = LOW;

//Pin connected to ST_CP of 74HC595
int latchPin = 8; //RCK MARRON
//Pin connected to SH_CP of 74HC595
int clockPin = 10; //SCK ROJO
////Pin connected to DS of 74HC595
int dataPin = 9; // DIO NARANJA

int point_numbers[10] = {
  B01000000
  ,B01111001
  ,B00100100
  ,B00110000
  ,B00011001
  ,B00010010
  ,B00000010
  ,B01111000
  ,B00000000
  ,B00010000
};

int numbers[12] = {
  B11000000   // 0
  ,B11111001  // 1
  ,B10100100  // 2
  ,B10110000  // 3
  ,B10011001  // 4
  ,B10010010  // 5
  ,B10000010  // 6
  ,B11111000  // 7
  ,B10000000  // 8
  ,B10010000  // 9
  ,B10111111  // -
  ,B10101111  // r
};

int positions[] = {
  128
  ,64
  ,32
  ,16
  ,8
  ,4
  ,2
  ,1
};

int refresh = 1;

void setup()
{
	Serial.begin(50000);
	//set pins to output because they are addressed in the main loop
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
}

void loop()
{
  while (Serial.available() > 0)
	{
    Serial.readBytesUntil(delimiter, data, 80);
    String command = String(data);
    setData(command);
    //memset(data, '\0', 80);
	} 
  
}

void setData(String cadena)
{
  int posicion = 7;
  int digitos[8];
  
  for (int i=7; i>=0; i--)
  {    
    if (i != 2 && i != 4)
    {
      showData(cadena[i], posicion - i);
    }
  }
}

void showData(char digit, int posicion)
{
  int indice = digit - '0';
  
  if (posicion == 4)
  {
    if (indice == 0)
      indice = 11;
    else if (indice == 1)
      indice = 10;
    else
      indice--;
  }
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, positions[posicion]);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[indice]);
  digitalWrite(latchPin,HIGH);
  delay(refresh);  
}

