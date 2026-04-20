#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* Sleep time to delay */
#define SLEEP_TIME_MS CONFIG_LED_BLINKING_PERIOD

#if CONFIG_ADVANCE_SETTINGS
    /* Period of pwma 1Khz*/
    #define PWM_PERIOD_NS PWM_KHZ(1)
    /* Fade duration */
    #define FADE_DELAY (CONFIG_LED_BRIGHTNESS ? \
        (CONFIG_LED_FADE_DURATION / CONFIG_LED_BRIGHTNESS) : 0)
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led1)
#define LED_PWM_NODE DT_ALIAS(pwm_led0)
/* Led port habilitation LED0*/
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
/* Pwm port habilitation to LED1*/
static const struct pwm_dt_spec pwmLed = PWM_DT_SPEC_GET(LED_PWM_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    int resultPwm;
    uint32_t pulse;
    bool led_state = true;

    /* Verify if led0 is ready*/
    if (!gpio_is_ready_dt(&led)) return 0;
    /* Led configuration */
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    /* Verify if the pwm port is ready */
    if (!pwm_is_ready_dt(&pwmLed))
    {
        printk("Error: PWM device %s is not ready\n", pwmLed.dev->name);
        return 0;
    }

    while (1) {

        #if !CONFIG_ADVANCE_SETTINGS
            if (gpio_pin_toggle_dt(&led) < 0) return 0;

            led_state = !led_state;
            LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
            k_msleep(SLEEP_TIME_MS);
        #else
            // Fade up
            for (int i = 0; i <= CONFIG_LED_BRIGHTNESS; i++) {
                pulse = (PWM_PERIOD_NS * i) / CONFIG_LED_BRIGHTNESS;

                pwm_set_dt(&pwmLed, PWM_PERIOD_NS, pulse);
                k_msleep(FADE_DELAY);
            }

            // Fade down
            for (int i = CONFIG_LED_BRIGHTNESS; i >= 0; i--) {
                pulse = (PWM_PERIOD_NS * i) / CONFIG_LED_BRIGHTNESS;

                pwm_set_dt(&pwmLed, PWM_PERIOD_NS, pulse);
                k_msleep(FADE_DELAY);
            }
        #endif
    }
    return 0;
}
