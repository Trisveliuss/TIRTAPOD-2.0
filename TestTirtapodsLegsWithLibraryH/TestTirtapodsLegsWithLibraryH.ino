#include "legs.h"
#include <SoftwareSerial.h>
SoftwareSerial servoserial(2, 3);
bool nanjak=true;
int s=1000;
void setup() {

servoserial.begin(9600);
legs::setup();
}

void loop() {
legs::testkoordinat();
}
