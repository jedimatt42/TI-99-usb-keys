#ifndef _TI_KBD_RPT_PARSER_H
#define _TI_KBD_RPT_PARSER_H

#include <hidboot.h>

#include "USBCodes.h"

//-----------------------------------------------
// USB Keyboard input handling - mostly taken
//   from the USB Host Boot Keyboard example.

class TiKbdRptParser : public KeyboardReportParser
{
  protected:
    virtual void OnControlKeysChanged(uint8_t before, uint8_t after);
    virtual void OnKeyDown	(uint8_t mod, uint8_t key);
    virtual void OnKeyUp	(uint8_t mod, uint8_t key);

  private:
    void updateModifier(uint8_t mask, uint8_t before, uint8_t after, int* tk);
    boolean handleSimple(uint8_t key, int state);

  public:
    TiKbdRptParser();
    void setKeyLocks(HID* hid, boolean numLock, boolean capsLock, boolean scrollLock);
};

TiKbdRptParser::TiKbdRptParser()
{
}

void TiKbdRptParser::setKeyLocks(HID* hid, boolean numLock, boolean capsLock, boolean scrollLock)
{ 
  if (numLock) {
    kbdLockingKeys.kbdLeds.bmNumLock = 1;
  }
  if (capsLock) {
    kbdLockingKeys.kbdLeds.bmCapsLock = 1;
    *tk_Alpha = 1;
  }
  if (scrollLock) {
    kbdLockingKeys.kbdLeds.bmScrollLock = 1;
  }
  hid->SetReport(0, 0, 2, 0, 1, &(kbdLockingKeys.bLeds));
}

void TiKbdRptParser::updateModifier(uint8_t mask, uint8_t before, uint8_t after, int* tk)
{
  boolean wasMod = before & mask;
  boolean isMod = after & mask;
  if (wasMod && (!isMod)) {
    tk_release(tk);
  } else if (isMod && (!wasMod)) {
    tk_press(tk);
  }
}

void TiKbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  updateModifier(U_LEFTSHIFT | U_RIGHTSHIFT, before, after, tk_Shift);
  updateModifier(U_LEFTALT | U_RIGHTALT, before, after, tk_Fctn);
  updateModifier(U_LEFTCTRL | U_RIGHTCTRL, before, after, tk_Ctrl);
}

void TiKbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  handleSimple(key, 1);
}

void TiKbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  if (handleSimple(key, 0)) return;
  
  switch(key) {
    case U_CAPSLOCK:
      *tk_Alpha = kbdLockingKeys.kbdLeds.bmCapsLock;
      break;
  }
}

#define SCASE(X, Y) case X: *Y = state; return true;

// Handle the keys that are a one to one mapping, with no modifier issues.
boolean TiKbdRptParser::handleSimple(uint8_t key, int state)
{
  switch(key) {
    SCASE(U_NUM1,tk_1)
    SCASE(U_NUM2,tk_2)
    SCASE(U_NUM3,tk_3)
    SCASE(U_NUM4,tk_4)
    SCASE(U_NUM5,tk_5)
    SCASE(U_NUM6,tk_6)
    SCASE(U_NUM7,tk_7)
    SCASE(U_NUM8,tk_8)
    SCASE(U_NUM9,tk_9)
    SCASE(U_NUM0,tk_0)
    SCASE(U_A,tk_A)
    SCASE(U_B,tk_B)
    SCASE(U_C,tk_C)
    SCASE(U_D,tk_D)
    SCASE(U_E,tk_E)
    SCASE(U_F,tk_F)
    SCASE(U_G,tk_G)
    SCASE(U_H,tk_H)
    SCASE(U_I,tk_I)
    SCASE(U_J,tk_J)
    SCASE(U_K,tk_K)
    SCASE(U_L,tk_L)
    SCASE(U_M,tk_M)
    SCASE(U_N,tk_N)
    SCASE(U_O,tk_O)
    SCASE(U_P,tk_P)
    SCASE(U_Q,tk_Q)
    SCASE(U_R,tk_R)
    SCASE(U_S,tk_S)
    SCASE(U_T,tk_T)
    SCASE(U_U,tk_U)
    SCASE(U_V,tk_V)
    SCASE(U_W,tk_W)
    SCASE(U_X,tk_X)
    SCASE(U_Y,tk_Y)
    SCASE(U_Z,tk_Z)
    SCASE(U_COMMA,tk_Comma)
    SCASE(U_PERIOD,tk_Period)
    SCASE(U_EQUAL,tk_Equal)
    SCASE(U_SEMICOLON,tk_Semicolon)
    SCASE(U_SPACE,tk_Space)
    SCASE(U_ENTER,tk_Enter)
  }
  return false;
}


#endif

