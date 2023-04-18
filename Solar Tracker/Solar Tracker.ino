#include <Servo.h>  //include the software library required to control the servo motors
#define THRESHOLD 100
#define DTIME 50
#define SENSOR_TL 2  //Top Left sensor pin
#define SENSOR_TR 0  //Top Right sensor pin
#define SENSOR_BL 3  //Bottom Left sensor pin
#define SENSOR_BR 1  //Bottom Right sensor pin

#define HORIZ_LIMIT 160 //Horizontal limit should be the full rotation of a servo
#define VERT_LIMIT 160   //The value is determined depending on the mechanical setup (the angle at which the vertical servo keeps the panel parallel to ground)

Servo horiz;        //Servo for horizontal movement
Servo vert;         //Servo for vertical movement
int horizpos;
int vertpos;

void setup() {
  Serial.begin(9600); 
  horiz.attach(3);
  vert.attach(4);

  delay(1000);
}
void loop() {

   track();
   
}
void track(){

  int tl = analogRead(SENSOR_TL); //Read the value of the TL sensor
  int tr = analogRead(SENSOR_TR); //Read the value of the TR sensor
  int bl = analogRead(SENSOR_BL); //Read the value of the BL sensor
  int br = analogRead(SENSOR_BR); //Read the value of the BR sensor
  int average_top = (tl + tr) / 2;
  int average_bottom = (bl + br) / 2;
  int average_left = (tl + bl) / 2;
  int average_right = (tr + br) / 2;


  int dif_vert = average_top - average_bottom;
  int dif_horz = average_left - average_right;

  if ( ((-1 * THRESHOLD) <= dif_vert) && (dif_vert <= THRESHOLD) ) {
    vert.detach();
  }
  else {
    vert.attach(4); 

    if (average_top > average_bottom)
    {
      vertpos = --vertpos;
      if (vertpos > VERT_LIMIT)
      {
        vert.detach();
        vertpos = VERT_LIMIT;
      }
    }
    else if (average_top < average_bottom)
    {
      vertpos = ++vertpos;
      if (vertpos < 0)
      {
        vert.detach();
        vertpos = 0;
      }
    }
    else if (average_top == average_bottom)
    {
      // nothing
    }
    vert.write(vertpos);
  }
  if ( ((-1 * THRESHOLD) <= dif_horz) && (dif_horz <= THRESHOLD) ) {
    horiz.detach();
  }
  else {
    horiz.attach(3); 

    if (average_left > average_right)
    {
      horizpos = --horizpos;
      if (horizpos < 0)
      {
        horiz.detach();
        horizpos = 0;
      }
    }
    else if (average_left < average_right)
    {
      horizpos = ++horizpos;
      if (horizpos > HORIZ_LIMIT)
      {
        horiz.detach();
        horizpos = HORIZ_LIMIT;
      }
    }
    else if (average_left == average_right)
    {
      // nothing
    }
      horiz.write(horizpos);
  }

  delay(DTIME);
}
