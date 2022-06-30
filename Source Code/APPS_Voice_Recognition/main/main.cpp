/**
 * @file main.cpp
 * @author Jan Sevrin
 * @brief Main file of APPS project.
 * @version 0.1
 * @date 2022-04-25
 * 
 * @copyright Fully open-source
 * 
 */

//ESP-IDF library
#include "esp_err.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Custom includes
#include "BLE.hpp"
#include "main_tasks.hpp"

enum {CORE_PROGRAM = 0, CORE_APP = 1};

extern "C" {
	void app_main(void)
	{
		esp_err_t err = nvs_flash_init();
		if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			err = nvs_flash_init();
		}
		ESP_ERROR_CHECK(err);

		setup_ble();
		xTaskCreatePinnedToCore(task_process_audio, "process audio", 2048, NULL, 1, NULL, CORE_PROGRAM);
		xTaskCreatePinnedToCore(task_read_audio, "read audio", 2048, NULL, 1, NULL, CORE_APP);
	}
}


