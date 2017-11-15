
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
          int calc = digitalRead(5) + digitalRead(6)*2;
          Serial.print(calc);
          
          Serial.write(70); // LRC
         Serial.write(48); // 
         
          Serial.write(13); // CR
          Serial.write(10); // LF
        }
        else if (strRead[3] == 48 && strRead[4] == 53) // we're in fc05
        {
          
          Serial.write(58);
         
          Serial.write(slaveId1);
          Serial.write(slaveId2);
          if(strRead[11]==70 && strRead[12]==70) {
            digitalWrite(strRead[8]-40, HIGH); // ascii to pin number trick here
          } else if(strRead[11]==48 && strRead[12]==48) { 
            digitalWrite(strRead[8]-40, LOW);
          }
          
          Serial.write(48);
          Serial.write(53);
          Serial.write(48);
          Serial.write(48);
          Serial.write(48);
          Serial.write(strRead[8]);
          Serial.write(48);
          Serial.write(48);
          Serial.write(strRead[11]);
          Serial.write(strRead[12]);

          Serial.write(70); // LRC
         Serial.write(48); // 
  
         Serial.write(13); // CR
         Serial.write(10); // LF
        } else if (strRead[3] == 48 && strRead[4] == 52) // we're in fc04
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

         Serial.write(70); // LRC
         Serial.write(48); // 
         
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
