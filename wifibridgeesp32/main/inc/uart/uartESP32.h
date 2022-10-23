#ifndef _UART_ESP32_H_
#define _UART_ESP32_H_

void uartInit();

int sendData(uint8_t *data, int sizeOfData);
void setSendMessageToTelnet(void * the_callback);

#endif /* _UART_ESP32_H_ */