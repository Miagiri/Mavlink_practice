#include "mavlink/common/mavlink.h"

#define TELEM_PORT Serial
#define BAUDRATE 19200

static int heartbeat_counter = 0;
static int optical_counter = 0;
static float integrated_x = 0.0;
static float integrated_y = 0.0;

static unsigned long last_heartbeat = 0;
static unsigned long last_optical = 0;

void setup() {
  TELEM_PORT.begin(BAUDRATE);
  delay(1000);
  pinMode(13, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  
  if (now - last_heartbeat >= 1000) {
    last_heartbeat = now;
    sendHeartbeat();
    
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
  }
  
  if (now - last_optical >= 100) {
    last_optical = now;
    sendOpticalFlowData();
  }
}

void sendHeartbeat() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  heartbeat_counter++;
  
  mavlink_msg_heartbeat_pack(
      1, MAV_COMP_ID_PERIPHERAL, &msg,
      MAV_TYPE_GIMBAL,
      MAV_AUTOPILOT_GENERIC,
      MAV_MODE_PREFLIGHT,
      0,
      MAV_STATE_ACTIVE
  );
  
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  TELEM_PORT.write(buf, len);
}

void sendOpticalFlowData() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  integrated_x += 0.01;
  integrated_y += 0.005;
  optical_counter++;
  
  uint32_t now_us = micros();
  
  // ГЛАВНОЕ ИЗМЕНЕНИЕ: system_id = 2 (не 1!)
  mavlink_msg_optical_flow_rad_pack(
      2,      // system_id - НЕ 1!
      1,      // component_id
      &msg,
      now_us,
      1,
      100000,
      integrated_x,
      integrated_y,
      0.0, 0.0, 0.0,
      2500,
      200,
      100000,
      2.5
  );
  
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  TELEM_PORT.write(buf, len);
}