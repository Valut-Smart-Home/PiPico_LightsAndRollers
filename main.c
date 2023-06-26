#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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

uint8_t _ram_adr_pwm_states[2] = {0x00, 0x00};              // 16 * 2 bytes = 32 -> 0x00..0x1F
uint8_t _ram_adr_device_configuration[2] = {0x00, 0x20};    // 8 bytes + 88 bytes reserved = 96 -> 0x20..0x7F
uint8_t _ram_adr_button_config[2] = {0x00, 0x80};           // 64 * (108 bytes + 6 bytes reserved) = 7296 -> 0x80..0x1CFF
uint8_t _ram_adr_output_config[2] = {0x00, 0x80};           // 32 * (16 bytes + 8 bytes reserved) = 768 -> 0x1D00..0x1FFF

#define _button_config_ram_bytes 114
#define _output_config_ram_bytes 24

struct device_configuration _config;
struct button_configuration _buttons[64];
struct virtual_output_configuration _external_outputs[32];

uint16_t _pwm_state[16];

void _pwm_init();
void _uart_init();
void _i2c_init();

void _load_device_config();
void _load_pwm_states();
void _load_keyboard_config();
void _load_output_config();

void _set_default_config();
//void _set_pwms();

absolute_time_t next_uart_write;

int main ()
{
    char __foo[] = {[sizeof(struct button_configuration)] = 'a'};
    // Init
    _i2c_init();
    _pwm_init();
    _uart_init();

    _load_device_config();
    if (_config.features & device_feature_fram)
    {
        _load_pwm_states();
        _load_keyboard_config();
    }

    next_uart_write = delayed_by_ms(get_absolute_time(), 1000);
    
    uint8_t buttons_raw[6];
    while(1)
    {
        // Loop
        i2c_read_blocking(i2c0, 0x20, buttons_raw, 6, false);

        if (uart_is_writable(uart0))
        {
            absolute_time_t actual = get_absolute_time();
            if (to_us_since_boot(actual) >= to_us_since_boot(next_uart_write))
            {
                gpio_put(3, true); // 3us to switch
                next_uart_write = delayed_by_ms(actual, 1000);
                char print_buff[100];
                sprintf(print_buff, "%02X %02X %02X %02X %02X %02X\n", buttons_raw[0], buttons_raw[1], buttons_raw[2], buttons_raw[3], buttons_raw[4], buttons_raw[5]);
                uart_puts(uart0, print_buff);
            }
            else if (!(uart_get_hw(uart0)->fr & UART_UARTFR_BUSY_BITS))
            {
                //3us to switch = 375 cycles
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

    pwm_set_chan_level(0, 0, 0);
    pwm_set_chan_level(0, 1, 0);
    pwm_set_chan_level(1, 0, 0);
    pwm_set_chan_level(1, 1, 0);
    pwm_set_chan_level(2, 0, 0);
    pwm_set_chan_level(2, 1, 0);
    pwm_set_chan_level(3, 0, 0);
    pwm_set_chan_level(3, 1, 0);
    pwm_set_chan_level(4, 0, 0);
    pwm_set_chan_level(4, 1, 0);
    pwm_set_chan_level(5, 0, 0);
    pwm_set_chan_level(5, 1, 0);
    pwm_set_chan_level(6, 0, 0);
    pwm_set_chan_level(6, 1, 0);
    pwm_set_chan_level(7, 0, 0);
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
    gpio_init(3);
    gpio_set_dir(3, true);
}

void _i2c_init()
{
    i2c_init(i2c0, 100000);
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

void _load_pwm_states()
{
    uint16_t states[16];
    int write = i2c_write_blocking_until(i2c1, 0x50, _ram_adr_pwm_states, 2, true, make_timeout_time_ms(2));
    if (write != 2) return;

    int read = i2c_read_blocking_until(i2c1, 0x50, (uint8_t*)states, 32, false, make_timeout_time_ms(2));
    if (read != 32) return;

    for (int i = 0; i < 16; i++)
    {
        _pwm_state[i] = states[i];
        pwm_set_chan_level(i >> 1, i % 2, states[i]);
    }
}

void _load_device_config()
{
    struct device_configuration readed_config;
    int write = i2c_write_blocking_until(i2c1, 0x50, _ram_adr_device_configuration, 2, true, make_timeout_time_ms(2));
    if (write != 2)
    {
        _set_default_config();
        return;
    }

    int read = i2c_read_blocking_until(i2c1, 0x50, (uint8_t*)(&readed_config), sizeof(readed_config), false, make_timeout_time_ms(2));
    if (read != sizeof(readed_config))
    {
        _set_default_config();
        return;
    }

    readed_config.features |= device_feature_fram;
    memcpy(&_config, &readed_config, sizeof(_config));
}

void _set_default_config()
{
    _config.slaveId = 127;
    _config.features = 0;
}

void _load_keyboard_config()
{

}

void _load_output_config()
{

}