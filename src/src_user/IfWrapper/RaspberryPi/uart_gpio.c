int GPIO_set_direction(const int port, const GPIO_DIRECTION direction)
{
  return GPIO_CHIPKIT_set_direction(port, direction);
}

int GPIO_set_output(const int port, const GPIO_HL direction)
{
  return GPIO_CHIPKIT_set_output(port, direction);
}

int GPIO_read_input(const int port)
{
  return GPIO_CHIPKIT_read_input(port);
}
