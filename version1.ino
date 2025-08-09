#include <IRremote.h>

// IR Receiver pin
const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);

// 7-segment display pins (segments a, b, c, d, e, f, g)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;

// Segment patterns for digits 0-9 (Common Cathode: HIGH = on, LOW = off)
const byte digits[10][7] = {
  // a, b, c, d, e, f, g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  // Start the receiver
  
  // Setup segment pins as outputs
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  
  // Clear display on startup
  clearDisplay();
  Serial.println("IR Remote 7-Segment Display Ready!");
}

void loop() {
  if (irrecv.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    
    // Map IR code to number
    int number = mapIRCodeToNumber(irrecv.decodedIRData.decodedRawData);
    
    if (number >= 0 && number <= 9) {
      Serial.print("Displaying number: ");
      Serial.println(number);
      displayDigit(number);
    } else if (number == -2) {
      Serial.println("Repeat signal - ignoring");
    } else {
      Serial.println("Unknown button - clearing display");
      clearDisplay();
    }
    
    irrecv.resume();  // Receive the next value
  }
}

// Map your specific IR codes to numbers
int mapIRCodeToNumber(long int code) {
  switch(code) {
    case 0xE916FF00: return 0;
    case 0xF30CFF00: return 1;
    case 0xE718FF00: return 2;
    case 0xA15EFF00: return 3;
    case 0xF708FF00: return 4;
    case 0xE31CFF00: return 5;
    case 0xA55AFF00: return 6;
    case 0xBD42FF00: return 7;
    case 0xAD52FF00: return 8;
    case 0xB54AFF00: return 9;
    case 0x0: return -2; // Repeat signal
    default: return -1; // Unknown code
  }
}

// Display a digit on the 7-segment display
void displayDigit(int num) {
  if (num >= 0 && num <= 9) {
    digitalWrite(segA, digits[num][0]);
    digitalWrite(segB, digits[num][1]);
    digitalWrite(segC, digits[num][2]);
    digitalWrite(segD, digits[num][3]);
    digitalWrite(segE, digits[num][4]);
    digitalWrite(segF, digits[num][5]);
    digitalWrite(segG, digits[num][6]);
  }
}

// Clear the display (turn off all segments)
void clearDisplay() {
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
