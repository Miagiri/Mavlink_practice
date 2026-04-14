#include "mavlink/minimal/mavlink.h"
#include <SoftwareSerial.h>

SoftwareSerial telemetrySerial(10, 11);

void setup() {
  Serial.begin(57600);
  telemetrySerial.begin(57600);
}

void loop() {
  mavlink_message_t msg;
  mavlink_status_t status;
  
  while (telemetrySerial.available() > 0) {
    if (mavlink_parse_char(MAVLINK_COMM_0, telemetrySerial.read(), &msg, &status)) {
      if (msg.msgid == 0) {  // 0 = Heartbeat
        Serial.println(millis() / 1000);
      }
    }
  }
}