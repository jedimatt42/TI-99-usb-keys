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
void setRowOutputs(int* rows, int alock)
{
  setOutputPin(ti_r0, rows[0]);
  setOutputPin(ti_r1, rows[1]);
  setOutputPin(ti_r2, rows[2]);
  setOutputPin(ti_r3, rows[3]);
  setOutputPin(ti_r4, rows[4] + alock);
  setOutputPin(ti_r5, rows[5]);
  setOutputPin(ti_r6, rows[6]);
  setOutputPin(ti_r7, rows[7]);
}

void clearOutputs(int alock) {
  digitalWrite(ti_r0, HIGH);
  digitalWrite(ti_r1, HIGH);
  digitalWrite(ti_r2, HIGH);
  digitalWrite(ti_r3, HIGH);
  digitalWrite(ti_r4, alock ? LOW : HIGH);
  digitalWrite(ti_r5, HIGH);
  digitalWrite(ti_r6, HIGH);
  digitalWrite(ti_r7, HIGH);
}

void onTiColumnChange()
{
  // Stop interrupts so I don't get two with each column change.
  noInterrupts();

  // The column for the alpha-lock is not controlled by the
  // octal decoder that the rest of the keyboard is controlled by.
  int alock = 0;
  if (digitalRead(ti_c6) == LOW && *tk_Alpha) {
    alock = 1;
    digitalWrite(ti_r4, LOW);
  }

  // The outputs from the TI 9901 go to a octal decoder, so only 
  // one of these can be LOW at a time. 
  if (digitalRead(ti_c0) == LOW) {
    setRowOutputs(c0rows, alock);
  } else if (digitalRead(ti_c1) == LOW) {
    setRowOutputs(c1rows, alock);
  } else if (digitalRead(ti_c2) == LOW) {
    setRowOutputs(c2rows, alock);
  } else if (digitalRead(ti_c3) == LOW) {
    setRowOutputs(c3rows, alock);
  } else if (digitalRead(ti_c4) == LOW) {
    setRowOutputs(c4rows, alock);
  } else if (digitalRead(ti_c5) == LOW) {
    setRowOutputs(c5rows, alock);
  }

  interrupts();
}

void setColumnInterrupts()
{
  int interruptMode = CHANGE;
  attachInterrupt(ti_c0, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c1, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c2, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c3, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c4, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c5, onTiColumnChange, interruptMode);
  attachInterrupt(ti_c6, onTiColumnChange, interruptMode);
}

void forceFctnEquals(int state) {
  setOutputPin(ti_r0, state);
  setOutputPin(ti_r4, state);
}

#endif

