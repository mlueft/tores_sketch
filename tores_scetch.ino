#include <Wire.h>

int ledPin = 13;
char buf[100];

void setup() {

  pinMode( ledPin, OUTPUT);
  Serial.begin(9600);
  Wire.begin();

}

String command = "";
int executeCommand = 0;

void loop() {

  while(Serial.peek() != 0x00 && Serial.available() > 0){
      command += Serial.readStringUntil(0x00);

      executeCommand = 1;
       break;
  }
  /*
  while( Serial.available() > 0 ){
      command += Serial.readString();
      executeCommand = 1;
  }
/**/
  if( executeCommand > 0 ){

    if( getValue( command, ' ', 0) == "read" ){
      
      readBytes( getValue( command, ' ', 1).toInt(), getValue( command, ' ', 2).toInt() );
    
    }else if( getValue( command, ' ', 0) == "write" ){
      
      Serial.println("received command:" +command+ "\n" );  
      writeByte( getValue( command, ' ', 1).toInt(), (byte)getValue( command, ' ', 2).toInt(), (byte)getValue( command, ' ', 3).toInt() );
    
    }else if( getValue( command, ' ', 0) == "scan" ){

      Serial.println("received command:" +command+ "\n" );
      scanForDevices();
    
    }else if( getValue( command, ' ', 0) == "dump" ){

      Serial.println("received command:" +command+ "\n" );
      dumpDevice( getValue( command, ' ', 1).toInt(), getValue( command, ' ', 2).toInt() );
    
    }else
      Serial.println("unknown command:" +command+ "\n" );
    
    executeCommand = 0;
    command = "";
  }
  
}


void readBytes( int addr, int _size ){

  Wire.beginTransmission(addr);
  Wire.write(0);
  Wire.endTransmission();

  int memPos = 0;
  byte dump[_size] ;

  while( true ){
      
      if( memPos < _size){
  
        Wire.requestFrom( addr, 1);
        
        byte c = Wire.read();
        dump[memPos++] = c;
        
      }else
        break;
  }

  Serial.write( dump, _size );
  Serial.flush();
  
}

void writeByte( int addr, int memPos, byte data ){
  Wire.beginTransmission(addr);
  Wire.write( memPos );
  Wire.write( data );
  Wire.endTransmission();
}


void dumpDevice( int addr, int _size ){

  Wire.beginTransmission(addr);
  Wire.write(0);
  Wire.endTransmission();

  int line[16] ;
  int linePos = 0;
  int memPos = 0;
  
  while( true ){
    
    if( memPos < _size){

      Wire.requestFrom( addr, 1); 
    
      int c = Wire.read();
      line[linePos++] = c;

      if( linePos > 15 || Wire.available() ){

        sprintf(buf, "%8d: ", memPos);
        Serial.print( buf );

        for( int i = 0; i < linePos; i++){
          if(i==8)
            Serial.print( " " );
          sprintf(buf, "%2X ", line[i] );
          Serial.print(buf);
        }

        for( int i = 0; i < linePos; i++){
          if(i==8)
            Serial.print( " " );
          sprintf(buf, "%c", (char) line[i] );
          Serial.print(buf);
        }

        Serial.println();
        memPos += linePos;
        linePos = 0;
      }

    }else
      break;

  }
    
}

void scanForDevices(){

  Serial.println( "" );
  Serial.println( "Scanning for devices ..." );
  
  for( int i=0; i < 127; i++ ){

    Wire.requestFrom( i, 1);
    
    bool found = false;
    while( Wire.available() ){
     Wire.read();
     found = true;
    }

    if(found){
      sprintf(buf, "Device found at address:%3d", i);
      Serial.println( buf );
    }else{
      //sprintf(buf, "No device found ad address:%3d: ", i);
      //Serial.println( buf );
    }
    
  }
  
  Serial.println( "Scanning finished." );

}

void blink(int onPhase, int offPhase ){
  digitalWrite(ledPin, HIGH);
  delay(onPhase);
  digitalWrite(ledPin, LOW);
  delay(offPhase);
}

// http://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
  
