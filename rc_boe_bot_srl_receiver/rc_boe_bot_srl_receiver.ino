#define IS_RECEIVER_LBB 1
#ifdef IS_REMOTE_LBB
#undef IS_REMOTE_LBB
#endif


#define NOTE_C4  262
 

static bool please_dump_info = true;


static const uint8_t left_flag = 1 << 0;
static const uint8_t bkwd_flag = 1 << 1;
static const uint8_t frwd_flag = 1 << 2;
static const uint8_t rght_flag = 1 << 3;
static const uint8_t honk_flag = 1 << 4;


static const uint8_t left_btn_pin = 1;
static const uint8_t bkwd_btn_pin = 2;
static const uint8_t frwd_btn_pin = 3;
static const uint8_t rght_btn_pin = 4;
static const uint8_t honk_btn_pin = 5;
// static const uint8_t speaker_pin  = 5;
static const uint8_t off_pin      = 255;


static const int step_dur = 50;                 // In milliseconds
static const int honk_dur = 1000 + 1000 + 250;  // In milliseconds




uint32_t abs_start;
void setup() {
  // // Serial port is initialized automatically; we don't have to do anything
  // while(!Serial);

  Serial.begin(57600);

  abs_start = millis();

  pinMode(left_btn_pin, OUTPUT);
  pinMode(bkwd_btn_pin, OUTPUT);
  pinMode(frwd_btn_pin, OUTPUT);
  pinMode(rght_btn_pin, OUTPUT);
  pinMode(honk_btn_pin, OUTPUT);
  // pinMode(speaker_pin, OUTPUT);

  // Note:  Active pin is LOW
  digitalWrite(left_btn_pin, HIGH);
  digitalWrite(bkwd_btn_pin, HIGH);
  digitalWrite(frwd_btn_pin, HIGH);
  digitalWrite(rght_btn_pin, HIGH);
  digitalWrite(honk_btn_pin, HIGH);
  // digitalWrite(speaker_pin, LOW);

  while(!Serial);
}


void blink_led(unsigned int p, unsigned int d) {
  // Serial.print("got: "); Serial.print(p); Serial.print(", "); Serial.println(d);
  digitalWrite(p, LOW);
  Bean.sleep(d);
  digitalWrite(p, HIGH);
}


void loop() {
  if (Serial.available()) {
    String s = Serial.readString(); // Until('\n');
    int d = 0;
    if (s[0] != 'h') {
      int t = s.substring(2).toInt();
      d = t / step_dur * step_dur;
    }
    Serial.print(s[0]); Serial.print(": "); Serial.println(d);
    switch (s[0]) {
      case 'h':
        blink_led(honk_btn_pin, honk_dur);
        // honk(1);
        break;
      case 'f':
        blink_led(frwd_btn_pin, d);
        break;
      case 'b':
        blink_led(bkwd_btn_pin, d);
        break;
      case 'r':
        blink_led(rght_btn_pin, d);
        break;
      case 'l':
        blink_led(left_btn_pin, d);
        break;
      default:
        Bean.sleep(50);
    }
  } else {
    Bean.sleep(50);
  }
}


// void honk(unsigned int n) {
//   for (int i = 0; i < n; i++) {
//     tone(speaker_pin, NOTE_C4, 1000);        // Play tone for 1 second
//     // Bean.sleep(1000);                        // Delay to finish tone
//     delay(1000);                             // Delay to finish tone
//     noTone(speaker_pin);
//     // if (i+1 < n) delay(250);                // Only delay between honks
//     if (i+1 < n) Bean.sleep(250);            // Only delay between honks
//   }
// }


