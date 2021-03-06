#include "sw_uart.h"

due_sw_uart uart;

void setup() {
  sw_uart_setup(&uart, 19, 18, 1, 8, SW_UART_EVEN_PARITY);
  Serial.begin(115200);
  //Serial.begin(9600);
}

void loop() {
 test_receive();
}

void test_receive() {
  char data;
  int code = sw_uart_receive_byte(&uart, &data);
  if(code == SW_UART_SUCCESS) {
     Serial.print(data);
  } else if(code == SW_UART_ERROR_PARITY) {
    Serial.println("\nPARITY ERROR");
  } else {
    Serial.println("OTHER");
    Serial.print(code);
  }
}

