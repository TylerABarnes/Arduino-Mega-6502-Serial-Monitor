---Credit Due To Ben Eater For The Foundation Of This Code---

const char ADDR[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A1, A2, A3, A4, A5};
const char DATA[] = {A8, A9, A10, A11, A12, A13, A14, A15};
#define CLOCK 2
#define READ_WRITE 46

void setup() {
  for (int n = 0; n < 16; n += 1) {                //Address Inputs
    pinMode(ADDR[n], INPUT); 
  }
  for (int n = 0; n < 8; n += 1) {                 //Data Inputs
    pinMode(DATA[n], INPUT); 
  }
  pinMode(CLOCK, INPUT);                           //Clock Input
  pinMode(READ_WRITE, INPUT);                      //Indicates if data is being written to or read by

  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);    //Monitor Updates Per Clock (only works Pin 2)
  
  Serial.begin(57600);                             //Standard Speed
}

void onClock() {
  char output [15];
  unsigned int address = 0;
  unsigned int modified_address = 0;
  for (int n = 0; n < 16; n += 1) {                //Address Inputs
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    Serial.print(bit);                             //Displaying Address
    address = (address << 1) + bit;
    if (n < 11) {                                 //Only take the first 11 bits into consideration for modified_address representation where in the AT28C16 16K (2K x 8) the CPU is actually looking
                                                  //(Given the last 5-bits are just disconnected to lessen restrictions
      modified_address = (modified_address << 1) + bit;
    }
  }

  unsigned int reversed_address = 0;
  for (int i = 0; i < 11; i++) {
    reversed_address = (reversed_address << 1) | (modified_address & 1);
    modified_address >>= 1;
  }

  Serial.print("   ");                             //Space between address and data

  // Print decimal representation of "address"
  Serial.print(address);

  Serial.print("   "); 

  // Print decimal representation of "reversed_address"
  Serial.print(reversed_address);

  Serial.print("   ");  

  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {               //Data Inputs
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    Serial.print(bit);                           //Displaying Data
    data = (data << 1) + bit;
  }
sprintf(output, "   %04x  %c  %02x", address, digitalRead(READ_WRITE) ? 'R' : 'W', data);
  Serial.println(output);
}

void loop() {                                     //Loop doesn't do anything, only updates on clock pulse
}
