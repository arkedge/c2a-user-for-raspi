#pragma section REPRO
#include <src_core/IfWrapper/uart.h>
#include "../../Settings/port_config.h"

#ifdef USE_SCI_COM_UART
#include "uart_sils_sci_if.hpp"
#endif

int OBC_C2A_SendFromObc(int port_id, unsigned char* buffer, int offset, int count);
int OBC_C2A_ReceivedByObc(int port_id, unsigned char* buffer, int offset, int count);

int UART_init(void* my_uart_v)
{
  return 0;
}

int UART_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

  return OBC_C2A_ReceivedByObc(my_uart->ch, (unsigned char*)data_v, 0, buffer_size);
}

int UART_tx(void* my_uart_v, void* data_v, int data_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

  if (OBC_C2A_SendFromObc(my_uart->ch, (unsigned char*)data_v, 0, data_size) < 0)
  {
    return -1;
  }

  return 0;
}

int UART_reopen(void* my_uart_v, int reason)
{
  return 0;
}

#pragma section
