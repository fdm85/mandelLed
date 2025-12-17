/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "uart_tasks.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/_intsup.h>
#include "BridgeParser.h"

typedef struct lSGen_t
{
  uint32_t tInt;
  uint32_t bMult;
  uint32_t bDiv;
  uint32_t lStart;
  uint32_t lEnd;
}LSGen;



typedef struct ringBuff_t
{
  char *pl; /**< pointer to start of payload buffer */
  const uint16_t size; /**< total count of max payload possible */
  uint16_t r; /**< current read pointer */
  uint16_t w; /**< current write pointer */
  uint16_t pad; /**< padding */
}RingBuff;

static const int RX_BUF_SIZE = 128;
static const char *RX_TASK_TAG = "RX_TASK";
static const char *TX_TASK_TAG = "TX_TASK";
static const TickType_t t2w = 2000 / portTICK_PERIOD_MS;
#define UART_TASK_STACK_SIZE (2 * 2048)

static void init(void) {
	const uart_config_t uart_config = {
		.baud_rate = 115200uL,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	// We won't use a buffer for sending data.
	ESP_ERROR_CHECK(
		uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2u, 0, 0, NULL, 0));
	ESP_ERROR_CHECK(uart_set_mode(UART_NUM_2, UART_MODE_UART));
	ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 47, 48, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

static int sendData(const char *logName, const char *data) {
	const size_t len = strlen(data);
	const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
	ESP_LOGV(logName, "Wrote %d bytes", txBytes);
	return txBytes;
}

static void tx_task(void *arg) {
	while (1) {
		sendData(TX_TASK_TAG, "?0\n");
		vTaskDelay(t2w);
	}
}


static void rx_task(void *arg) {
	static pb_Ctx pb;
  int inc;
  uint32_t tmp;
  pb_ParserState parRes = pb_eIdle;
  bp_Init(&pb, 10u,RX_BUF_SIZE);
  
	while (1) {
    inc = uart_read_bytes(UART_NUM_2, &pb.pl[pb.wr], 1, t2w);

    if(inc)
      bp_Fill(&pb, inc);
      
    parRes = bp_Parse(&pb);
    
    if(parRes == pb_eRxNum){
      tmp = pb_Convert(&pb);
      ESP_LOGW("uart_rx", "rx: %lu", pb.wr - pb.rd, pb.pl, tmp);
      bp_Reset(&pb);
    }
      
	}
}

esp_err_t uart_main(void) {
	init();
	BaseType_t RxInitSucc =
		xTaskCreate(rx_task, "uart_rx_task", UART_TASK_STACK_SIZE, NULL,
					configMAX_PRIORITIES - 1, NULL);
	if (!RxInitSucc)
		ESP_LOGE("uart_rx", "Create uart_rx task failed");
	BaseType_t TxInitSucc =
		xTaskCreate(tx_task, "uart_tx_task", UART_TASK_STACK_SIZE, NULL,
					configMAX_PRIORITIES - 2, NULL);
	if (!TxInitSucc)
		ESP_LOGE("uart_tx", "Create uart_tx task failed");
  
	return (RxInitSucc && TxInitSucc) ? ESP_OK : ESP_FAIL;
}
