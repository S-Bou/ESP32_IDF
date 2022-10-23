#ifndef _LINKING_MANAGER_H_
#define _LINKING_MANAGER_H_

#include <stdint.h>

#define PROVISION_PROCESS_READY                 0
#define PROVISION_PROCESS_LINKING               1
#define PROVISION_PROCESS_CONNECTING_TO_AWS     2

extern int8_t provisionStatus;

void wakeUpSoftAP();

#endif /* _LINKING_MANAGER_H_ */
