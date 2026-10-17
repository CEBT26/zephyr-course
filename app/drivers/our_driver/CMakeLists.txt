#If CONFIG_OUR_DRIVER is enabled in prj.conf, take the .c file in the compilation.
if(CONFIG_OUR_DRIVER)
    target_sources(app PRIVATE our_driver.c)
endif()
