#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "our_driver.h"

//If in a .dts file exist a node with compatible = "our,driver"
#define DT_DRV_COMPAT our_driver
//Register a log module
LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static struct our_driver_data driver_data;

bool led_state = true;

/*
Implement a function in the sensor´s API.
dev -> Zephyr dispositive of our driver.
chan -> Sensor´s channel to read.
val -> Pointer to look the value of the read channel.
*/
static int sensor_channel_get_impl(const struct device *dev,
                                   enum sensor_channel chan,
                                   struct sensor_value *val)
{
    ARG_UNUSED(chan);

    struct our_driver_data *data = dev->data;

    LOG_INF("LED Status: %s", data->led_state ? "ON" : "OFF");

    val->val1 = data->led_state ? 1 : 0;
    val->val2 = 0;

    return 0;
}

static int sensor_sample_fetch_impl(const struct device *dev, enum sensor_channel chan)
{
    ARG_UNUSED(chan);

    struct our_driver_data *data = dev->data;

    LOG_INF("Fetching sample: toggling LED");

    int ret = gpio_pin_toggle_dt(&led);

    if (ret < 0)
    {
        LOG_ERR("Failed to toggle LED: %d", ret);
        return ret;
    }

    // Actualizar el estado lógico solo si el GPIO tuvo éxito.
    data->led_state = !data->led_state;

    return 0;
}

int our_driver_set_state(const struct device *dev, bool state)
{
    struct our_driver_data *data = dev->data;

    LOG_INF("Driver Extension API: Setting LED %s",
            state ? "ON" : "OFF");

    int ret = gpio_pin_set_dt(&led, state);

    if (ret < 0) {
        LOG_ERR("Failed to set LED state: %d", ret);
        return ret;
    }

    data->led_state = state;

    return 0;
}

//Conection with the generic sensor´s API
static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = sensor_channel_get_impl, //When call the API operation develop the function.
    .sample_fetch = sensor_sample_fetch_impl,
};

/* 
Init function.
Develop this function when Zephyr develop this register 
*/
static int init(const struct device *dev)
{
    struct our_driver_data *data = dev->data;

    LOG_INF("Initializing LED driver");

    if (!gpio_is_ready_dt(&led))
    {
        LOG_ERR("LED GPIO device is not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    if (ret < 0)
    {
        LOG_ERR("Failed to configure LED GPIO: %d", ret);
        return ret;
    }

    // GPIO_OUTPUT_ACTIVE inicializa el LED en su estado lógico activo.
    data->led_state = true;

    LOG_INF("LED driver initialized successfully");

    return 0;
}

/*
Intance the driver to Zephyr.
0 -> Índice de la instancia del dispositivo.
init -> Función de inicialización.
First NULL -> No se utiliza una función de inicialización diferente.
Second NULL -> No se proporciona memoria para datos privados del dispositivo.
Third NULL -> No se proporciona una configuración constante del dispositivo.
POST_KERNEL -> Nivel de inicialización posterior a la inicialización del kernel.
80 -> Prioridad de inicialización. Un número menor representa mayor prioridad dentro del mismo nivel.
&api_iomico_lecture -> Puntero a la tabla de funciones de la API. 
*/
DEVICE_DT_INST_DEFINE(0, init, NULL,  &driver_data, NULL, POST_KERNEL, 80, &api_iomico_lecture);