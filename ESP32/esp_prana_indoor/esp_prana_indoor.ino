
int pm2 = 0;
int pm10 = 0;

byte command_frame[9] = {0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB};
byte received_data[9];
int sum = 0;

void send_command(byte command) {
  command_frame[1] = command;
  sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
  int rem = sum % 256;
  command_frame[6] = (sum - rem) / 256;
  command_frame[7] = rem;
  delay(1000);
  Serial.write(command_frame, 9);
}

bool checksum() {
  sum = received_data[0] + received_data[1] + received_data[2] + received_data[3] + received_data[4] + received_data[5] + received_data[8];
  return sum == (received_data[6] * 256 + received_data[7]);
}

void calculate_pm() {
  pm2 = received_data[4] * 256 + received_data[5];
  delay(2000);
  pm10 = received_data[2] * 256 + received_data[3];
  Serial.print("PM2.5: ");
  Serial.println(pm2);
  Serial.print("PM10: ");
  Serial.println(pm10);
  Serial.println("----------PM updated successfully!-------------");
}

void PM_setup() {
  send_command(0x01);
}

void PM_Reading() {
  delay(5000);
  if (millis() > 5000) {
    send_command(0x02);
  }
  if (Serial.available()) {
    Serial.readBytes(received_data, 9);
    if (checksum()) {
      calculate_pm();
    }
  }
}

void setup() {
  Serial.begin(9600);
  PM_setup();
  delay(1000);
}

void loop() {
  PM_Reading();
  Serial.println("Loop completed, waiting for 5 seconds...");
  delay(5000);
}