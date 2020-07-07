String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int VoltageValue;

#define RELE_1  7
#define RELE_2  6
#define RELE_3  5
#define RELE_4  4


void set_rele(char status, int rele)
{
  if (status == '0')  digitalWrite(rele, LOW);
  if (status == '1')  digitalWrite(rele, HIGH);
  Serial.print("OK ");
  Serial.print(rele);
  Serial.print("=");
  Serial.println(status);
}




void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial.println("CAREL GME TEST IO BOARD V.1.1");
  
  // reserve 100 bytes for the inputString:
  inputString.reserve(100);


  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  pinMode(RELE_3, OUTPUT);
  pinMode(RELE_4, OUTPUT);


}

void loop() {
   //Voltage is continously polled  
  VoltageValue = analogRead(A0);
  delay(1);     
  if (stringComplete) {
    switch (inputString[0])
    { 
      case 'A' :   
        set_rele(inputString[1], RELE_1);
        break;
        
      case 'B' :
        set_rele(inputString[1], RELE_2);
        break;
        
      case 'C' :
        set_rele(inputString[1], RELE_3);
        break;
        
      case 'D' :
        set_rele(inputString[1], RELE_4);
        break;
                   
      case 'V' :  
        //voltage @pin A0 the conversion at PC side 
        Serial.print("V=");
        Serial.println(VoltageValue);
        break;
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
