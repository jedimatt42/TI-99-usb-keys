#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

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

// This data is used to maintain the state of the TI keyboard switches.
// when a column is scanned by the TI, then the rows are set to match
// this state. 
// The state is updated by the key down and key up routines handling
// the USB keyboard input.
int c0rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c1rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c2rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c3rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c4rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c5rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int c6rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// map symbols for the address of column/row pairs ( a key on the TI matrix )
// column 0
int* tk_Equal = c0rows;
int* tk_Space = c0rows+1;
int* tk_Enter = c0rows+2;
int* tk_Fctn = c0rows+4;
int* tk_Shift = c0rows+5;
int* tk_Ctrl = c0rows+6;
// column 1
int* tk_Period = c1rows;
int* tk_L = c1rows+1;
int* tk_O = c1rows+2;
int* tk_9 = c1rows+3;
int* tk_2 = c1rows+4;
int* tk_S = c1rows+5;
int* tk_W = c1rows+6;
int* tk_X = c1rows+7;
// column 2 (, K I 8 3 D E C)
int* tk_Comma = c2rows;
int* tk_K = c2rows+1;
int* tk_I = c2rows+2;
int* tk_8 = c2rows+3;
int* tk_3 = c2rows+4;
int* tk_D = c2rows+5;
int* tk_E = c2rows+6;
int* tk_C = c2rows+7;
// column 3 (M J U 7 4 F R V)
int* tk_M = c3rows;
int* tk_J = c3rows+1;
int* tk_U = c3rows+2;
int* tk_7 = c3rows+3;
int* tk_4 = c3rows+4;
int* tk_F = c3rows+5;
int* tk_R = c3rows+6;
int* tk_V = c3rows+7;
// column 4 (N H Y 6 5 G T B)
int* tk_N = c4rows;
int* tk_H = c4rows+1;
int* tk_Y = c4rows+2;
int* tk_6 = c4rows+3;
int* tk_5 = c4rows+4;
int* tk_G = c4rows+5;
int* tk_T = c4rows+6;
int* tk_B = c4rows+7;
// column 5 (/ ; P 0 1 A Q Z)
int* tk_Slash = c5rows;
int* tk_Semicolon = c5rows+1;
int* tk_P = c5rows+2;
int* tk_0 = c5rows+3;
int* tk_1 = c5rows+4;
int* tk_A = c5rows+5;
int* tk_Q = c5rows+6;
int* tk_Z = c5rows+7;
// column 6 (alpha_lock)
int* tk_Alpha = c6rows+4;



//-------------------------------------
// Initialize the TI interfacing pins.

void initPinModes() 
{
  initInputs();
  initOutputs();
}

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
   // Init outputs to 'floating undriven' so as to not interfere with TI keyboard.
  int outputMode = INPUT;
  pinMode(ti_r0, outputMode);
  pinMode(ti_r1, outputMode);
  pinMode(ti_r2, outputMode);
  pinMode(ti_r3, outputMode);
  pinMode(ti_r4, outputMode);
  pinMode(ti_r5, outputMode);
  pinMode(ti_r6, outputMode);
  pinMode(ti_r7, outputMode);
}

//------------------------------------------------------
// TI Keyboard Column Scan Handling
//   The following functions are part of the interrupt
//   handlers triggered when the TI scans a keyboard
//   column.

void setOutputPin(int pin, int state) 
{
  if (state) {
    // The TI input pins are connected to 10K Ohm pull-up resisters. So, to activate we
    // want to drive them low to ground. 
    pinMode(pin, OUTPUT_OPENDRAIN);
    digitalWrite(pin, LOW);
  } else {
    // When a key is not pressed, we don't want to drive it anywhere, so that the 
    // built-in keyboard can still work. 
    pinMode(pin, INPUT);
  }
}

// Given a columns set of switches, set each output pin accordingly.
void setRowOutputs(int* rows)
{
  int i = 0;
  setOutputPin(ti_r0, rows[i++]);
  setOutputPin(ti_r1, rows[i++]);
  setOutputPin(ti_r2, rows[i++]);
  setOutputPin(ti_r3, rows[i++]);
  setOutputPin(ti_r4, rows[i++]);
  setOutputPin(ti_r5, rows[i++]);
  setOutputPin(ti_r6, rows[i++]);
  setOutputPin(ti_r7, rows[i++]);
}

void onTiC0()
{
  setRowOutputs(c0rows);
}

void onTiC1()
{
  setRowOutputs(c1rows);
}

void onTiC2()
{
  setRowOutputs(c2rows);
}

