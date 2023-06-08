#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "hardware/regs/uart.h"
#include "pico/platform.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include "pico/types.h"
#include "configuration.h"
#include "pca95555.h"

void _pwm_init();
void _uart_init();
void _i2c_init();
void ensure_pca95555_output_zeros();

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

Pca95555 inputsExpander;
absolute_time_t next_uart_write;

int main ()
{
    char __foo[] = {[sizeof(struct button_configuration)] = 'a'};
    // Init
    _i2c_init();
    _pwm_init();
    _uart_init();

    Pca95555_Init(&inputsExpander, i2c0, false, false, false);

    next_uart_write = delayed_by_ms(get_absolute_time(), 1000);
    gpio_init(3);
    gpio_set_dir(3, true);
    uint8_t row = 8;
    uint8_t buttons[8];
    while(1)
    {
        // Loop
        if (row > 7)
        {
            row = 0;
            ensure_pca95555_output_zeros();
        }
        Pca95555_SetAllInputsOneOutput(&inputsExpander, row);
        busy_wait_at_least_cycles(Pca95555_SetUpTimeNs / 8);
        buttons[row] = Pca95555_ReadI1(&inputsExpander);

        if (uart_is_writable(uart0))
        {
            absolute_time_t actual = get_absolute_time();
            if (to_us_since_boot(actual) >= to_us_since_boot(next_uart_write))
            {
                gpio_put(3, true); // 3us to switch
                next_uart_write = delayed_by_ms(actual, 1000);
                char print_buff[100];
                sprintf(print_buff, "%02X %02X %02X %02X %02X %02X %02X %02X\n", buttons[0], buttons[1], buttons[2], buttons[3], buttons[4], buttons[5], buttons[6], buttons[7]);
                uart_puts(uart0, print_buff);
            }
            else if (!(uart_get_hw(uart0)->fr & UART_UARTFR_BUSY_BITS))
            {
                //3us to switch = 375 cycles
                gpio_put(3, false);
            }
        }
        row++;
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

void ensure_pca95555_output_zeros()
{
    uint8_t buff[2];
    Pca95555_ReadOutputRegister(&inputsExpander, buff);
    if (buff[0] != 0 || buff[1] != 0)
    {
        Pca95555_SetOutputRegister(&inputsExpander, 0, 0);
    }
}
