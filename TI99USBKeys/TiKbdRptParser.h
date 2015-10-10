#ifndef _TI_KBD_RPT_PARSER_H
#define _TI_KBD_RPT_PARSER_H

#include <hidboot.h>


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
    void toggleKey(uint8_t key, int state);
    void toggleMod(uint8_t mod, int state);
    boolean specialCombos(uint8_t mod, uint8_t key, int state);

  public:
    void setKeyLocks(HID* hid, boolean numLock, boolean capsLock, boolean scrollLock);
};

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

void TiKbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

}

void TiKbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  if ( !specialCombos(mod, key, 1) ) {
    toggleMod(mod, 1);
    toggleKey(key, 1);
  }
}

void TiKbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  if ( !specialCombos(mod, key, 0) ) {
    toggleMod(mod, 0);
    toggleKey(key, 0);
  }
}

boolean TiKbdRptParser::specialCombos(uint8_t mod, uint8_t key, int state)
{
  switch (mod) {
    case 32: // Shift
    case 2:
      switch (key) {
        case 0x35: // ` -> ~
          *tk_Fctn = state;
          *tk_W = state;
          return true;
        case 0x2F: // [ -> {
          *tk_Fctn = state;
          *tk_F = state;
          return true;
        case 0x30: // ] -> }
          *tk_Fctn = state;
          *tk_G = state;
          return true;
        case 0x31: // \ -> |
          *tk_Fctn = state;
          *tk_A = state;
          return true;
        case 0x38: // / -> ?
          *tk_Fctn = state;
          *tk_I = state;
          return true;
        case 0x2D: // - -> _
          *tk_Fctn = state;
          *tk_U = state;
          return true;
        case 0x34: // ' -> "
          *tk_Fctn = state;
          *tk_P = state;
          return true;
      }
      break;
    case 64: // Alt
    case 4:
      switch (key) {
        case 0x2E: // =   QUIT 
          *tk_Fctn = 1;
          *tk_Equal = 1;
          return true;
      }
      break;
    case 5: // Control-Alt
    case 80:
    case 65:
    case 20:
      CPU_RESTART;
      break;
  }
  switch (key) {
    case 0x39: // caps lock
      *tk_Alpha = kbdLockingKeys.kbdLeds.bmCapsLock;
      break;
  }
  return false;
}

void TiKbdRptParser::toggleMod(uint8_t mod, int state)
{
  switch (mod) {
    case 32:
    case 2:
      *tk_Shift = state;
      break;
    case 64:
    case 4:
      *tk_Fctn = state;
      break;
    case 16:
    case 1:
      *tk_Ctrl = state;
      break;
  }
}

void TiKbdRptParser::toggleKey(uint8_t key, int state)
{
  switch (key) {
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
    case 0x13:
      *tk_P = state;
      break;
    case 0x38:
    case 0x54:
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
    // ----------- PC Unique keys
    case 0x2A: // backspace
    case 0x50: // left arrow
      if ( !kbdLockingKeys.kbdLeds.bmScrollLock ) {
        *tk_Fctn = state;
      }
      *tk_S = state;
      break;
    case 0x4F: // right arrow
      if ( !kbdLockingKeys.kbdLeds.bmScrollLock ) {
        *tk_Fctn = state;
      }
      *tk_D = state;
      break;
    case 0x52: // up arrow
      if ( !kbdLockingKeys.kbdLeds.bmScrollLock ) {
        *tk_Fctn = state;
      }
      *tk_E = state;
      break;
    case 0x51: // down arrow
      if ( !kbdLockingKeys.kbdLeds.bmScrollLock ) {
        *tk_Fctn = state;
      }
      *tk_X = state;
      break;
    case 0x3A: // F1
    case 0x4C: // Delete
      *tk_Fctn = state;
      *tk_1 = state;
      break;
    case 0x3B: // F2
    case 0x49: // Insert
      *tk_Fctn = state;
      *tk_2 = state;
      break;
    case 0x3C: // F3
      *tk_Fctn = state;
      *tk_3 = state;
      break;
    case 0x3D: // F4
    case 0x48: // Break
    case 0x4E: // PgDn
      *tk_Fctn = state;
      *tk_4 = state;
      break;
    case 0x3E: // F5
      *tk_Fctn = state;
      *tk_5 = state;
      break;
    case 0x3F: // F6
    case 0x4B: // PgUp
      *tk_Fctn = state;
      *tk_6 = state;
      break;
    case 0x40: // F7
    case 0x2B: // TAB
      *tk_Fctn = state;
      *tk_7 = state;
      break;
    case 0x41: // F8
      *tk_Fctn = state;
      *tk_8 = state;
      break;
    case 0x42: // F9
    case 0x29: // Esc
      *tk_Fctn = state;
      *tk_9 = state;
      break;
    case 0x43: // F10
      *tk_Fctn = state;
      *tk_0 = state;
      break;
    case 0x44: // F11
      *tk_Ctrl = state;
      *tk_1 = state;
      break;
    case 0x45: // F12
      *tk_Ctrl = state;
      *tk_2 = state;
      break;
    case 0x35: // ` - backquote
      *tk_Fctn = state;
      *tk_C = state;
      break;
    case 0x2D: // hyphen
    case 0x56: // hyphen numpad
      *tk_Shift = state;
      *tk_Slash = state;
      break;
    case 0x2F: // [
      *tk_Fctn = state;
      *tk_R = state;
      break;
    case 0x30: // ]
      *tk_Fctn = state;
      *tk_T = state;
      break;
    case 0x59: // 1 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        // act like the end key
        *tk_Ctrl = state;
        *tk_V = state;
      } else {
        *tk_1 = state;
      }
      break;
    case 0x5A: // 2 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_X = state;
      } else {
        *tk_2 = state;
      }
      break;
    case 0x5B: // 3 numpad
      if (! kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_4 = state;
      } else {
        *tk_3 = state;
      }
      break;
    case 0x5C: // 4 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_S = state;
      } else {
        *tk_4 = state;
      }
      break;
    case 0x5D: // 5 numpad
      *tk_5 = state;
      break;
    case 0x5E: // 6 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_D = state;
      } else {
        *tk_6 = state;
      }
      break;
    case 0x5F: // 7 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Ctrl = state;
        *tk_U = state;
      } else {
        *tk_7 = state;
      }
      break;
    case 0x60: // 8 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_E = state;
      } else {
        *tk_8 = state;
      }
      break;
    case 0x61: // 9 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_6 = state;
      } else {
        *tk_9 = state;
      }
      break;
    case 0x62: // 0 numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_2 = state;
      } else {
        *tk_0 = state;
      }
      break;
    case 0x63: // . numpad
      if ( !kbdLockingKeys.kbdLeds.bmNumLock ) {
        *tk_Fctn = state;
        *tk_1 = state;
      } else {
        *tk_Period = state;
      }
      break;
    case 0x31: // \ numpad
      *tk_Fctn = state;
      *tk_Z = state;
      break;
    case 0x55: // * numpad
      *tk_Shift = state;
      *tk_8 = state;
      break;
    case 0x57: // + numpad
      *tk_Shift = state;
      *tk_Equal = state;
      break;
    case 0x34: // '
      *tk_Fctn = state;
      *tk_O = state;
      break;
    case 0x4A: // Home
      *tk_Ctrl = state;
      *tk_U = state;
      break;
    case 0x4D: // End
      *tk_Ctrl = state;
      *tk_V = state;
      break;
  }
}

#endif

