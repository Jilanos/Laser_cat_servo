#include <Servo.h>
Servo myServo_1;
Servo myServo_2;
int const potPin = A0;

int angle_1, angle_2;
float x1, x2;
int target_1, target_2, delta_1, delta_2;
int dur_slow, dur_fast, dt_fast, dt_slow, dt, mode, count, n_consecutif, n_fast;
int influence;
//je veux déclarer le tableau


void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  myServo_1.attach(9);
  myServo_2.attach(10);
  Serial.begin(9600);
  delta_1 = 30;
  delta_2 = 15;
  target_1 = 148;
  target_2 = 125;
  angle_1 = target_1;
  angle_2 = target_2;
  count = 0;
  dur_slow = 3000;
  dur_fast = 1000;
  dt_slow = 130;
  dt_fast = 30;
  dt = dt_slow;
  mode = 1;
  n_consecutif = 4;
  influence = 75;
}

void loop() {
  if ((count * dt_fast > dur_fast && mode == -1) || (count * dt_slow > dur_slow && mode == 1)){
    count = 0;
    mode = mode *-1;
    if (mode==1){
      dt = dt_slow;
    }
    else{
      dt = dt_fast;
      n_fast = 0;
    }

  }
  if (mode==-1 && n_fast < n_consecutif){
      dt = dt_slow;
      n_fast +=1;
    }
  else{
    x1 = proba(float(ecart_no_sat(angle_1-target_1,delta_1,7 ))/7, influence);
    x2 = proba(float(ecart_no_sat(angle_2-target_2,delta_2,4 ))/4, influence);
 // x1 = proba((angle_1-target_1)/delta_1, influence);
 // x2 = proba((angle_2-target_2)/delta_2, influence);
    n_fast = 0;


    }
  count = count + 1;

  angle_1 = angle_1 + x1;
  angle_2 = angle_2 + x2;
  if (angle_1 > target_1+delta_1){
    angle_1 = target_1+delta_1;
  }
  if (angle_2 > target_2+delta_2){
    angle_2 = target_2+delta_2;
  }

  if (angle_1 < target_1-delta_1){
    angle_1 = target_1-delta_1;
  }
  if (angle_2 < target_2-delta_2){
    angle_2 = target_2-delta_2;
  }

  
  Serial.print("A1: ");
  Serial.print(angle_1);
  Serial.print(", x1: ");
  Serial.print(float (angle_1-target_1)/delta_1);
  Serial.print("°,  A2: ");
  Serial.print(angle_2);
  Serial.print(", x2: ");
  Serial.print(float (angle_2-target_2)/delta_2);
  Serial.print("°, MODE: ");
  Serial.print(mode);
  Serial.print(", n_fast: ");
  Serial.println(n_fast);
  myServo_1.write(angle_1);
  myServo_2.write(angle_2);
  delay(dt);
}


int proba(float ecart, int influence)
{
  int proba, val;
  int addition = influence * ecart;
  proba = random(-100-addition, 101-addition);
  if (proba > 0){
    val = 1;
  }
  else {
    val = -1;
  }
  return(val);
}


int ecart_no_sat(int ecart,int delta, int seuil){
  int val = 0;
  if (ecart > 0){
    if ((delta - ecart)<seuil){
      val = seuil-(delta- ecart);
  }}
  if (ecart < 0){
    if ((delta + ecart)<seuil){
      val = -(seuil-(delta+ ecart));
  }}
  return(val);
}