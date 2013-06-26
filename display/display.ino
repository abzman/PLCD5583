int addrPins[4] = {2,3,4,5};
int dataPins[8] = {6,7,8,9,10,11,12,13};
int resetPin = A0; //active low
int flashPin = A1; //active low
int writePin = A2; //active low
int enablePin = A3; //active low
int brightness = 7;
boolean flashing = 0;
boolean clears = 0;
boolean blinking = 0;
boolean lampTest = 0;

void setup() {
  for (int i=0; i<4; ++i) pinMode(addrPins[i], OUTPUT);
  for (int i=0; i<8; ++i) pinMode(dataPins[i], OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(flashPin, OUTPUT);
  pinMode(writePin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(resetPin,HIGH);
  digitalWrite(flashPin, HIGH);
  digitalWrite(enablePin,HIGH);
  digitalWrite(writePin,HIGH);
  
  rst();
  
  digitalWrite(enablePin,LOW);

   char cmd[] = {'A','b','z','m','a','n','2','K'};
   for (int i=0; i<8; ++i) {
     flash(i,(i%2)?true:false);
     writeLED(cmd[i],i);
   }
   brightness = 4;
   flashing = 1;
   blinking = 0;
  setCommand();
}

void rst() {
  digitalWrite(resetPin,LOW);
  delay(10);  
  digitalWrite(resetPin,HIGH);
}

boolean isBitSet(byte data, int i) {
  return ((data & (1 << i)) >> i);
}

void write(byte data) {
  for (int i = 0; i < 8; ++i)
    digitalWrite(dataPins[i], isBitSet(data,i));
  latch();
}

void latch()
{
  //Latch out our write
  digitalWrite(writePin,LOW);
  delay(10);  
  digitalWrite(writePin,HIGH);
}

void writeLED(char c, int digit) {
  
  //Address Decode
  digitalWrite(addrPins[0], (digit % 2) ? HIGH : LOW);
  digitalWrite(addrPins[1], ((digit / 2) % 2) ? HIGH : LOW);
  digitalWrite(addrPins[2], (digit > 3) ? HIGH : LOW);
  digitalWrite(addrPins[3], HIGH);
  
  //Write our character
  write(c);
}

void flash(int digit, bool flash) {
  
  //Flash
  digitalWrite(flashPin, LOW);
  
  //Address Decode
  digitalWrite(addrPins[0], (digit % 2) ? HIGH : LOW);
  digitalWrite(addrPins[1], ((digit / 2) % 2) ? HIGH : LOW);
  digitalWrite(addrPins[2], (digit > 3) ? HIGH : LOW);
  digitalWrite(addrPins[3], HIGH);
  
  //Write our character
  write(flash ? 0x01 : 0x00);
 
  
  digitalWrite(flashPin, HIGH);
}

void setCommand()
{
  digitalWrite(addrPins[3], LOW);
  if (brightness < 8)
  {
     digitalWrite(dataPins[7], clears);
     digitalWrite(dataPins[6], lampTest);
     digitalWrite(dataPins[4], blinking);
     digitalWrite(dataPins[3], flashing);
     digitalWrite(dataPins[2], (brightness > 3) ? LOW : HIGH);
     digitalWrite(dataPins[1], ((brightness / 2) % 2) ? LOW : HIGH);
     digitalWrite(dataPins[0], (brightness % 2) ? LOW : HIGH);
     latch();
  }
  digitalWrite(addrPins[3], HIGH);
}

void loop() {
  
}
