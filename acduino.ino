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

char data[80];
int delimiter = 59; // means: ; 
String readString;


// PANEL 1
//Pin connected to ST_CP of 74HC595
int latchPin = 8; //RCK MARRON
//Pin connected to SH_CP of 74HC595
int clockPin = 10; //SCK ROJO
////Pin connected to DS of 74HC595
int dataPin = 9; // DIO NARANJA

// PANEL 2
//Pin connected to ST_CP of 74HC595
int latchPin2 = 4; //RCK MARRON
//Pin connected to SH_CP of 74HC595
int clockPin2 = 6; //SCK ROJO
////Pin connected to DS of 74HC595
int dataPin2 = 5; // DIO NARANJA

int cache1[8];
int cache2[8];

int gearLed = 7;
int gearLedStatus = 0;

int refresh = 1;

void setCache1()
{
  for (int i=0; i<8; i++)
  {
    cache1[i] = -1;

    if (i != 3 && i != 5)
    {
      if (i == 4) // Gear
        cache1[i] = 10;
      else
        cache1[i] = 0;
    }
  }
}

void setCache2()
{
  for (int i=0; i<8; i++)
  {
    cache2[i] = -1;

    if (i != 5)
      cache2[i] = 0;
  }
}

void setup()
{
  // SET CACHE TO DEFAULT VALUES
  setCache1();
  setCache2();

  // CONFIGURE DISPLAYS OUTPUTS PINS
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);

  // GEEAR LED CONFIGURATION
  pinMode(gearLed, OUTPUT);
  
  // OPEN SERIAL COMMUNICATION
  Serial.begin(9600);
}

void loop()
{
  showPanels();

  while (Serial.available() > 0)
  {
    Serial.readBytesUntil(delimiter, data, 80);
    String command = String(data);
    processCommand(command);
    memset(data, '\0', 80);
  }
}

void processCommand(String command)
{
  //Serial.println(command);
  char aux = command.charAt(0);
  int type = aux - '0';
  command = command.substring(2, command.length());
  
  if (type == 1 || type == 2)
    setData(type, command);
  else if (type == 3)
    setRPMLed(command);
}

void setRPMLed(String cadena)
{
  char aux = cadena.charAt(0);
  gearLedStatus = aux - '0';
}

void showRPMLed()
{
  if (gearLedStatus)
    digitalWrite(gearLed, HIGH);
  else 
    digitalWrite(gearLed, LOW);
}

void setData(int panel, String cadena)
{
  //Serial.print("Panel: "); Serial.print(panel); Serial.println("");
  //Serial.print("Texto: "); Serial.print(cadena); Serial.println("");
  
  int posicion = 7;
  int digitos[8];
  
  for (int i=7; i>=0; i--)
  {
    setCacheData(panel, cadena[i], posicion - i);
  }
}

// SET DATA
void setCacheData(int panel, int numero, int indice)
{
  switch (panel)
  {
    case 1:
      setData1(numero, indice);
      break;
    case 2:
      setData2(numero, indice);
      break;
    default:
      break;
  }
}

// SET LAPS, GEAR AND SPEED IN CACHE1
void setData1(char digit, int posicion)
{
  if (3 == posicion || 5 == posicion) return;
  
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
  
  cache1[posicion] = numbers[indice];
}

// SET DATA AND RPM IN CACHE2
void setData2(char digit, int posicion)
{
  if (posicion == 5) return;
  
  int indice = digit - '0';
  cache2[posicion] = numbers[indice];
}

void showPanels()
{
  showRPMLed();
  showPanel1();
  showPanel2();
}

void showPanel1()
{
  for (int i=0; i<8; i++)
  {
    if (cache1[i] != -1)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, positions[i]);
      shiftOut(dataPin, clockPin, MSBFIRST, cache1[i]);
      digitalWrite(latchPin, HIGH);
      delay(refresh); 
    }
  }
}

void showPanel2()
{
  for (int i=0; i<8; i++)
  {
    if (cache2[i] != -1)
    {
      digitalWrite(latchPin2, LOW);
      shiftOut(dataPin2, clockPin2, MSBFIRST, positions[i]);
      shiftOut(dataPin2, clockPin2, MSBFIRST, cache2[i]);
      digitalWrite(latchPin2, HIGH);
      delay(refresh); 
    }
  }
}
