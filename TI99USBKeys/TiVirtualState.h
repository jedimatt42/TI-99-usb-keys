#ifndef _TI_VIRTUAL_STATE_H
#define _TI_VIRTUAL_STATE_H

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

int joy1rows[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// map symbols for the address of column/row pairs ( a key on the TI matrix )
// column 0 (= space enter fctn shift ctrl)
int* tk_Equal = c0rows;
int* tk_Space = c0rows+1;
int* tk_Enter = c0rows+2;
int* tk_Fctn = c0rows+4;
int* tk_Shift = c0rows+5;
int* tk_Ctrl = c0rows+6;
// column 1 (. L O 9 2 S W X)
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

int* tj_Fire = joy1rows;
int* tj_Left = joy1rows+1;
int* tj_Right = joy1rows+2;
int* tj_Down = joy1rows+3;
int* tj_Up = joy1rows+4;

void clearRow(int* rows)
{
  for( int i = 0; i < 8; i++ ) {
    rows[i] = 0;
  }
}

void initData() 
{
  fctnEqualsTimestamp = 0L;
  clearRow(c0rows);
  clearRow(c1rows);
  clearRow(c2rows);
  clearRow(c3rows);
  clearRow(c4rows);
  clearRow(c5rows);
  clearRow(c6rows);
  clearRow(joy1rows);
}


#endif

