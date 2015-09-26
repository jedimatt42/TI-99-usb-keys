#ifndef _TI_SCAN_H
#define _TI_SCAN_H

//------------------------------------------------------
// TI Keyboard Column Scan Handling
//   The following functions are part of the interrupt
//   handlers triggered when the TI scans a keyboard
//   column.

void setOutputPin(int pin, int state) 
{
  digitalWrite(pin, state ? LOW : HIGH);
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


#endif

