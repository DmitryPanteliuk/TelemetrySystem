#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); 
int recieved_data[2];
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"}; 

int sens[2]; 

int left1 = A1;
int right1 = A2;

long timeleft;
long timeright;

long resl;
long resr;

const int SENSOR = 150; 

void setup(){
  Serial.begin(9600);
 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(7,OUTPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 Serial.begin(9600);


  
  radio.begin(); 
  radio.setAutoAck(1);         
  radio.setRetries(0,15);     
  radio.enableAckPayload();    
  radio.setPayloadSize(32);    

  radio.openReadingPipe(1,address[0]);      
  radio.setChannel(0x60);  

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  
  
  radio.powerUp(); 
  radio.startListening();  
}

void loop() {

char start = Serial.read();

if(start == 's'){
      
     Serial.println("\t ON THE START");
     
     int randomne = 1500; //задержка светофора
     int val = 0;
     
     digitalWrite(2, HIGH);   // Жовтий 1 ряд

     for(;randomne > val;){
      analogRead(left1);
      analogRead(right1);
      val++;
      if(analogRead(left1)<SENSOR){
        sens[1] = 1;
        }
  
    if(analogRead(right1)<SENSOR){
        sens[2] = 1;
      }
   
     }
     val = 0;


     
     digitalWrite(3, HIGH);  // Жовтий 2 ряд
           
      for(;randomne > val;){
      analogRead(left1);
      analogRead(right1);
      val++;
      if(analogRead(left1)<SENSOR){
        sens[1] = 1;
        }
   
    if(analogRead(right1)<SENSOR){
        sens[2] = 1;
      }
   
     }
     val = 0;

     
     digitalWrite(4, HIGH);  // Жовтий 3 ряд
   
     for(;randomne > val;){
      analogRead(left1);
      analogRead(right1);
      val++;
      if(analogRead(left1)<SENSOR){
        sens[1] = 1;
        }
   
    if(analogRead(right1)<SENSOR){
        sens[2] = 1;
      }
    
     }
     val = 0;

     
     digitalWrite(5, HIGH);  // Жовтий 4 ряд
           
      for(;randomne > val;){
      analogRead(left1);
      analogRead(right1);
      val++;
      if(analogRead(left1)<SENSOR){
        sens[1] = 1;
        }
   
    if(analogRead(right1)<SENSOR){
        sens[2] = 1;
      }
    
     }
     val = 0;



Serial.println("GO!   GO!  GO!");
Serial.println(" ");
Serial.print("PARAMETER ");
Serial.print("\t LEFT ");
Serial.println("\t RIGHT");
Serial.println(" ");
Serial.print("START");


     
      if(sens[1] == 1){
     digitalWrite(11,HIGH); // Красний Лівий вкл
     digitalWrite(6,LOW); // Зелений Лівий викл
     Serial.print("\t");
     Serial.print("\t FALSE ");
     
        
      }
      else{
        digitalWrite(6, HIGH);
        Serial.print("\t");
        Serial.print("\t OK");
      }
      
     if(sens[2] == 1){
     digitalWrite(12,HIGH); // Красний Правий вкл
     digitalWrite(7,LOW); // Зелений Лівий викл
     Serial.print("\t");
     Serial.println(" FALSE");
        
      }
      else{
        digitalWrite(7, HIGH);
        Serial.println(" OK");
      }
     
  
     
    
     
     unsigned long last_time = micros(); 
     
     for(;3000 > val;){
      analogRead(left1);
      analogRead(right1);
      val++;
      if(analogRead(left1)<SENSOR){
       timeleft = (micros() - last_time)/10000;
        }
   
    if(analogRead(right1)<SENSOR){
        timeright = (micros() - last_time)/10000;
      }
    
     }
     val = 0;
Serial.println(" ");
Serial.print("Delay Start");
Serial.print("\t"); Serial.print(timeleft);
Serial.print("\t"); Serial.println(timeright);
Serial.println(" ");
     
     digitalWrite(2,LOW); // Жовтий 1 ряд викл
     digitalWrite(3,LOW); // Жовтий 2 ряд викл
     digitalWrite(4,LOW); // Жовтий 3 ряд викл
     digitalWrite(5,LOW); // Жовтий 4 ряд викл
     digitalWrite(6,LOW); // Зелений Лівий викл
     digitalWrite(7,LOW); // Зелений Правий викл
     digitalWrite(11, LOW);   // Красний лівий виключити
     digitalWrite(12, LOW);   // Красний правий виключити
   


   while(recieved_data[0] == 0 || recieved_data[1] == 0 ){
     byte pipeNo;                       
     while(radio.available(&pipeNo)){
     radio.read( &recieved_data, sizeof(recieved_data) );    
     }
   }
 


Serial.print("TIME");
if(recieved_data[0]== 1){
    
    Serial.print("\t"); Serial.print("\t");
    resl = ((micros()-last_time)/10000)- recieved_data[1];
    Serial.print(resl);
    Serial.print("\t");  
    resr = ((micros()-last_time)/10000)+ recieved_data[0];
    Serial.print(resr); 
    

   }
   if(recieved_data[1]== 1){
    
    Serial.print("\t"); Serial.print("\t"); 
    resl = ((micros()-last_time)/10000)+ recieved_data[1];
    Serial.print(resl);
    Serial.print("\t");  
    resr = ((micros()-last_time)/10000)- recieved_data[0];
    Serial.print(resr); 
    

   }

   
Serial.println(" ");
Serial.println(" ");
Serial.print("RESULT");
if(resl < resr){
Serial.print("\t LEFT WIN");
Serial.println("\t RIGHT LOSE");
}
else{
Serial.print("\t LEFT LOSE");
Serial.println("\t RIGHT WIN");
}







sens[0] = 0 ;
sens[1] = 0;
    
    delay(5000);
    } 
}





   
   
