#include <src_core/IfWrapper/uart.h>
#include <src_core/Library/print.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "../../Settings/port_config.h"

int OBC_C2A_SendFromObc(int port_id, unsigned char* buffer, int offset, int count);
int OBC_C2A_ReceivedByObc(int port_id, unsigned char* buffer, int offset, int count);

static int uart_fd_ = -1;
static struct termios uart_tty_;

static UART_ERR_CODE UART_validate_config_(const UART_Config* my_uart)
{
  int i = 0;
  uint32_t available_baudrate[] = {4800, 9600, 19200, 115200};

  if (my_uart->parity_settings > PARITY_SETTINGS_SPACE) return UART_PARITY_ERR;
  if (my_uart->data_length > UART_DATA_LENGTH_8BIT) return UART_DATA_NEGA_ERR;
  if (my_uart->stop_bit > UART_STOP_BIT_2BIT) return UART_STOP_BIT_ERR;

  for (i = 0; i < sizeof(available_baudrate) / sizeof(available_baudrate[0]); ++i)
  {
    if (my_uart->baudrate == available_baudrate[i]) return UART_OK; // 正常
  }

  return UART_BAUDRATE_ERR;
}

static speed_t UART_get_speed_param_from_config_(const UART_Config* my_uart)
{
  int i = 0;
  uint32_t available_baudrate[] = {4800, 9600, 19200, 115200};
  speed_t baurate_params[] = {B4800, B9600, B19200, B115200}; 
  for (i = 0; i < sizeof(available_baudrate) / sizeof(available_baudrate[0]); ++i)
  {
    if (my_uart->baudrate == available_baudrate[i]) return baurate_params[i]; // baurateを返却
  }
}

static tcflag_t UART_make_termios_param_from_config_(const UART_Config* my_uart, tcflag_t c_cflag)
{
  switch(my_uart->parity_settings)
  {
  case PARITY_SETTINGS_NONE:
    c_cflag &= ~PARENB;
    break;
  case PARITY_SETTINGS_ODD:
    c_cflag |= PARENB;
    c_cflag |= PARODD;
    break;
  case PARITY_SETTINGS_EVEN:
    c_cflag |= PARENB;
    c_cflag &= ~PARODD;
    break;
  case PARITY_SETTINGS_MARK:
    c_cflag |= (PARENB | CMSPAR);
    c_cflag |= PARODD;
    break;
  case PARITY_SETTINGS_SPACE:
    c_cflag |= (PARENB | CMSPAR);
    c_cflag &= PARODD;
    break;
  default:
    c_cflag &= ~PARENB;
  }

  switch(my_uart->data_length)
  {
  case UART_DATA_LENGTH_7BIT:
    c_cflag |= CS7;
    break;
  case UART_DATA_LENGTH_8BIT:
    c_cflag |= CS8;
    break;
  default:
    c_cflag |= CS8;
  }

  switch(my_uart->stop_bit)
  {
  case UART_STOP_BIT_1BIT:
    c_cflag &= ~CSTOPB;
  case UART_STOP_BIT_2BIT:
    c_cflag |= CSTOPB; 
  default:
    c_cflag &= ~CSTOPB;
  }

  return c_cflag;
}

int UART_init(void* my_uart_v)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;
  int ret = (int)UART_validate_config_(my_uart);
  if (ret != (int)UART_OK) return ret;

  uart_fd_ = open(my_uart->device_file_name, O_RDWR);

  if (uart_fd_ < 0)
  {
    Printf("Error: Serial port %s can't be opend\n", my_uart->device_file_name);
    return uart_fd_;
  }

  tcgetattr(uart_fd_, &uart_tty_);

  cfmakeraw(&uart_tty_);
  uart_tty_.c_cc[VMIN] = 0;
  uart_tty_.c_cc[VTIME] = 0;

  uart_tty_.c_cflag = UART_make_termios_param_from_config_(my_uart, uart_tty_.c_cflag);

  cfsetospeed(&uart_tty_, UART_get_speed_param_from_config_(my_uart));
  cfsetispeed(&uart_tty_, UART_get_speed_param_from_config_(my_uart));

  tcflush(uart_fd_, TCIFLUSH);
  tcsetattr(uart_fd_, TCSANOW, &uart_tty_);

  return 0;
}

int UART_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  unsigned char* data = (unsigned char*) data_v;
  unsigned char buf[255];
  int data_length;
  int data_length_sum = 0;
  int i;

  if(uart_fd_ != 1)
  {
    do
    {
      data_length = read(uart_fd_, buf, sizeof(buf));
      for(i = 0; i < data_length; i++)
      {
        data[data_length_sum] = buf[i];
        data_length_sum++;
      }
    }
    while(data_length > 0);
  }

  return data_length_sum;
}

int UART_tx(void* my_uart_v, void* data_v, int data_size)
{
  if(uart_fd_ != 1) {
    int count = write(uart_fd_, (unsigned char*)data_v, data_size);
    if(count < 0) {
      return count;
    }
  }

  return 0;
}

static int UART_reset_port_(const void* my_uart_v, int reason)
{
  int ret;
  // TODO: EL_record_eventでreasonを記録する
  close(uart_fd_);
  ret = UART_init(my_uart_v);

  return ret;
}

int UART_reopen(void* my_uart_v, int reason)
{
  return UART_reset_port_(my_uart_v, reason);
}
