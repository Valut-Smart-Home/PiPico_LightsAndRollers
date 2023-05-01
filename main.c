#include "hardware/timer.h"
#include "pico/platform.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdint.h>

void _pwm_init();

int main ()
{
    // Init
    stdio_init_all();
    _pwm_init();

    while(1)
    {
        // Loop
        
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
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(5, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(2, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(7, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(4, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(1, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(6, true);
    busy_wait_at_least_cycles(15300);
    pwm_set_enabled(3, true);
}