// // void loop() {
// //   uint32_t start = millis();
// // 
// //   // ObseverAdvertisementInfo info;
// //   int res = Bean.getObserverMessage(&info, 2000);
// //   if (res == 1) {
// //     // dump_info(info);
// // 
// //     // Serial.print(info.eventType);
// //     // Serial.print(',');
// //     // Serial.print(info.addrType);
// //     // Serial.print(',');
// //     // Serial.print("addr: ");
// //     // for (int i = 0; i < 6; i++) {
// //     //   Serial.print(info.addr[i]);
// //     //   Serial.print(',');
// //     // }
// //     // Serial.print("  advData: ");
// //     // for (int i = 0; i < info.dataLen; i++) {
// //     //   Serial.print(info.advData[i], HEX);
// //     //   Serial.print(',');
// //     // }
// //     // Serial.println();
// // 
// //     // check to see if this packet is form the custom advertiser
// //     bool isEqual = true;
// //     // for (int i = 0; i < sizeof(expectedPacket) - 1; i++) {
// //     //   if (expectedPacket[i] != info.advData[i]) {
// //     //     isEqual = false;
// //     //   }
// //     // }
// //     isEqual = as_expected(info);
// // 
// // 
// //     if (isEqual) {
// //       print_data(info);
// //       // Serial.print("Custom adv found:  ");
// //       // Serial.println(info.advData[6]);
// //       update_button_state((uint8_t) info.advData[6]);
// //       if (info.advData[6] == 0) {
// //         Bean.setLedGreen(60);
// //       } else {
// //         Bean.setLedGreen(0);
// //       }
// //     }
// //   }
// // }
// 
// 
// void print_data(ObseverAdvertisementInfo info) {
//   if (please_dump_info) {
//     Serial.print("Custom adv found:  ");
//     Serial.println(info.advData[6]);
//   }
// }
// 
// 
// void dump_info(ObseverAdvertisementInfo info) {
//   if (please_dump_info) {
//     Serial.print(info.eventType);
//     Serial.print(',');
//     Serial.print(info.addrType);
//     Serial.print(',');
//     Serial.print("addr: ");
//     for (int i = 0; i < 6; i++) {
//       Serial.print(info.addr[i]);
//       Serial.print(',');
//     }
//     Serial.print("  advData: ");
//     for (int i = 0; i < info.dataLen; i++) {
//       Serial.print(info.advData[i], HEX);
//       Serial.print(',');
//     }
//     Serial.println();
//   }
// }
// 
// 
// bool as_expected(ObseverAdvertisementInfo info) {
//   for (int i = 0; i < sizeof(expectedPacket) - 1; i++) {
//     if (expectedPacket[i] != info.advData[i]) {
//       // Serial.print("Wrong at  "); Serial.println(i);
//       return false;
//     }
//   }
//   return true;
// }
// 
// 
// void report(const char *t) {
//   if (please_dump_info) Serial.println(t);
// }
// 
// 
// void update_button_state(uint8_t new_state) {
//   static uint8_t old_state = 0;
//   if (new_state != old_state) {
//     // DO SOMETHING
//     if        (new_state & frwd_flag) {
//       report("frwd");
//       /* turn_on_pin(frwd_btn_pin); */
//     } else if (new_state & bkwd_flag) {
//       report("bkwd");
//       /* turn_on_pin(bkwd_btn_pin); */
//     } else if (new_state & left_flag) {
//       report("left");
//       /* turn_on_pin(left_btn_pin); */
//     } else if (new_state & rght_flag) {
//       report("rght");
//       /* turn_on_pin(rght_btn_pin); */
//     } else if (new_state & honk_flag) {
//       report("honk");
//       /* turn_on_pin(honk_btn_pin); */
//     } else {
//       report("none");
//       turn_on_pin(off_pin);
//     }
// 
//     // LAST:
//     old_state = new_state;
//   }
// }
// 
// 
// void turn_on_pin(uint8_t pin) {
//   switch(pin) {
//     case frwd_btn_pin:
//       digitalWrite(left_btn_pin, HIGH);
//       digitalWrite(bkwd_btn_pin, HIGH);
//       digitalWrite(frwd_btn_pin, LOW);
//       digitalWrite(rght_btn_pin, HIGH);
//       digitalWrite(honk_btn_pin, HIGH);
//     case bkwd_btn_pin:
//       digitalWrite(left_btn_pin, HIGH);
//       digitalWrite(bkwd_btn_pin, LOW);
//       digitalWrite(frwd_btn_pin, HIGH);
//       digitalWrite(rght_btn_pin, HIGH);
//       digitalWrite(honk_btn_pin, HIGH);
//     case left_btn_pin:
//       digitalWrite(left_btn_pin, LOW);
//       digitalWrite(bkwd_btn_pin, HIGH);
//       digitalWrite(frwd_btn_pin, HIGH);
//       digitalWrite(rght_btn_pin, HIGH);
//       digitalWrite(honk_btn_pin, HIGH);
//     case rght_btn_pin:
//       digitalWrite(left_btn_pin, HIGH);
//       digitalWrite(bkwd_btn_pin, HIGH);
//       digitalWrite(frwd_btn_pin, HIGH);
//       digitalWrite(rght_btn_pin, LOW);
//       digitalWrite(honk_btn_pin, HIGH);
//     case honk_btn_pin:
//       digitalWrite(left_btn_pin, HIGH);
//       digitalWrite(bkwd_btn_pin, HIGH);
//       digitalWrite(frwd_btn_pin, HIGH);
//       digitalWrite(rght_btn_pin, HIGH);
//       digitalWrite(honk_btn_pin, LOW);
//     default:
//       digitalWrite(left_btn_pin, HIGH);
//       digitalWrite(bkwd_btn_pin, HIGH);
//       digitalWrite(frwd_btn_pin, HIGH);
//       digitalWrite(rght_btn_pin, HIGH);
//       digitalWrite(honk_btn_pin, HIGH);
//   }
// }
