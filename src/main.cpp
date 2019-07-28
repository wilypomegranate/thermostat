#include "Esp8266.hpp"
#include "Register.hpp"

extern "C" {
#include <util/delay.h>
}

using namespace microcpp;

void handle_error(bool ret) {
  if (!ret) {
    // Set PB5 which is port 13 and led L.
    reg::set_bits(DDRB, DDB5);

    // Now just block until reset.
    while (true) {
      _delay_ms(1000);
    }
  }
}

int main() {

  wifi::Esp8266 wifi(wifi::Esp8266::DEFAULT_BAUD_RATE);

  if (!wifi.connected_ap()) {
    handle_error(wifi.quit_ap());
    _delay_ms(1000);

    handle_error(wifi.join_ap());
    _delay_ms(1000);
  }

  return 0;
}
