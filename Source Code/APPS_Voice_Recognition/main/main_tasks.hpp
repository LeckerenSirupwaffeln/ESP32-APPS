/**
 * @file main_tasks.hpp
 * @author Jan Sevrin
 * @brief Contains the RTOS tasks used to process speech quality.
 * @version 0.1
 * @date 2022-06-25
 * 
 * @copyright Fully open-source
 * 
 */


/**
 * @brief Constantly read audio data.
 * 
 * @param params N/A.
 */
void task_read_audio(void* params);

/**
 * @brief Constantly process the most recent audio data.
 * 
 * @param params N/A.
 */
void task_process_audio(void* params);
