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

void clearOutputs() {
  setOutputPin(ti_r0, 0);
  setOutputPin(ti_r1, 0);
  setOutputPin(ti_r2, 0);
  setOutputPin(ti_r3, 0);
  setOutputPin(ti_r4, 0);
  setOutputPin(ti_r5, 0);
  setOutputPin(ti_r6, 0);
  setOutputPin(ti_r7, 0);
}

int lastColumn = -1;

void handleTiScans() {
  if(digitalRead(ti_c0) == LOW && (lastColumn != ti_c0)) {
    setRowOutputs(c0rows);
    lastColumn = ti_c0;
  } else if (digitalRead(ti_c1) == LOW && (lastColumn != ti_c1)) {
    setRowOutputs(c1rows);
    lastColumn = ti_c1;
  } else if (digitalRead(ti_c2) == LOW && (lastColumn != ti_c2)) {
    setRowOutputs(c2rows);
    lastColumn = ti_c2;
  } else if (digitalRead(ti_c3) == LOW && (lastColumn != ti_c3)) {
    setRowOutputs(c3rows);
    lastColumn = ti_c3;
  } else if (digitalRead(ti_c4) == LOW && (lastColumn != ti_c4)) {
    setRowOutputs(c4rows);
    lastColumn = ti_c4;
  } else if (digitalRead(ti_c5) == LOW && (lastColumn != ti_c5)) {
    setRowOutputs(c5rows);
    lastColumn = ti_c5;
  } else if (digitalRead(ti_c6) == LOW && (lastColumn != ti_c6)) {
    setRowOutputs(c6rows);
    lastColumn = ti_c6;
  } else {
    clearOutputs();
    lastColumn = -1;
  }
}

#endif

