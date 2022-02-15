#define sensor1 A0  // thumb 
#define sensor2 A3  // ring

#define thumb 6
#define ring 5
#define power 8

#define THUMB_THRESH 300 //550
#define RING_THRESH 400 //550

int sensorValue = 0;       
int sensorValue2 = 0;

bool flag = false;
bool prev_flag = false;

bool flag2 = false;
bool prev_flag2 = false;

bool flag3 = false;
bool prev_flag3 = false;

bool ACTIVE = false;
bool LED_STATE = false;


void setup() {
  Serial.begin(9600); //115200);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(power, INPUT);
  
  pinMode(thumb, OUTPUT);
  pinMode(ring, OUTPUT);
}

void loop() 
{
  float sensorValue = analogRead(sensor1);    // A0 thumb
  float sensorValue2 = analogRead(sensor2);   // A1 ring

  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(sensorValue2); 
    
  prev_flag = flag;
  prev_flag2 = flag2;

  if (digitalRead(power)== HIGH){
    ACTIVE = HIGH;
  }
  else{
    ACTIVE = LOW;
  }
  
  if ((sensorValue >= THUMB_THRESH) && ACTIVE) // This value works well to identify the thumb when placed on forearm
  {
    flag = true;
  }
  else
  {
    flag = false;
  }

  if((sensorValue2 >= RING_THRESH) && ACTIVE) // Ring finger
  {
    flag2 = true;
  }
  else
  {
    flag2 = false;
  }
  
  if ((prev_flag == true) && (flag == false))
  {
    Serial.println("====== NEXT SLIDE ======");
    digitalWrite(thumb, 1);
    delay(800);
    digitalWrite(thumb, 0);
  }
  if ((prev_flag2 == true) && (flag2 == false))
  {
    Serial.println("====== PREVIOUS SLIDE ======");
    digitalWrite(ring, 1);
    delay(800);
    digitalWrite(ring, 0);
  }
}

void PLOT_SIGNALS() {
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(sensorValue2); 
}