void onTiC3()
{
  setRowOutputs(c3rows);
}

void onTiC4()
{
  setRowOutputs(c4rows);
}

void onTiC5()
{
  setRowOutputs(c5rows);
}

void onTiC6()
{
  setRowOutputs(c6rows);
}

void setColumnInterrupts()
{
  int interruptMode = FALLING;
  attachInterrupt(ti_c0, onTiC0, interruptMode);
  attachInterrupt(ti_c1, onTiC1, interruptMode);
  attachInterrupt(ti_c2, onTiC2, interruptMode);
  attachInterrupt(ti_c3, onTiC3, interruptMode);
  attachInterrupt(ti_c4, onTiC4, interruptMode);
  attachInterrupt(ti_c5, onTiC5, interruptMode);
  attachInterrupt(ti_c6, onTiC6, interruptMode);
}

//-----------------------------------------------
// USB Keyboard input handling - mostly taken
//   from the USB Host Boot Keyboard example.

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void toggleKey(uint8_t mod, uint8_t key, int state);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< mod:");
  Serial.println(m);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  switch(mod) {
    case 64:
      *tk_Fctn = 1;
      break;
  }
  toggleKey(mod, key, 1);
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
  switch(mod) {
    case 64:
      *tk_Fctn = 0;
    break;
  }
  toggleKey(mod, key, 0);
}

void KbdRptParser::toggleKey(uint8_t mod, uint8_t key, int state)
{
    switch(key) {
    // ---------- First row of TI keyboard
    case 0x1E:
      *tk_1 = state;
      break;
    case 0x1F:
      *tk_2 = state;
      break;
    case 0x20:
      *tk_3 = state;
      break;
    case 0x21:
      *tk_4 = state;
      break;
    case 0x22:
      *tk_5 = state;
      break;
    case 0x23:
      *tk_6 = state;
      break;
    case 0x24:
      *tk_7 = state;
      break;
    case 0x25:
      *tk_8 = state;
      break;
    case 0x26:
      *tk_9 = state;
      break;
    case 0x27:
      *tk_0 = state; // zero
      break;
    case 0x2E:
      *tk_Equal = state;
      break;
    // ------ Second Row
    case 0x14:
      *tk_Q = state;
      break;
    case 0x1A:
      *tk_W = state;
      break;
    case 0x08:
      *tk_E = state;
      break;
    case 0x15:
      *tk_R = state;
      break;
    case 0x17:
      *tk_T = state;
      break;
    case 0x1C:
      *tk_Y = state;
      break;
    case 0x18:
      *tk_U = state;
      break;
    case 0x0C:
      *tk_I = state;
      break;
    case 0x12:
      *tk_O = state; // oh
      break;
    case 0x38:
      *tk_Slash = state; // '/'
      break;
    // ---------- Third Row
    case 0x04:
      *tk_A = state;
      break;
    case 0x16:
      *tk_S = state;
      break;
    case 0x07:
      *tk_D = state;
      break;
    case 0x09:
      *tk_F = state;
      break;
    case 0x0A:
      *tk_G = state;
      break;
    case 0x0B:
      *tk_H = state;
      break;
    case 0x0D:
      *tk_J = state;
      break;
    case 0x0E:
      *tk_K = state;
      break;
    case 0x0F:
      *tk_L = state;
      break;
    case 0x33:
      *tk_Semicolon = state;
      break;
    case 0x28:
    case 0x58:
      *tk_Enter = state;
      break;
    // --------- Fourth Row 
    case 0x1D:
      *tk_Z = state;
      break;
    case 0x1B:
      *tk_X = state;
      break;
    case 0x06:
      *tk_C = state;
      break;
    case 0x19:
      *tk_V = state;
      break;
    case 0x05:
      *tk_B = state;
      break;
    case 0x11:
      *tk_N = state;
      break;
    case 0x10:
      *tk_M = state;
      break;
    case 0x36:
      *tk_Comma = state;
      break;
    case 0x37:
      *tk_Period = state;
      break;
    // ----------- Fifth Row
    case 0x2C:
      *tk_Space = state;
      break;
  }
}

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

void setup()
{
  initPinModes();
  setColumnInterrupts();
  
  Serial.begin( 115200 );

  // THIS BLOCKS IF NOT CONNECTED TO PC.
  while(!Serial); 
  
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  else
    Serial.println("USB.Init succeeded.");

  delay( 200 );

  next_time = millis() + 5000;

  HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
  // Read USB input which updates the state of the in-memory keyboard matrix.
  Usb.Task();
}

