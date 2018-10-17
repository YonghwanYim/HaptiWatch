#include <Boards.h>
#include <Firmata.h>
#include <FirmataConstants.h>
#include <FirmataDefines.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

/*
 * Project Title : Smartwatch Prototype
 * Author : Yonghwan Yim
 * Experience Innovation Design Lab.
 * Department of Industrial Engineering
 * Hongik University, Seoul, Korea 
 
 * Final Update : 2017.09.11
 */
// Library List //
#include <Adafruit_NeoPixel.h>  // NeoPixel Library.
#include <SoftwareSerial.h>     // Bluetooth Library.
#include <avr/power.h>

// NeoPixel //
#define NEO_VCC 5
#define NEO_GROUND 6
#define NEO_PIN 7      // Digital Pin (Signal)
#define LEDNUM 4      // NeoPixel LED Number.
#define BRIGHTNESS 50  // NeoPixel Brightness Setting (0~255)

// Vibration Motor //
#define MOTOR_VCC 8
#define MOTOR_GND 9

// HC-06, Bluetooth Module //
//#define BT_RXD 0
//#define BT_TXD 1
//#define BT_GND 11
//#define BT_VCC 10
                                                                                                
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, NEO_PIN, NEO_GRB + NEO_KHZ800); // NeoPixel Setup.
//SoftwareSerial BTSerial(BT_TXD, BT_RXD);  // Bluetooth Pin Setup.

// Global Variable //
String Command_String = ""; 
String Split_result[6];       // Save the result of the split function.
uint32_t color_level3 = strip.Color(255,0,0);
uint32_t color_level2 = strip.Color(255,127,127);
uint32_t color_level1 = strip.Color(255,255,255);
uint32_t color_off = strip.Color(0,0,0);

// Function Declaration //
void Split(String sData, char cSeparator);
void Pulse_1(uint32_t color_input, double dela_y, int LED_n);  // Pulse 1 - Heartbeat
void Pulse_2(uint32_t color_input, double dela_y, int LED_n);  // Pulse 2 - Pulse Fast
void Pulse_3(uint32_t color_input, double dela_y, int LED_n);  // Pulse 3 - Blink Decreasing
void Pulse_4(uint32_t color_input, double dela_y, int LED_n);  // Pulse 4 - SOS Blink
void Pulse_5(uint32_t color_input, double dela_y, int LED_n);  // Pulse 5 - Blink Increasing


void setup() {
  //BTSerial.begin(9600);
  Serial.begin(9600);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1) // Since clocks are different for each CPU,
#endif                                                   // they are defined to match the real time.
  while (!Serial);  // Wait until serial communication is available.
  delay(200);
  Serial.println("Hi Yonghwan, Input Command.(App-Number  Notification-Level)"); //********** Temporary code***************

  // Pin Mode Setting //
  pinMode(NEO_VCC, OUTPUT);
  pinMode(NEO_GROUND, OUTPUT);
  pinMode(MOTOR_VCC, OUTPUT);
  pinMode(MOTOR_GND, OUTPUT);
  //pinMode(BT_VCC, OUTPUT);
  //pinMode(BT_GND, OUTPUT);
  
  // VCC & Ground Setting //
  digitalWrite(NEO_VCC, HIGH);
  digitalWrite(NEO_GROUND, LOW);  // NeoPixel GROUND, 0V
  digitalWrite(MOTOR_GND, LOW);   // Vibration Motor GND, 0V
  //digitalWrite(BT_VCC, HIGH);
  //digitalWrite(BT_GND, LOW);
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();   // Initialize all pixels to 'off'
}

void loop() {
  /*
  if (BTSerial.available()) {       
    Serial.write(BTSerial.read()); 
  }
  if (Serial.available()) {         
    BTSerial.write(Serial.read()); 
  }
  */
  strip.begin();  // NeoPixel Control Start.
  strip.show();   // NeoPixel Reset.
  while(Serial.available())  // If there is a value sent to Serial.
    Command_String = Serial.readString();
  if(!Command_String.equals(""))  // If there is a Command String Value.
  {
    for(int i = 0; i < 6; i++)
      Split_result[i] = "\n";
    Split(Command_String, ' ');
    
    if(Split_result[0] == "reset")  // Reset Command.
    {
      for(int i = 0; i < 4; i++)
      {
         strip.setPixelColor(i, color_off);
         strip.show();
      }
      Serial.println("Reset Complete.");
    }
    
    switch(Split_result[0].toInt())  // toInt() : "char type" -> "int type"
    {
      case 1:
        switch(Split_result[1].toInt())
        {
          case 1:
            Serial.println("App 1 - level 1, Notification send complete.");  //*************************** Temporary code ******
            strip.setPixelColor((Split_result[0].toInt() - 1), color_level1);
            strip.show(); break;
          case 2:
            Serial.println("App 1 - level 2, Notification send complete.");  //*************************** Temporary code ******
            Pulse_1(color_level2, 1.5, Split_result[0].toInt()); break;
          case 3:
            Serial.println("App 1 - level 3, Notification send complete.");  //*************************** Temporary code ******
            Pulse_1(color_level3, 1.0, Split_result[0].toInt()); break;
        }
        break;
      case 2:
        switch(Split_result[1].toInt())
        {
          case 1:
            Serial.println("App 2 - level 1, Notification send complete.");  //*************************** Temporary code ******
            strip.setPixelColor((Split_result[0].toInt() - 1), color_level1);
            strip.show(); break;
          case 2:
            Serial.println("App 2 - level 2, Notification send complete.");  //*************************** Temporary code ******
            Pulse_2(color_level2, 1.5, Split_result[0].toInt()); break;
          case 3:
            Serial.println("App 2 - level 3, Notification send complete.");  //*************************** Temporary code ******
            Pulse_2(color_level3, 1.0, Split_result[0].toInt()); break;
        }
        break;
      case 3:
        switch(Split_result[1].toInt())
        {
          case 1:
            Serial.println("App 3 - level 1, Notification send complete.");  //*************************** Temporary code ******
            strip.setPixelColor((Split_result[0].toInt() - 1), color_level1);
            strip.show(); break;
          case 2:
            Serial.println("App 3 - level 2, Notification send complete.");  //*************************** Temporary code ******
            Pulse_3(color_level2, 1.5, Split_result[0].toInt()); break;
          case 3:
            Serial.println("App 3 - level 3, Notification send complete.");  //*************************** Temporary code ******
            Pulse_3(color_level3, 1.0, Split_result[0].toInt()); break;
        }
        break;
      case 4:
        switch(Split_result[1].toInt())
        {
          case 1:
            Serial.println("App 4 - level 1, Notification send complete.");  //*************************** Temporary code ******
            strip.setPixelColor((Split_result[0].toInt() - 1), color_level1);
            strip.show(); break;
          case 2:
            Serial.println("App 4 - level 2, Notification send complete.");  //*************************** Temporary code ******
            //Pulse_4(color_level2, 1.5, Split_result[0].toInt()); break;
            Pulse_5(color_level2, 1.5, Split_result[0].toInt()); break;
          case 3:
            Serial.println("App 4 - level 3, Notification send complete.");  //*************************** Temporary code ******
            //Pulse_4(color_level3, 1.0, Split_result[0].toInt()); break;
            Pulse_5(color_level3, 1.0, Split_result[0].toInt()); break;
        }
        break;
    }  
    Command_String = ""; // Reset, Command String.
  }
}

