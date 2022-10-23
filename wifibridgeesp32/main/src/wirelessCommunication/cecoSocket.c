#include "wirelessCommunication/cecoSocket.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#define STACKSIZE (512 + 768)
#define ADDRESS_MAMBO ((u32_t)0x00000000UL)
#define NO_SOCKET -1

#define SOCKET_THRAD_ON 1
#define SOCKET_THRAD_OFF 0

#define SOCKET_CONNECTION_STACK_SIZE 4096
#define SOCKET_CONNECTION_PRIORITY 7
#define SOCKET_STACK_SIZE 8192
#define SOCKET_PRIORITY 4

static const char *TAG = "SOCKET";

int PORT = 3000;
int new_socket = NO_SOCKET;
uint8_t socket_connection_task_status = SOCKET_THRAD_OFF;
uint8_t socket_connection_task = SOCKET_THRAD_OFF;
struct sockaddr from;

void set_socket_connection(void *param);
void socket_connection_thread(void *param);

void (*send_packet2callback)(uint8_t *, int);

void socket_connection_thread(void *param)
{
	int socket_id = (int)param;
	int packetLen;
	uint8_t packet[64];

	struct sockaddr_in *from_sin = (struct sockaddr_in *)&from;
	socklen_t fromLen = sizeof(from);

	ESP_LOGI(TAG, "Socket connection DONE!");

	while (true)
	{
		if ((packetLen = recvfrom(new_socket, &packet, sizeof(packet), 0, &from, &fromLen)) > 0)
		{
			packetLen = packetLen - 2;
			ESP_LOGI(TAG, "Message from the PC: %d!", packetLen);

			uint8_t *packet2 = NULL;
			uint8_t *ip = (uint8_t *)&from_sin->sin_addr.s_addr;
			uint16_t from_port = ntohs(from_sin->sin_port);
			ESP_LOGI(TAG, "recvfrom - %d bytes from %d.%d.%d.%d:%d\n", packetLen, ip[0], ip[1], ip[2], ip[3], from_port);
			
			packet2 = (uint8_t *)malloc(packetLen * sizeof(uint8_t));
			uint8_t bytes = 0;
			
			char *paquetFromPc = (char *)malloc(packetLen * sizeof(char));
			sprintf(paquetFromPc, "%.*s", packetLen, packet);
			ESP_LOGI(TAG, "paquetFromPc: %s", paquetFromPc);

			char *token = strtok(paquetFromPc, " ");
			packet2[bytes] = token[bytes] - '0';
			bytes++;
			while( token != NULL ) {
				ESP_LOGI(TAG, "token: %s", token);
				ESP_LOGI(TAG, "paquetFromPc: %s", paquetFromPc);
				token = strtok(NULL, " ");
				packet2[bytes] = token[bytes] - '0';
				bytes++;
			}
			free(paquetFromPc);
			free(packet2);
			send_packet2callback(packet2, bytes);
		}
		else
		{
			ESP_LOGI(TAG, "The socket has been disconnected!!");
			new_socket = NO_SOCKET;
			socket_connection_task_status = SOCKET_THRAD_OFF;
			vTaskDelete( NULL );
		}
	}
	socket_connection_task_status = SOCKET_THRAD_OFF;
	vTaskDelete( NULL );
}

void sendMessageToPc(uint8_t *data, int data_len)
{
	if(new_socket != NO_SOCKET)
    {

		data_len = send(new_socket, data, data_len, 0);
		if (data_len < 0)
		{
			ESP_LOGE(TAG, "ERROR: send to broadcast\n");
		}
    }
	else
	{
		ESP_LOGE(TAG, "ERROR: no socket is open\n");
	}
}

void set_socket_connection(void *param)
{
    int softAP_socket = NO_SOCKET;
	int tcp_reuse_timeout = 1;
	struct sockaddr_in softAP_addr;

	softAP_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (softAP_socket == NO_SOCKET)
	{
		ESP_LOGI(TAG, "softAP_socket create error\r\n");
		vTaskDelete( NULL ); /* Delete this task. */
	}

	if (setsockopt(softAP_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&tcp_reuse_timeout, sizeof(tcp_reuse_timeout)) != 0)
	{
		ESP_LOGI(TAG, "set sockopt failed\r\n");
		close(softAP_socket);
		vTaskDelete( NULL ); /* Delete this task. */
	}

	ESP_LOGI(TAG, "the socket has been created!\r\n");

	memset(&softAP_addr, 0, sizeof(softAP_addr));
	softAP_addr.sin_family = AF_INET;
	softAP_addr.sin_port = htons(PORT);
	softAP_addr.sin_addr.s_addr = ADDRESS_MAMBO;

	ESP_LOGI(TAG, "The PORT is: %d [%d]\r\n", softAP_addr.sin_port, PORT);

	if (bind(softAP_socket, (struct sockaddr *)&softAP_addr, sizeof(softAP_addr)) != 0)
	{
		ESP_LOGI(TAG, "softAP bind error\n");

		close(softAP_socket);
		vTaskDelete( NULL ); /* Delete this task. */	
	}

	ESP_LOGI(TAG, "Bind Socket");
	if (listen(softAP_socket, 1) == NO_SOCKET)
	{
		ESP_LOGI(TAG, "Error listen socket");
		vTaskDelete( NULL ); /* Delete this task. */	
	}

	struct sockaddr_in cli_addr;
	socklen_t cli_len;

	while (true)
	{

		if (new_socket == NO_SOCKET)
		{
			new_socket = accept(softAP_socket, (struct sockaddr *)&cli_addr, &cli_len);

			fd_set read_fd_set;
			fd_set error_fd_set;

			FD_ZERO(&read_fd_set);
			FD_SET((unsigned int)new_socket, &read_fd_set);
			FD_ZERO(&error_fd_set);
			FD_SET((unsigned int)new_socket, &error_fd_set);

			int retval;
			retval = select(new_socket + 1, &read_fd_set, NULL, &error_fd_set, NULL);

			if (retval == NO_SOCKET)
			{
				ESP_LOGI(TAG, "Socket Error, close socket %s", strerror(errno));
				close(new_socket);
			}

			ESP_LOGI(TAG, "IP connected, creating thread socket!");
			
			if( socket_connection_task_status == SOCKET_THRAD_OFF)
			{
				socket_connection_task_status = SOCKET_THRAD_ON;
				socket_connection_task = xTaskCreate( socket_connection_thread,
														"socket_connection_thread",
														SOCKET_CONNECTION_STACK_SIZE,
														NULL,
														SOCKET_CONNECTION_PRIORITY,
														NULL);

				if( socket_connection_task != pdPASS){
					socket_connection_task_status = SOCKET_THRAD_OFF;
					ESP_LOGE(TAG, "It has not been posible to create task prvMQTTConnectAndPublishTask");
				}
			}
			else
			{
				ESP_LOGW(TAG, "MQTT_start is already running");
			}

		}
		else
		{
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
	
    ESP_LOGI(TAG, "Kill Socket thread!");
    vTaskDelete( NULL );
}


void socket_init()
{
    ESP_LOGI(TAG, "Init socket at PORT: %d", PORT);
    xTaskCreate(set_socket_connection,
					"socket thread",
					SOCKET_STACK_SIZE,
					NULL,
					SOCKET_PRIORITY,
					NULL);
}

void socketCallbackFunction(void *the_callback)
{
	send_packet2callback = the_callback;
}
