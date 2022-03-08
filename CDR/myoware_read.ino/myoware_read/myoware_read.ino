// myoware_read.ino

// This program sends analog values A0, A1, A2, A3 from Arduino via serial line delimited with ", "

#define sensor1 A5  
#define sensor2 A4  
#define sensor3 A2  
#define sensor4 A3  

#define LED 8

#define threshold 300

int val1, val2, val3, val4;


void setup() {
  Serial.begin(9600); //115200);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
}

void loop() 
{
  val1 = analogRead(sensor1);    
  val2 = analogRead(sensor2);
  //val3 = analogRead(sensor3);
  //val4 = analogRead(sensor4);   

  Serial.print(val1);
  Serial.print(", ");
  Serial.println(val2);
  /*Serial.print(", ");
  Serial.print(val3);
  Serial.print(", ");
  Serial.println(val4);*/
}
