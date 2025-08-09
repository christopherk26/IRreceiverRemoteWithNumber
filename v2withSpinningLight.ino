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

// Variables for spinning animation
bool isSpinning = false;
unsigned long lastSpinTime = 0;
int currentSegment = 0;
const int spinDelay = 150; // milliseconds between segment changes

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
  Serial.println("Press 0-9 to show numbers, Play button to spin!");
}

void loop() {
  // Handle IR input
  if (irrecv.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    
    long int code = irrecv.decodedIRData.decodedRawData;
    
    // Check for play button
    if (code == 0xBF40FF00) {
      Serial.println("Play button pressed - starting spin animation!");
      isSpinning = !isSpinning; // Toggle spinning on/off
      if (isSpinning) {
        currentSegment = 0;
        lastSpinTime = millis();
      } else {
        clearDisplay();
      }
    }
    // Check for number buttons
    else {
      int number = mapIRCodeToNumber(code);
      
      if (number >= 0 && number <= 9) {
        Serial.print("Displaying number: ");
        Serial.println(number);
        isSpinning = false; // Stop spinning when showing a number
        displayDigit(number);
      } else if (code == 0x0) {
        Serial.println("Repeat signal - ignoring");
      } else {
        Serial.println("Unknown button - clearing display");
        isSpinning = false; // Stop spinning
        clearDisplay(); // Turn off all lights
      }
    }
    
    irrecv.resume();  // Receive the next value
  }
  
  // Handle spinning animation
  if (isSpinning) {
    unsigned long currentTime = millis();
    if (currentTime - lastSpinTime >= spinDelay) {
      spinSegments();
      lastSpinTime = currentTime;
    }
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

// Spin the segments in a circle: a->b->c->d->e->f->repeat (no middle segment G)
void spinSegments() {
  clearDisplay(); // Turn off all segments first
  
  // Light up the current segment (only outer segments, no middle G)
  switch(currentSegment) {
    case 0: digitalWrite(segA, HIGH); break; // top
    case 1: digitalWrite(segB, HIGH); break; // top right
    case 2: digitalWrite(segC, HIGH); break; // bottom right
    case 3: digitalWrite(segD, HIGH); break; // bottom
    case 4: digitalWrite(segE, HIGH); break; // bottom left
    case 5: digitalWrite(segF, HIGH); break; // top left
  }
  
  // Move to next segment
  currentSegment++;
  if (currentSegment > 5) {
    currentSegment = 0; // Loop back to start (only 6 segments: a,b,c,d,e,f)
  }
}
