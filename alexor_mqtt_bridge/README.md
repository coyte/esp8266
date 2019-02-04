# MQTT Bridge for the DSC Alexor wireless alarm system

This turns the ESP8266 into a MQTT Client which listens to a 
[DSC Alexor panel](https://www.dsc.com/alarm-security-products/PC9155%20-%20Alexor%202-Way%20Wireless%20Panel/53). 
The Alexor is able to signal using 2 GPIOs on the panel circuitboard. The events
armed/unarmed and triggered/safe can be monitored using this.

The MQTT bridge will report changes on these two GPIOs to a MQTT broker as
published topics.

# Software install

Change the `ssid` field to point to your wifi, likewise you need to change
`password` to match.

You also want to point out the MQTT broker by changing `mqtt_server`.

If you want, you can also choose what the topics shall be used.

Once done, flash the ESP8266 with the software and hook it up

# Hardware install

Configure the DSC Alexor panel to act as NO relays on the two GPIOs (typically
referred to PGM in the manual). Once configured, they will report the states
mentioned above.

Connect the PGMs to Pin 13 and Pin 12 and make sure the GND output from the
alexor is connected to Ground on the ESP8266, or the ESP will not be able
to detect the difference between HIGH/LOW states.

# Running

Once all is connected and power is provided, there's a blue LED, which will
turn on once the device is connected to WiFi and has opened a connection to
the MQTT broker.

Any time wifi or mqtt server connection is lost, the led will turn off until
all connectivity is restored.

# Testing

Simplest method is to run [MQTTBox](http://workswithweb.com/mqttbox.html) and
subscribe to the topic and arm the alarm or disarm it. Both things should
cause MQTT topics to be published. 

Depending on how your Alexor is configured, the armed event won't happen until
the alarm countdown is done and the system is armed, so don't expect instant
event after you've tapped in the pin.

I find it easier to use arm stay since there typically is no countdown.

