#include "sw_uart.h"
#pragma GCC optimize ("-O3")

void sw_uart_setup(due_sw_uart *uart, int rx, int tx, int stopbits, int databits, int paritybit) {
	uart->pin_tx     = tx;
	uart->pin_rx     = rx;
	uart->stopbits   = stopbits;
	uart->paritybit  = paritybit;
  uart->databits   = databits;
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  digitalWrite(tx, HIGH);
}

void sw_uart_write_data(due_sw_uart *uart, char* bufferData, int writeN) {
  for(int i = 0; i < writeN; i++) {
    sw_uart_write_byte(uart, bufferData[i]);
  }
}

void sw_uart_write_string(due_sw_uart *uart, char* stringData) {
  sw_uart_write_data(uart, stringData, strlen(stringData));
}

int calc_even_parity(char data) {
  char copy= (int)data;
  copy= ((copy>>4)^copy)&15;
  copy= ((copy>>2)^copy)&3;
  copy= ((copy>>1)^copy)&1;
  
  return (int)copy;
}

// recebimento de dados da serial
int sw_uart_receive_byte(due_sw_uart *uart, char* data) {

  // variavel para recebimento de dados
  char nchar  = 0;
  
  // variavel para calculo da paridade
  char parity, rx_parity;
  
  // aguarda start bit
  bool check = false;
  char start = 0;
  char start2 = 0;

  while(!check){
    start = digitalRead(uart->pin_rx);
    _sw_uart_wait_half_T(uart);
    // Confirma start BIT
    if(start == 0){
      start2 = digitalRead(uart->pin_rx);
      _sw_uart_wait_half_T(uart);
      // checa se bit ainda é 0
      if(start2 == 0){
        check = !check;
      }
    }
  }

  // recebe dados
  for (int i = 0, i < uart->databits, i++){
    nchar = digitalRead(uart->pin_rx)<<i;
    _sw_uart_wait_T();
  }
  
  // recebe paridade
  rx_parity = digitalRead(uart->pin_rx);
  _sw_uart_wait_T();

  // recebe stop bit
  char stop = digitalRead(uart->pin_rx);
  _sw_uart_wait_T();

  // checa paridade
  parity = (char)calc_even_parity(nchar); //medo dessa conversao
  if(parity != rx_parity) {
    return SW_UART_ERROR_PARITY;
  }
  
  *data = nchar;
  return SW_UART_SUCCESS;
}

void sw_uart_write_byte(due_sw_uart *uart, char data) {
 
}

// MCK 21MHz
// 1093 para baudrate 9600/2
void _sw_uart_wait_half_T(due_sw_uart *uart) {
  for(int i = 0; i < 1093; i++)
    asm("NOP");
}

void _sw_uart_wait_T(due_sw_uart *uart) {
  _sw_uart_wait_half_T(uart);
  _sw_uart_wait_half_T(uart);
}

