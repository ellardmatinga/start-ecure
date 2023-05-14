#include <TinyGPS++.h>
#include <Keypad.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd( 4, 5, 6, A3, A4, A5);
int led = A3;
int j1 = 0;
int i = 0, j = 0, temp = 0;
const int bzr = A1;
TinyGPSPlus gps;//This is the GPS object that will pretty
int cm;
char str[25];

char* password1 = "1234"; //password FOR BIKE START
int pass1 = 0; //keypad position

char* password2 = "4567"; //create a password FOR TO CHANE FINGER PRINT
int pass2 = 0; //keypad position


char* password3 = "1111"; //create a password TO UNLOCK  USING FINGERPRINT
int pass3 = 0; //keypad position


char* password4 = "2"; //create a password
int pass4 = 0; //keypad position

int count = 0;
int n = 0; int w = 0;
const byte rows = 4; //number of the keypad's rows and columns
const byte cols = 3;

char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}

};

byte rowPins[rows] = {7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[cols] =  {11, 12, 13}; //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);
int getFingerprintIDez();

#define relay A2 //define the LED pins
uint8_t id;
int r = 0;

uint8_t getFingerprintEnroll();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int buttonPin1 = A1;
const int buttonPin2 = A0;
const int buttonPin3 = A0;
const int motor1 = A0;
const int motor2 = 11;
// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

//===========================================================================================
//===========================================================================================
void setup()
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("Fingure print based");
  lcd.setCursor(0, 1);
  lcd.print("Vehicle starter       ");
  pinMode(relay, OUTPUT);  //set the LED as an output
  digitalWrite(relay, LOW);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(bzr, OUTPUT);
  digitalWrite(bzr, LOW);
  lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GSM initialisation");
  lcd.setCursor(0, 1);
  lcd.print("wait for 2 second");
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  Serial.println("AT+CMGF=1");
  delay(1000);
  lcd.clear();
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Did't findout   ");
    lcd.setCursor(0, 1);
    lcd.print("Fingureprint sensor   ");
    Serial.println("Did not find fingerprint sensor :(");
    while (1);

  }
}


void senddata()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for GPS");
  lcd.setCursor(0, 1);
  lcd.print("char come ");
  while (Serial.available()) //While there are characters to come from the GPS
  {
    gps.encode(Serial.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAT:");
    lcd.setCursor(4, 0);
    lcd.print(gps.location.lat(), 6);
    lcd.setCursor(0, 1);
    lcd.print("LOG:");
    lcd.setCursor(4, 1);
    lcd.print(gps.location.lng(), 6);

    Serial.print("AT\r");
    delay(200);
    Serial.print("AT+CMGF=1\r");
    delay(200);
    Serial.print("AT+CNMI=2,2,0,0,0\r");
    delay(200);
    Serial.print("AT+CMGS=");
    delay(200);
    Serial.print('"');
    delay(200);
    Serial.print("+447405264237");
    delay(200);
    Serial.print('"');
    delay(200);
    Serial.write(0x0D);
    delay(200);
    Serial.print("VEHICLE NO:01 ");
    Serial.print("https://maps.google.com/maps?q=loc:");
    //Serial.print("*Lattitude=");
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    // Serial.print("Longitude=");
    Serial.print(gps.location.lng(), 6);
    //Serial.print(",");
    delay(500);
    Serial.write(0x1A);
    delay(200);

    j = 1;
    // delay(5000);

  }
}

void loop ()                     // run over and over again
{
  id = 1;
  char whichKey = myKeypad.getKey(); //define which key is pressed with getKey
  if (whichKey == '0' || whichKey == '1' || whichKey == '2' || whichKey == '3' || whichKey == '4' || whichKey == '5' || whichKey == '6' || whichKey == '7' || whichKey == '8' || whichKey == '9' ||
      whichKey == '*' || whichKey == '#' )
  {
    w++;

  }
  lcd.setCursor(0, 0);
  lcd.print("PRES 1 FOR FINGER");
  lcd.setCursor(0, 1);
  lcd.print("OR ENTER PASSCODE");

  if (whichKey == '*' || whichKey == '#' || whichKey == 'A' ||      //define invalid keys
      whichKey == 'B' || whichKey == 'C' || whichKey == 'D')
  {

    pass1 = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Invalid Key!");
    delay(1000);
    lcd.clear();
  }
  if (whichKey == password1 [pass1])
  {


    pass1 ++;
  }
  else if (whichKey == password2 [pass2])
  {

    pass2 ++;
  }
  else if (whichKey == password3 [pass3])
  {

    pass3 ++;
  }
  whichKey = '^';

  if (pass1 == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** Verified ***");
    delay(3000); w = 0;
    while (1)
    {
      digitalWrite(relay, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" ***VEHICLE***");
      lcd.setCursor(0, 1);
      lcd.print("***STARTED***");
      delay(7000);
    }
  }

  else  if (pass2 == 4)
  {
    lcd.clear();
    w = 0;
    while (1)
    {

      lcd.setCursor(0, 0);
      lcd.print("**FINGER CHANGE**");
      lcd.setCursor(0, 1);
      lcd.print("*** MODE ***");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New fingureprint ");
      lcd.setCursor(0, 1);
      lcd.print("entering mode  ");

      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Waiting for new ");
      lcd.setCursor(0, 1);
      lcd.print("finger to enroll  ");
      while (!  getFingerprintEnroll() );
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New fingureprint ");
      lcd.setCursor(0, 1);
      lcd.print("entered succesfully");
      delay(2000);
      lcd.clear();
      //lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Kindly OFF and         ");
      lcd.setCursor(0, 1);
      lcd.print("ON the main kit      ");
      while (1);
      //  while (digitalRead(buttonPin3)==0);


    }
  }

  else if (pass3 == 3)
  {
    while (r == 0)
    {
      getFingerprintIDez();
      delay(50);            //don't ned to run this at full speed.
    }
    while (1)
    {
      digitalWrite(relay, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" ***VEHICLE***");
      lcd.setCursor(0, 1);
      lcd.print("***STARTED***");
      delay(7000);
    }
  }
  else if (w == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** WRONG ***");
    delay(3000);

    lcd.clear();
    count++;
    w = 0;

  }
  else if (count >= 3)
  {
    digitalWrite(bzr, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** send sms***");
    delay(3000);
    lcd.clear();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reading location ");
    lcd.setCursor(0, 1);
    lcd.print("Sending Location    ");
    delay(2000);
    digitalWrite(bzr, LOW);
    while (j == 0)
    {
      // senddataac();
      senddata();
    }
    j = 0;

    for (j1 = 0; j1 < 10; j1++)
    {
      str[j1] = 's';
    }

    count = 0;
  }

}
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  { //Serial.println(" Kindly put the fingure");
    return -1;
  }
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  if (finger.fingerID == 1)
  { lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rider is  ");
    lcd.setCursor(0, 0);
    lcd.print("   authorised");
    r = 1;
    delay(1000);
  }
  return finger.fingerID;
}

//========================================================================================
//========================================================================================

uint8_t getFingerprintEnroll() {

  int p = -1;


  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Communctn error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Imaging error");
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place same    ");
  lcd.setCursor(0, 1);
  lcd.print("finger again  ");


  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Communctn error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Imaging error");
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stored!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Kindly OFF and         ");
    lcd.setCursor(0, 1);
    lcd.print("ON the main kit      ");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

//=====================================================================================
//=====================================================================================
