#include "LedControl.h"

LedControl lc = LedControl(11, 13, 10, 1);

const int boton_inicio = 2;
int joy_Pin1 = A0; 
int joy_Pin2 = A1; 
int axis_x = 0;
int axis_y = 0;

String direction = " up ";

int snake_X[36]; // there are 36 possible positions on the screen for the x axis
int snake_Y[36]; // there are 36 possible positions on the screen for the y axis

int Snakelength = 1;

int food_X;
int food_Y;

boolean end = false;

void setup() {
  Serial.begin(9600);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0); // clean the matrix

  pinMode(start_button, INPUT); // configuration as input 
  digitalWrite(start_button, HIGH);// we set it high

  food_X = random(0, 8);
  food_Y = random(0, 8); 
}

void loop() {
  lc.clearDisplay(0);

  if(!end){
    axis_x = analogRead(joy_Pin1);
    axis_y = analogRead(joy_Pin2);
    //Serial.println("Reading from joystick");
    //Serial.println(axis_x); // Print x-axis to serial port
    //Serial.println(axis_y); // Print y-axis to serial port
  
    // in which direction are we moving?
    if (axis_y >= 0 && axis_y <480) {
      if (direction != " below ") direction = "up"; 
    }
    if (axis_y > 520 && axis_y <=1023) {
      if (direction != "up") direction = "below";
    }
    if (axis_x >= 0 && axis_x <480) {
      if (direction != "right") direction = "left";
    }
    if (axis_x > 520 && axis_x <=1023) {
      if (direction != " left ") direction = " right ";
    }
  
    //Serial.println(direction);
  
    for (int i = Snakelength - 1; i > 0; i--) {
      snake_X[i] = snake_X[i - 1];
      snake_Y[i] = snake_Y[i - 1];
    }
   
    if (direction == "up") {
      if (snake_Y[0] == 0) {
        snake_Y[0] = 7;
      } else {
         snake_Y[0]--;
      }
    } else if (direction== " below") {
      if (snake_Y[0] == 7) {
        snake_Y[0] = 0;
      } else {
        snake_Y[0]++;
      }
    } else if (direction== "left") {
      if (snake_X[0] == 0) {
        snake_X[0] = 7;
      } else {
        snake_X[0]--;
      }
    } else if (direction== "right") {
      if (snake_X[0] == 7) {
        snake_X[0] = 0;
      } else {
        snake_X[0]++;
      }
    }
  
    // draw the snake
    for (int i = 0; i < snakelength; i++) {
      lc.setLed(0, snake_Y[i], snake_X[i], true); // (device number, row, column, value)
    }
  
    // draw the food
    if (snake_X[0] == food_X && snake_Y[0] == food_Y){
      // if the snake's head matches the food, it has eaten it and an LED grows
      snakelength ++;
      // new position
      food_X = random(0, 8); 
      food_Y = random(0, 8);
    }
    lc.setLed(0, food_Y, food_X, true);
    delay(50); // we'll make it blink
    lc.setLed(0, food_Y, food_X, false);
  
    // check if it collides with itself
    for (int i = 1; i < snakelength; i++) {
      if (snake_X[0] == snake_X[i] && snake_Y[0] == snake_Y[i]) {
        end = true;
      }
    }
  
    delay(300);
  } else {
    Serial.println("game over");
    if(digitalRead(start_button)==0){
      Serial.println("New");
      // initialize again
      // food
      food_X = random(0, 8);
      food_Y = random(0, 8);
      // initial position
     axis_x = 0;
      axis_y = 0;
      // length
      snakelength = 1; 
      // direction
      direction = "up";
      
      end = false;
    }
  }
}
