#include "esphome.h"

class UartReadLineSensor : public Component, public UARTDevice, public TextSensor {
    public:
        //TextSensor *uart_ev_mode = new TextSensor();
        //Number *ev_max_charge_current = new Number();
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

                        /* filter out these lines: */
                        if (!strncmp(" ", buffer, 1)) {
                            return 0;
                        }
                        else if (!strncmp("---", buffer, 3)) {
                            return 0;
                        }
                        else if (!strncmp("SW", buffer, 2)) {
                            return 0;
                        }
                        else if (!strncmp("RCMON", buffer, 5)) {
                            return 0;
                        }
                        else if (!strncmp("RFID", buffer, 4)) {
                            return 0;
                        }
                        else if (!strncmp("LOADBL", buffer, 6)) {
                            return 0;
                        }
                        else if (!strncmp("CONFIG", buffer, 6)) {
                            return 0;
                        }
                        else if (!strncmp("WARNING", buffer, 7)) {
                            return 0;
                        }
                        else if (!strncmp("OR GREATER", buffer, 10)) {
                            return 0;
                        }
                        else if (!strncmp("MAINS", buffer, 5)) {
                            return 0;
                        }
                        else if (!strncmp("MAINM", buffer, 5)) {
                            return 0;
                        }
                        else if (!strncmp("MAINAD", buffer, 6)) {
                            return 0;
                        }
                        else if (!strncmp("MAINEM", buffer, 6)) {
                            return 0;
                        }
                        else if (!strncmp("EVEM", buffer, 4)) {
                            return 0;
                        }
                        else if (!strncmp("START", buffer, 5)) {
                            return 0;
                        }
                        else if (!strncmp("STOP", buffer, 4)) {
                            return 0;
                        }
                        else if (!strncmp("IMPORT", buffer, 6)) {
                            return 0;
                        }
                        else if (!strncmp("STATE", buffer, 5)) {
                            return 0;
                        }
                        else if (!strncmp("MIN", buffer, 3)) {
                            return 0;
                        }
                        /* comment out to re-enable logging the current per phase */
                        else if (!strncmp("L1:", buffer, 3)) {
                            return 0;
                        }

                        return rpos;

                        /* only allow these lines: */
                        //if (!strncmp("MODE", buffer, 4)) {
                        //    return rpos;
                        //} else if (!strncmp("STATE", buffer, 5)) {
                        //    return rpos;
                        //} else if (!strncmp("L1:", buffer, 3)) {
                        //    return rpos;
                        //} else if (!strncmp("MAX", buffer, 3)) {
                        //    return rpos;
                        //} else {
                        //    //return 0;
                        //    return rpos;
                        //}

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
                int bytes_read = readline(read(), buffer, max_line_length);
                if (bytes_read > 0) {
                    publish_state(buffer);
                    char * param = &buffer[63];
                    // we can parse and publish to different sensors over here
                    // "MODE    - Set to Normal, Smart or Solar EVSE mode            - "
                    if (bytes_read <= 65) {
                        continue;
                    }

                    if (!strncmp("MODE    -", buffer, 9)) {
                        ESP_LOGD("custom_uart", "Found MODE: %s", param);
                        ev_charge_solar_mode->publish_state(strncmp("Solar", param, 5) == 0);
                        //uart_ev_mode->publish_state(&buffer[63]);
                    }
                    if (!strncmp("MAX     -", buffer, 9)) {
                        ESP_LOGD("custom_uart", "Found MAX current: %s", &buffer[63]);
                        ev_charge_max_current_A->publish_state((float)atoi(&buffer[63]));
                    }
                }
            }
        }
};

