#include <stdbool.h>
#include <stdint.h>
#include "hardware/regs/uart.h"
#include "pico/platform.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include "pico/types.h"

void _pwm_init();
void _uart_init();
void _i2c_init();

// PCA9555: 
//   Write:
//     Address 0100AAA0
//     Command
//     Bytes
//   Read:
//     Address Address 0100AAA0
//     Command
//     Reset
//     Address Address 0100AAA1
//     Read bytes

uint8_t btn_read = 0;
absolute_time_t next_uart_write;

int main ()
{
    // Init
    _i2c_init();
    _pwm_init();
    _uart_init();

    next_uart_write = delayed_by_ms(get_absolute_time(), 1000);
    gpio_init(3);
    gpio_set_dir(3, true);
    while(1)
    {
        // Loop
        int ret = i2c_read_blocking(i2c0, 0x20, &btn_read, 1, false);
        if (ret == 1)
        {
            pwm_set_chan_level(0, 0, (btn_read & 0x01) ? 25000 : 0);
            pwm_set_chan_level(1, 0, (btn_read & 0x02) ? 25000 : 0);
            pwm_set_chan_level(2, 0, (btn_read & 0x04) ? 25000 : 0);
            pwm_set_chan_level(3, 0, (btn_read & 0x08) ? 25000 : 0);
            pwm_set_chan_level(4, 0, (btn_read & 0x10) ? 25000 : 0);
            pwm_set_chan_level(5, 0, (btn_read & 0x20) ? 25000 : 0);
            pwm_set_chan_level(6, 0, (btn_read & 0x40) ? 25000 : 0);
            pwm_set_chan_level(7, 0, (btn_read & 0x80) ? 25000 : 0);
        }

        if (uart_is_readable(uart0))
        {
            char c = uart_getc(uart0);
            if (c >= '0' && c <= '9')
            {
                uint16_t value = (c - '0') * 6250;
                pwm_set_chan_level(3, 1, value);
            }
        }
        else if (uart_is_writable(uart0))
        {
            absolute_time_t actual = get_absolute_time();
            if (actual >= next_uart_write)
            {
                gpio_put(3, true);
                next_uart_write = delayed_by_ms(actual, 1000);
                uart_puts(uart0, "OK\n");
            }
            else if (!(uart_get_hw(uart0)->fr & UART_UARTFR_BUSY_BITS))
            {
                gpio_put(3, false);
            }
        }
    }
}

void _pwm_init()
{
    // USING GPIO6-GPIO21
    gpio_set_function(6, GPIO_FUNC_PWM);
    gpio_set_function(7, GPIO_FUNC_PWM);
    gpio_set_function(8, GPIO_FUNC_PWM);
    gpio_set_function(9, GPIO_FUNC_PWM);
    gpio_set_function(10, GPIO_FUNC_PWM);
    gpio_set_function(11, GPIO_FUNC_PWM);
    gpio_set_function(12, GPIO_FUNC_PWM);
    gpio_set_function(13, GPIO_FUNC_PWM);
    gpio_set_function(14, GPIO_FUNC_PWM);
    gpio_set_function(15, GPIO_FUNC_PWM);
    gpio_set_function(16, GPIO_FUNC_PWM);
    gpio_set_function(17, GPIO_FUNC_PWM);
    gpio_set_function(18, GPIO_FUNC_PWM);
    gpio_set_function(19, GPIO_FUNC_PWM);
    gpio_set_function(20, GPIO_FUNC_PWM);
    gpio_set_function(21, GPIO_FUNC_PWM);

    //Frequency 1kHz, Range 0-50000, Precision = 0,002 %
    pwm_set_wrap(0, 49999);
    pwm_set_wrap(1, 49999);
    pwm_set_wrap(2, 49999);
    pwm_set_wrap(3, 49999);
    pwm_set_wrap(4, 49999);
    pwm_set_wrap(5, 49999);
    pwm_set_wrap(6, 49999);
    pwm_set_wrap(7, 49999);
    pwm_set_clkdiv_int_frac(0, 2, 8);
    pwm_set_clkdiv_int_frac(1, 2, 8);
    pwm_set_clkdiv_int_frac(2, 2, 8);
    pwm_set_clkdiv_int_frac(3, 2, 8);
    pwm_set_clkdiv_int_frac(4, 2, 8);
    pwm_set_clkdiv_int_frac(5, 2, 8);
    pwm_set_clkdiv_int_frac(6, 2, 8);
    pwm_set_clkdiv_int_frac(7, 2, 8);

    pwm_set_chan_level(0, 0, 25000);
    pwm_set_chan_level(0, 1, 0);
    pwm_set_chan_level(1, 0, 25000);
    pwm_set_chan_level(1, 1, 0);
    pwm_set_chan_level(2, 0, 25000);
    pwm_set_chan_level(2, 1, 0);
    pwm_set_chan_level(3, 0, 25000);
    pwm_set_chan_level(3, 1, 0);
    pwm_set_chan_level(4, 0, 25000);
    pwm_set_chan_level(4, 1, 0);
    pwm_set_chan_level(5, 0, 25000);
    pwm_set_chan_level(5, 1, 0);
    pwm_set_chan_level(6, 0, 25000);
    pwm_set_chan_level(6, 1, 0);
    pwm_set_chan_level(7, 0, 25000);
    pwm_set_chan_level(7, 1, 0);

    pwm_set_enabled(0, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(5, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(2, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(7, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(4, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(1, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(6, true);
    busy_wait_at_least_cycles(15275);
    pwm_set_enabled(3, true);
}

void _uart_init()
{
    uart_init(uart0, 115200);
    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
    uart_set_translate_crlf(uart0, false);
    uart_set_fifo_enabled(uart0, true);
    uart_set_hw_flow(uart0, false, true);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    //gpio_set_function(3, GPIO_FUNC_UART);
}

void _i2c_init()
{
    i2c_init(i2c0, 400000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    i2c_init(i2c1, 1000000);
    gpio_set_function(26, GPIO_FUNC_I2C);
    gpio_set_function(27, GPIO_FUNC_I2C);
    gpio_pull_up(26);
    gpio_pull_up(27);
}
