#ifndef _TI_PINS_H
#define _TI_PINS_H

// teensy 3.1 pin aliases for TI keyboard connector. 
//   and info describing the meaning. 
//   _c? is a column input pin from the TI
//   _r? is a row output pin for the TI
int ti_c3 = 6; // key_cable_pin_15  (M J U 7 4 F R V)
int ti_c2 = 5; // key_cable_pin_14  (, K I 8 3 D E C)
int ti_c1 = 4; // key_cable_pin_13  (. L O 9 2 S W X)
int ti_c0 = 3; // key_cable_pin_12  (= space enter fctn shift ctrl)
int ti_r7 = 15; // key_cable_pin_11 (X C V B Z)
int ti_r6 = 16; // key_cable_pin_10 (ctrl W E R T Q)
int ti_c4 = 2; // key_cable_pin_9   (N H Y 6 5 G T B)
int ti_c5 = 1; // key_cable_pin_8   (/ ; P 0 1 A Q Z)
int ti_r4 = 17; // key_cable_pin_7  (fctn 2 3 4 5 1 alpha_lock)
int ti_c6 = 0; // key_cable_pin_6   (alpha_lock)
int ti_r0 = 18; // key_cable_pin_5  (= . , M N /)
int ti_r1 = 19; // key_cable_pin_4  (space L K J H ;)
int ti_r5 = 20; // key_cable_pin_3  (shift S D F G A)
int ti_r3 = 21; // key_cable_pin_2  (9 8 7 6 0)
int ti_r2 = 22; // key_cable_pin_1  (enter O I U Y P)

//-------------------------------------
// Initialize the TI interfacing pins.


void initInputs() 
{
  int inputMode = INPUT_PULLUP;
  pinMode(ti_c0, inputMode);
  pinMode(ti_c1, inputMode);
  pinMode(ti_c2, inputMode);
  pinMode(ti_c3, inputMode);
  pinMode(ti_c4, inputMode);
  pinMode(ti_c5, inputMode);
  pinMode(ti_c6, inputMode);
}

void initOutputs()
{
  // this mode seems to work well with letting the TI builtin keyboard still function.
  int outputMode = OUTPUT_OPENDRAIN; 
  pinMode(ti_r0, outputMode);
  pinMode(ti_r1, outputMode);
  pinMode(ti_r2, outputMode);
  pinMode(ti_r3, outputMode);
  pinMode(ti_r4, outputMode);
  pinMode(ti_r5, outputMode);
  pinMode(ti_r6, outputMode);
  pinMode(ti_r7, outputMode);
  digitalWrite(ti_r0,HIGH);
  digitalWrite(ti_r1,HIGH);
  digitalWrite(ti_r2,HIGH);
  digitalWrite(ti_r3,HIGH);
  digitalWrite(ti_r4,HIGH);
  digitalWrite(ti_r5,HIGH);
  digitalWrite(ti_r6,HIGH);
  digitalWrite(ti_r7,HIGH);
}

void initPinModes() 
{
  initInputs();
  initOutputs();
}

#endif

