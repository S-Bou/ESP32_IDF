 #include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include "esp_log.h"
#include "freertos/stream_buffer.h"
#include "uart/uartESP32.h"


#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)


uint32_t numberOfMessages = 0;

void (*sendMessageToTelnet)( uint8_t *, int ) ;

int sendData(uint8_t *data, int sizeOfData)
{
    int txBytes = 0;

    ESP_LOGI("UART", "Message to UART!!");
	uint8_t showMessage = 0;
	for( showMessage = 0; showMessage < sizeOfData; showMessage++)
	{
		ESP_LOGI("UART", "%d", data[showMessage]);
	}

    txBytes = uart_write_bytes(UART_NUM_1, data, sizeOfData);
    ESP_LOG_BUFFER_HEXDUMP("Recived mensage", data, sizeOfData, ESP_LOG_INFO);
    ESP_LOGI("Sending uart message", "Wrote %d bytes", txBytes);
    return txBytes;
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);

    int32_t rxBytes = 0;
    uint8_t bufferToSend[128]= {0};
    int messegeLenght = 0;
    int readlen=0;
    uint8_t header=0;
    int i = 0;
    uint8_t rxBuffer[1] = {0};
    uint8_t dataToRead = 1;

    while(1){  
		
        while(1)
        {
            if(i > 3 && (readlen > messegeLenght))
            {
                header = 0;
                break;
            }
            
            rxBytes = uart_read_bytes(UART_NUM_1, rxBuffer, 1,  portMAX_DELAY);
            if(rxBytes < 0)
            {
                printf("Recive data fail\n");
            }
            
            if(rxBuffer[0] == 0xFA)
            {
                header = 1;
            }
            
            if(header==1)
            {
                bufferToSend[i] = rxBuffer[0];
                
                if(i == 2)
                {
                    messegeLenght = bufferToSend[2];
                    readlen = 0;
                }
                
                i++;
            }
            readlen += 1;
        }
   
        // rc = uart_read_bytes(UART_NUM_1, recvbuffer, dataToRead,  10 / portTICK_RATE_MS);

        // if(recvbuffer[0] == 0xFA)
        // {
        //     header = 1;
        //     data_buff[0] = recvbuffer[0];
        // }

        // if(recvbuffer[0] == 0xFB && header == 1)
        // {
        //     data_buff[1] = recvbuffer[0];
        //     uart_read_bytes(UART_NUM_1, recvbuffer, dataToRead,  10 / portTICK_RATE_MS);
        //     data_buff[2] = recvbuffer[0];
        //     dataToRead = recvbuffer[0];
        // }
        // if(dataToRead > 1 recvbuffer[0] == 0xFB && header = 1)
        // {

        // }
   
        readlen += 3;

        ///ESP_LOGI("RX_TASK", "UART message number [%d]", numberOfMessages);
        //ESP_LOG_BUFFER_HEXDUMP("Stream Buffer output data", data_buff, data_len, ESP_LOG_INFO);

        numberOfMessages++;
        sendMessageToTelnet(bufferToSend, readlen);
        memset(rxBuffer, 0, sizeof(rxBuffer));
        i = 0; 
        messegeLenght = 0;
        readlen = 0; 
        header=0;
   }
    vTaskDelete( NULL );
}

void uartInit()
{
    const uart_config_t uart_config = 
    {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_1, 256, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
 
    xTaskCreate(rx_task, "uart_rx_task", 4096, NULL, 6, NULL);
    ESP_LOGI("RX_TASK", "UART 1 rx task created");
}

void setSendMessageToTelnet(void * the_callback)
{
	sendMessageToTelnet = the_callback;
}