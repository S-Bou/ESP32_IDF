#ifndef _AL_CECOSOCKET_H_
#define _AL_CECOSOCKET_H_

#include <stdint.h>

void socket_init();
void socketCallbackFunction(void *the_callback);
void sendMessageToPc(uint8_t *data, int data_len);

#endif /* _AL_SOCKET_H_ */
