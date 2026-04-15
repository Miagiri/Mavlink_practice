#include "mavlink/minimal/mavlink.h"
#include <SoftwareSerial.h>

// Если данные не идут - поменяйте местами 10 и 11
SoftwareSerial telemetrySerial(10, 11);  // RX=10, TX=11

int heartbeatCount = 0;

void setup() {
  Serial.begin(57600);
  telemetrySerial.begin(57600);
  
  delay(2000);
  Serial.println("=== MAVLink Heartbeat Receiver ===");
  Serial.println("Waiting for heartbeat...");
}

void loop() {
  mavlink_message_t msg;
  mavlink_status_t status;
  
  while (telemetrySerial.available() > 0) {
    uint8_t c = telemetrySerial.read();
    
    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        heartbeatCount++;
        
        Serial.print("[");
        Serial.print(millis() / 1000);
        Serial.print("s] Heartbeat #");
        Serial.println(heartbeatCount);
      }
    }
  }
}
