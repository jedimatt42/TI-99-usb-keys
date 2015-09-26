
#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

// Reboot support
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

#include "TiPins.h"

#include "TiVirtualState.h"

#include "TiKbdRptParser.h"

// This is a hack to get keys released... 
// A better solution would be to ensure all states are cleared
// for any key.
long fctnEqualsTimestamp = 0L;

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

TiKbdRptParser Prs;

long lastGoodState;

void setup()
{
  lastGoodState = millis();
  pinMode(14, OUTPUT);
  
  initPinModes();
  setColumnInterrupts();
  initData();

  // Serial.begin(9600);

  // Wait for keyboard to be up? This doesn't come close to working.
  while (Usb.Init() == -1)
    delay( 200 );

  HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
  // Read USB input which updates the state of the in-memory keyboard matrix.
  Usb.Task();
  long loopMillis = millis();
  uint8_t state = Usb.getUsbTaskState();
  if (state != USB_STATE_RUNNING) {
    long rightnow = loopMillis;
    if ( (rightnow - lastGoodState) > 5000 ) {
      CPU_RESTART; 
    }
  } else {
    lastGoodState = loopMillis;
  }
  if (fctnEqualsTimestamp != 0) {
    if ( (loopMillis - fctnEqualsTimestamp) > 150 ) {
      fctnEqualsTimestamp = 0L;
      *tk_Fctn = 0;
      *tk_Equal = 0;
      setRowOutputs(c0rows);
    }
  }
  // For debugging the shift lock behavior. Turns an LED on if the virtual key matrix thinks shift is down.
  // I'll leave this in a bit while I work on the ability to hold shift down!
  digitalWrite(14, *tk_Shift ? HIGH : LOW );
}

