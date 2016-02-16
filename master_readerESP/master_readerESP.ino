#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin(); 
  delay(1000);

}
void loop() {
  Wire.requestFrom(10, 8); 
  int c1 = Wire.read(); 
  int c2 = Wire.read();
  int c3 = Wire.read() << 8 | Wire.read();
  float data = c3 / 1000.00;
  int c5 = Wire.read();
  int c6 = Wire.read();
  int c7 = Wire.read();
  int c8 = Wire.read();
  if (c7 == 1) {
    Serial.print("AMP");
    Serial.print(" ");
    if (c5 == 0) {
      Serial.print(data);
      Serial.print(" ");
    } else {
      Serial.print(-data);
      Serial.print(" ");
    }
    Serial.print("A");

  } else if (c7 == 3) {
    Serial.print("VAC");
    Serial.print(" ");
    if (c5 == 0) {
      Serial.print(data);
      Serial.print(" ");
    } else {
      Serial.print(-data);
      Serial.print(" ");
    }
    if (c8 == 1) {
      Serial.print("V");
    } else {
      Serial.print("mV");
    }
  } else if (c7 == 4) {
    Serial.print("VDC");
    Serial.print(" ");
    if (c5 == 0) {
      Serial.print(data);
      Serial.print(" ");
    } else {
      Serial.print(-data);
      Serial.print(" ");
    }
    if (c8 == 1) {
      Serial.print("V");
    } else {
      Serial.print("mV");
    }
  } else if (c7 == 6) {
    Serial.print("TEMP");
    Serial.print(" ");
    if (c5 == 0) {
      Serial.print(data);
      Serial.print(" ");
    } else {
      Serial.print(-data);
      Serial.print(" ");
    }
    Serial.print("C");
  } else {
    Serial.print("Fuck you");
    Serial.print(" ");
  }

    Serial.println(" ");

  delay(500);
}
