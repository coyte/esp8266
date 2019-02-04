# MQTT Bridge for RG-11 rain sensor

This turns the ESP8266 into a MQTT Client which listens to a 
[RG-11 rain sensor](http://rainsensors.com/how-it-works/). 
What makes this different than your regular tip-bucket rain sensor is
that it is both more sensitive (down to 0.0001" of rain) and impervious to
leafs and such since it uses an optical mechanism to detect rain.

The RG-11 must be running in virtual bucket mode. The sensitivity is entirely
up you, likewise what unit to use, since the bridge will simple publish the number of times the bucket emptied, anywhere from 1 to 255 depending on the load and speed of the network.

# Software install

Change the `ssid` field to point to your wifi, likewise you need to change
`password` to match.

You also want to point out the MQTT broker by changing `mqtt_server`.

If you want, you can also choose what the topic shall be when rain is detected by editing `rain_topic`.

Once done, flash the ESP8266 with the software and hook it up

# Hardware install

Provide the RG-11 with 12VDC and hook up the NO relay to Pin 13 and Ground 
on the ESP8266. Provide the ESP8266 with 5V and you're good to go hardware wise.

# Running

Once all is connected and power is provided, there's a blue LED, which will
turn on once the device is connected to WiFi and has opened a connection to
the MQTT broker.

Any time wifi or mqtt server connection is lost, the led will turn off until
all connectivity is restored.

# Testing

Simplest method is to run [MQTTBox](http://workswithweb.com/mqttbox.html) and subscribe to the topic and then pour
water on the dome of the RG-11. A green LED inside the dome should flash,
which indicates that one or more rain events where sent. The MQTTBox should also show you `house/outside/rain` with the payload of 1 or more.

