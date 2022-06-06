#include "esphome.h"

class UartReadLineSensor : public Component, public UARTDevice, public TextSensor {
 public:
  UartReadLineSensor(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    // nothing to do here
  }

  int readline(int readch, char *buffer, int len)
  {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case '\n': // Return on LF
          /* fall-through */
        case '\r': // Return on CR
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          //ESP_LOGD("custom_uart", "Found EOL @ %d", rpos);

          /* filter out lines starting w/ '>' */
          if (buffer[0] == '>') {
              rpos = 0;
          }

          /* only allow these lines: */
          if (!strncmp("MODE", buffer, 4)) {
              return rpos;
          } else if (!strncmp("STATE", buffer, 5)) {
              return rpos;
          } else if (!strncmp("L1:", buffer, 3)) {
              return rpos;
          } else {
              return 0;
          }

        default:
          if (pos < len-1) {
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
  }

  void loop() override {
    const int max_line_length = 512;
    static char buffer[max_line_length];
    while (available()) {
      if(readline(read(), buffer, max_line_length) > 0) {
        publish_state(buffer);
      }
    }
  }
};
