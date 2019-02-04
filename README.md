Contains all my ESP8266 projects. Most of them are essentially MQTT publish bridges, meaning they hook up to some kind of input which they monitor and upon change, publish the topic with a numerical/boolean value. Not too complicated.
Has logic to deal with loss of WiFi and/or loss of MQTT connectivity. In most cases, no effort is made to record changes to input during reconnect phase (except when using interrupts which trigger anyway).

# alexor_mqtt_bridge

Uses the two GPIOs in the DSC Alexor panel to indicate if alarm is armed or not as well as if it has triggered the alarm or not. Using these two inputs, MQTT topics are published on state changes. Does not use interrupts since alarm GPIOs aren't expected to change a lot.

# rg11_mqtt_bridge

Hooks up to a RG-11 optical rainsensor and publishes a value indicating how many times (1 or more) that the virtual bucket has triggered. This allows you to easily monitor rain and since it uses interrupts, it should not miss any rain event.

