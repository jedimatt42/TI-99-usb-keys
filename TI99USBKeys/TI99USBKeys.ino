
#include <hidboot.h>
#include <usbhub.h>
#include <XBOXUSB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

//#define MDS_DEBUG

// Reboot support
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

#define NUMLOCK_STARTUP true
#define CAPSLOCK_STARTUP true
#define SCROLLLOCK_STARTUP false

// This is a hack to get keys released...
// A better solution would be to ensure all states are cleared
// for any key.
long fctnEqualsTimestamp = 0L;

#include "TiPins.h"

#include "TiVirtualState.h"

#include "TiScan.h"

#include "TiKbdRptParser.h"

USB     Usb;
//USBHub     Hub(&Usb);
//HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
//TiKbdRptParser Prs;

XBOXUSB Xbox(&Usb);

long lastGoodState;
long firstBoot;

void setup()
{
#ifdef MDS_DEBUG
  Serial.begin(115200);
  while(!Serial); // wait for pc serial.
  Serial.println("started");
#endif

  firstBoot = lastGoodState = millis();
  
  initPinModes();
  setColumnInterrupts();
  initData();

#ifdef MDS_DEBUG
  Serial.println("waiting for hid device.");
#endif

  // Wait for keyboard to be up
  while (Usb.Init() == -1)
    delay( 20 );

  // HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);
#ifdef MDS_DEBUG
  Serial.println("ready.");
#endif
}

int xboxMode = 0;

void loop()
{
  // Read USB input which updates the state of the in-memory keyboard matrix.
  Usb.Task();

  if (Xbox.Xbox360Connected) {
    if (xboxMode == 0) {
      #ifdef MDS_DEBUG
      Serial.println("Xbox controller connected.");
      #endif
      xboxMode = 1;
    }
    *tj_Up = Xbox.getButtonPress(UP);
    *tj_Down = Xbox.getButtonPress(DOWN);
    *tj_Left = Xbox.getButtonPress(LEFT);
    *tj_Right = Xbox.getButtonPress(RIGHT);
    *tj_Fire = Xbox.getButtonPress(A);
    #ifdef MDS_DEBUG
    Serial.print("joy1: ");
    Serial.print(*tj_Up);
    Serial.print(", ");
    Serial.print(*tj_Down);
    Serial.print(", ");
    Serial.println(*tj_Fire);
    #endif
  } else {
    xboxMode = 0;
    // Keyboard handling
    long loopMillis = millis();
    uint8_t state = Usb.getUsbTaskState();
    
    if (state != USB_STATE_RUNNING) {
      long rightnow = loopMillis;
      if ( (rightnow - lastGoodState) > 5000 ) {
        CPU_RESTART;
      }
    } else {
      lastGoodState = loopMillis;
      if (firstBoot != 0) {
        // Set numlock and capslock on, leave scroll lock off.
        // Prs.setKeyLocks(&HidKeyboard, NUMLOCK_STARTUP, CAPSLOCK_STARTUP, SCROLLLOCK_STARTUP);
        firstBoot = 0; 
      }
    }
    if (fctnEqualsTimestamp != 0) {
      if ( (loopMillis - fctnEqualsTimestamp) > 150 ) {
        fctnEqualsTimestamp = 0L;
        *tk_Fctn = 0;
        *tk_Equal = 0;
        setRowOutputs(c0rows);
      }
    }
  }
}

