#include <l4/drivers/uart_pl011.h>
#include "support.h"
#include "startup.h"

namespace {
class Platform_hikey960 : public Platform_single_region_ram
{
  bool probe() { return true; }

  void init()
  {
    // set defaults for reg_shift and baud_rate
    kuart.baud      = 115200;
    kuart.reg_shift = 0;

    kuart.base_address = 0xFFF32000;
    kuart.base_baud    = 0;
    kuart.irqno        = 111;
    kuart.access_type  = L4_kernel_options::Uart_type_mmio;
    kuart_flags       |=   L4_kernel_options::F_uart_base
                         | L4_kernel_options::F_uart_baud
                         | L4_kernel_options::F_uart_irq;

    static L4::Io_register_block_mmio r(kuart.base_address);
    static L4::Uart_pl011 _uart(kuart.base_baud);
    _uart.startup(&r);
    set_stdio_uart(&_uart);
  }

  void reboot()
  {
    reboot_psci();
  }
};
}

REGISTER_PLATFORM(Platform_hikey960);
