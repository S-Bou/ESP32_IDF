#include "dummy/dummyControl.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wirelessCommunication/linkingManager.h"
#include "wirelessCommunication/cecoSocket.h"

#include "uart/uartESP32.h"

#include <unistd.h>

static void printFWInformation();
static char *get_module_software_ver(char *software_ver);
static char *get_module_software_compile_time(char *compile_time);
static uint8_t setUpLinkingProcess();
static const char *TAG = "DUMMY";

char *get_module_software_ver(char *software_ver)
{
	sprintf(software_ver, "V%d.%d.%d", MAJOR_VERSION_NUMBER, MINOR_VERSION_NUMBER, REVISION_NUMBER);
	return software_ver;
}

char *get_module_software_compile_time(char *compile_time)
{
	sprintf(compile_time, "%s %s", __TIME__, __DATE__ );
	return compile_time;
}

void printFWInformation()
{
	char software_ver[15] = {0};
	char compile_time[30] = {0};

	get_module_software_ver(software_ver);
	get_module_software_compile_time(compile_time);	

	sleep(2);
	ESP_LOGI(TAG, "\r\n**************************************************\r\n"\
	"**\tSupreme Speaker Lord\t\t\t**\r\n"\
	"**\tsf ver: [%s] mid ver: [%s]\t**\r\n"\
	"**\tSerial Number: [%s]\t\t**\r\n"\
	"**\tcompile time: [%s]\t**\r\n"\
	"**************************************************\r\n",
	software_ver,
	HARDWARE_MID,
	CONFIG_MQTT_CLIENT_IDENTIFIER,
	compile_time);
}

void initDummyFirmware()
{
	printFWInformation();
	setUpLinkingProcess();
	setSendMessageToTelnet((void*)sendMessageToPc);
	socketCallbackFunction(sendData);
}

static uint8_t setUpLinkingProcess()
{   
    wakeUpSoftAP();
    socket_init();
	return 0;
}