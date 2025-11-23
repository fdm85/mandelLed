/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "uart_tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 1024;

#define UART_TASK_STACK_SIZE 2048

static void init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200uL,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_mode(UART_NUM_2, UART_MODE_UART));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 47, 48, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

static int sendData(const char* logName, const char* data)
{
    const size_t len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
    ESP_LOGV(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_WARN);
    while (1) {
        sendData(TX_TASK_TAG, "g 0 r");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_DEBUG);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_2, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_VERBOSE);
        }
    }
    free(data);
}

esp_err_t uart_main(void)
{
    init();
    BaseType_t RxInitSucc = xTaskCreate(rx_task, "uart_rx_task", UART_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
    if(!RxInitSucc)
      ESP_LOGE("uart_rx", "Create uart_rx task failed");
    BaseType_t TxInitSucc = xTaskCreate(tx_task, "uart_tx_task", UART_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL);
    if(!TxInitSucc)
      ESP_LOGE("uart_tx", "Create uart_tx task failed");
	return (RxInitSucc && TxInitSucc) ? ESP_OK : ESP_FAIL;
}
