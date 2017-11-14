const char slaveId1 = 48;
const char slaveId2 = 49;
String strRead;
int potPin = A0;


void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  digitalWrite(13, LOW);
}

void loop()
{
  
  if (Serial.available()>0) {
      strRead = Serial.readStringUntil('\r\n');
     
    if(strRead[0]==58) { // check start bit
      if(strRead[1]==slaveId1 && strRead[2]==slaveId2) { // check address
        if (strRead[3] == 48 && strRead[4] == 49)  // we're in fc01
        {
          Serial.write(58);
          Serial.write(slaveId1);
          Serial.write(slaveId2);
          Serial.write(48);
          Serial.write(49);
          Serial.write(48); // byte count 01
          Serial.write(49);
          Serial.write(48);
          int calc = digitalRead(5) + digitalRead(6)*2 + digitalRead(7)*4;
          Serial.print(calc);
          
          
         // int lrc1 = calcCheckSum({58,slaveId1,slaveId2,48,49,48,49,48,calc,13,10},11);
         // Serial.print(lrc1);
          Serial.write(13); // CR
          Serial.write(10); // LF
        }
        else if (strRead[3] == 48 && strRead[4] == 53) // we're in fc05
        {
          Serial.write(58);
          Serial.write(slaveId1);
          Serial.write(slaveId2);
          Serial.write(48);
          Serial.write(53);
          Serial.write(48); // byte count 01
          Serial.write(49); // Sor
          Serial.write(48);
          Serial.write(7); 
          
        //  int lrc5 = calcCheckSum({58,slaveId1,slaveId2,48,53,48,49,48,7,13,10},11);
        //  Serial.write(lrc5);
         Serial.write(13); // CR
          Serial.write(10); // LF
        }   else if (strRead[3] == 48 && strRead[4] == 52) // we're in fc04
       
        {
          Serial.write(58);
          Serial.write(slaveId1);
          Serial.write(slaveId2);
          Serial.write(48);
          Serial.write(52);
          Serial.write(48); // byte count 02
          Serial.write(50);
          Serial.write(48);
          int deger = analogRead(potPin);
          String degerInHex = String(deger, HEX);
          char charBuf[10];
          degerInHex.toCharArray(charBuf, 10);
          sprintf(charBuf,"%02X", charBuf);
          
          Serial.print(degerInHex);

          //int lrc2 = calcCheckSum({58,48,49,48,50,48,49,48,buf,13,10},11);
         // Serial.print(lrc2);
           Serial.write(13); // CR
          Serial.write(10); // LF
        }
        delay(50);
      } else {
        Serial.end();
        delay(50);
        Serial.begin(9600);
      }
     }
   
  }


}

  int * calcCheckSum (int* pack, int packSize) {
   int sum = 0;
   for (int i=0; i < (packSize-1); i++) sum += pack[i];
   sum = sum & 0xFF;
   pack[packSize-1] = 0x100 - sum;
   return pack;
  }
