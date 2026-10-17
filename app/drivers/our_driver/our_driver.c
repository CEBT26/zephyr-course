#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

//If in a .dts file exist a node with compatible = "our,driver"
#define DT_DRV_COMPAT our_driver
//Register a log module
LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/*
Implement a function in the sensor´s API.
dev -> Zephyr dispositive of our driver.
chan -> Sensor´s channel to read.
val -> Pointer to look the value of the read channel.
*/
static int channel_get_my_impl(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    LOG_INF("Hello from channel get, channel %d", chan);
    return 0;
}

//Conection with the generic sensor´s API
static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = channel_get_my_impl, //When call the API operation develop the function.

};

/* 
Init function.
Develop this function when Zephyr develop this register 
*/
static int init(const struct device *dev)
{
    LOG_INF("Device initialized");

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