void Split(String sData, char cSeparator)
{  
  int nGetIndex = 0 ;
  int msgCount = 0;
  String sTemp = "";    // Temporary storage.
  String sCopy = sData; // Copy original.

  while(true)
  {
    // Find separator.
    nGetIndex = sCopy.indexOf(cSeparator);

    // Is there a returned index?
    if(-1 != nGetIndex)
    {
      // Loading data.
      sTemp = sCopy.substring(0, nGetIndex); // Copy from 0 to nGetIndex.
      Split_result[msgCount] = sTemp;
      msgCount++;
    
      // Cut the data.
      sCopy = sCopy.substring(nGetIndex + 1);
    }
    else
    {
      // If there is no separator, finish.
      Split_result[msgCount] = sCopy;
      break;
    }
  }
}

/////////////////////////////////////////////////// Pulse 1 - Heartbeat ///////////////////////////////////////////////////////////////////
void Pulse_1(uint32_t color_input, double dela_y, int LED_n)  // Heartbeat
{
  for(uint16_t i = 0; i < 2; i++)
  {
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     delay(100);
  
     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);
  
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(300 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(800 * dela_y);
  }
  strip.setPixelColor((LED_n - 1), color_input);
  strip.show();
}

/////////////////////////////////////////////////// Pulse 2 - Pulse Fast //////////////////////////////////////////////////////////////////
void Pulse_2(uint32_t color_input, double dela_y, int LED_n)
{
  for(uint16_t i = 0; i < 2; i++)
  {
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     delay(100);
     
     for(uint16_t i = 0; i < 4; i++)
     {
        strip.setPixelColor((LED_n - 1), color_input);
        strip.show();
        digitalWrite(MOTOR_VCC, HIGH);
        delay(145 * dela_y);
  
        strip.setPixelColor((LED_n - 1), color_off);
        strip.show();
        digitalWrite(MOTOR_VCC, LOW);
        delay(145 * dela_y);  
     }
     
     delay(655 * dela_y);  // 800 - 145 = 655
  }
  strip.setPixelColor((LED_n - 1), color_input);
  strip.show();
}

/////////////////////////////////////////////////// Pulse 3 - Blink Decreasing ////////////////////////////////////////////////////////////
void Pulse_3(uint32_t color_input, double dela_y, int LED_n)
{
  for(uint16_t i = 0; i < 2; i++)
  {
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     delay(100);
  
     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(60 * dela_y);
  
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(60 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(90 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(90 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(130 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(130 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(170 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(170 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(210 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(210 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(250 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(800 * dela_y);
  }
  strip.setPixelColor((LED_n - 1), color_input);
  strip.show();
}

/////////////////////////////////////////////////// Pulse 4 - SOS Blink ////////////////////////////////////////////////////////////
void Pulse_4(uint32_t color_input, double dela_y, int LED_n)
{
  for(uint16_t i = 0; i < 2; i++)
  {
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     delay(100);
  
     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);
  
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(100 * dela_y);     

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(100 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);
  
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(50 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(800 * dela_y);
  }
  strip.setPixelColor((LED_n - 1), color_input);
  strip.show();
}

/////////////////////////////////////////////////// Pulse 5 - Blink Increasing ////////////////////////////////////////////////////////////
void Pulse_5(uint32_t color_input, double dela_y, int LED_n)
{
  for(uint16_t i = 0; i < 2; i++)
  {
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     delay(100);
  
     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(250 * dela_y);
  
     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(210 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(210 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(170 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(170 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(130 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(130 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(90 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(90 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(60 * dela_y);

     strip.setPixelColor((LED_n - 1), color_input);
     strip.show();
     digitalWrite(MOTOR_VCC, HIGH);
     delay(60 * dela_y);

     strip.setPixelColor((LED_n - 1), color_off);
     strip.show();
     digitalWrite(MOTOR_VCC, LOW);
     delay(800 * dela_y);
  }
  strip.setPixelColor((LED_n - 1), color_input);
  strip.show();
}

