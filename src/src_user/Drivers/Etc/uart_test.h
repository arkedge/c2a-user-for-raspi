/**
* @file   uart_test.h
* @brief  テスト用UARTのDriver
*/
#ifndef UART_TEST_H_
#define UART_TEST_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/Drivers/Super/driver_super.h>

/**
 * @struct UART_TEST_Info
 * @brief
 */
typedef struct
{
  uint8_t flag;
} UART_TEST_Info;

/**
 * @struct  UART_TEST_Driver
 * @brief
 */
typedef struct
{
  struct
  {
    DriverSuper super;                  //!< DriverSuper class
    UART_Config uart_config;           //!< RS422 class
  } driver;
  UART_TEST_Info info;
} UART_TEST_Driver;


// 基本関数

/**
 * @brief  UART_TEST初期化
 *
 *         UART_TEST_Driver構造体のポインタを渡すことでポートを初期化し，UART_TEST_Driverの各メンバも初期化する
 * @param  *uart_test_instance : 初期化するUART_TEST_Driver構造体へのポインタ
 * @param device_file_name: UART_TESTが接続されているデバイスファイル名
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE UART_TEST_init(UART_TEST_Driver* uart_test_instance, char* device_file_name);

/**
 * @brief  UART_TESTのデータ（テレメ）受信
 * @param  *uart_test_instance : UART_TEST_Driver構造体へのポインタ
 * @return DS_REC_ERR_CODE
 */
DS_REC_ERR_CODE UART_TEST_rec(UART_TEST_Driver* uart_test_instance);

/**
 * @brief  UART_TESTへのコマンド送信
 * @param  *uart_test_instance   : UART_TEST_Driver構造体へのポインタ
 * @param  id      : Cmd id
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE UART_TEST_send(UART_TEST_Driver* uart_test_instance, uint8_t id);

#endif
