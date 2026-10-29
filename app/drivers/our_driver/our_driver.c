#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

//If in a .dts file exist a node with compatible = "our,driver"
#define DT_DRV_COMPAT our_driver
//Register a log module
LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

bool led_state = true;

/*
Implement a function in the sensor´s API.
dev -> Zephyr dispositive of our driver.
chan -> Sensor´s channel to read.
val -> Pointer to look the value of the read channel.
*/
static int sensor_channel_get_impl(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    LOG_INF("Hello from channel get, Estatus of led: %s", val->val1 ? "ON" : "OFF");
    //val->val1 = led_state ? 1 : 0;
    //val->val2 = 0;
    val->val1 = gpio_pin_get_dt(&led);
    return 0;
}

static int sensor_sample_fetch_impl(const struct device *dev, enum sensor_channel chan)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(chan);

    LOG_INF("Fetching sample: toggling LED");

    int ret = gpio_pin_toggle_dt(&led);

    if (ret < 0)
    {
        LOG_ERR("Failed to toggle LED: %d", ret);
        return ret;
    }

    // Actualizar el estado lógico solo si el GPIO tuvo éxito.
    led_state = !led_state;

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
    ARG_UNUSED(dev);

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
    led_state = true;

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
DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);