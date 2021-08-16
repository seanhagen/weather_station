# Weather Station

Code for the brains of a SparkFun MicroMod weather station.

Currently trying to sort out the [JSON
specification](./esp32micromod_platformio_board.json) so I can ensure PlatformIO
is compiling the code with the best set of optimizations and uploading the code
properly.

## End Goal

To have a weather station that reports data over WiFi to an MQTT server. That
way I can use the temperature in Home Assitant, as well as write programs that
listen to MQTT and post the data to weather services or broadcast it via LoRa or
whatever I feel like doing.

## Current Status

Not ready!

## Checklist of Shit to GET DONE

- [x] wind direction
- [x] wind speed
- [ ] rainfall ( see [this Twitter thread](https://twitter.com/seanphagen/status/1427064136391741444) )
- [ ] temperature
- [ ] humidity
- [ ] air pressure
- [ ] lightning dectection
- [ ] uv levels
- [ ] connecting WiFi 
- [ ] publishing messages to MQTT
