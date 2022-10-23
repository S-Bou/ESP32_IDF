#ifndef _DUMMY_CONTROL_H_
#define _DUMMY_CONTROL_H_

#include <stdint.h>

#define		HARDWARE_MID				"ESP32"
#define		MAJOR_VERSION_NUMBER		0
#define		MINOR_VERSION_NUMBER		0
#define		REVISION_NUMBER				2

/*
 * configuration data
 */
typedef struct {
    uint32_t count;
    uint32_t tmo_threshold; 
} demo_config_t;


/*
 * session context data
 */
typedef struct {
    uint32_t count;
    uint32_t timeout_count; 
} demo_context_t;

void initDummyFirmware();

#endif /*#ifndef _DUMMY_CONTROL_H_ */
