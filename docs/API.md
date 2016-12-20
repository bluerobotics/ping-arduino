#Ping API Reference

##Accessor Methods

These methods are used to acquire data from the device

-----

* `void update()`

Request and read in new data from Ping. You can then access this data with the methods below.
<br/>
<br/>

* `uint32_t getDistance()`

Returns the most recent smoothed distance reading in mm
<br/>
<br/>

* `uint8_t getConfidence()`

Returns the confidence in the distance measurement, as a percentage
<br/>
<br/>

##Control Methods

These methods are to control the device. Leave any argument as 0 to ignore.

-----

* `sendConfig(uint8_t rate, uint16_t cWater)`

Sends configuration options to Ping. See [here](http://keisan.casio.com/exec/system/1258122391) for determining the speed of sound in water. This depends on salinity and temperature!

|    Argument     |    Value    |             Result             |
|-----------------|-------------|--------------------------------|
| uint8_t rate    | 0           | Ignore                         |
|                 | 1           | Single                         |
|                 | 2           | Continuous with Automatic rate |
| uint16_t cWater | 0           | Ignore                         |
|                 | 1000 - 2000 | Sets the speed of sound in m/s |

<br/>
<br/>

* `sendRequest(uint16_t messageID)`

Requests a message from Ping. See the [Serial Protocol](http://github.com/bluerobotics/ping-python/blob/master/docs/Format.md) for message formats.

|      Argument      | Value |  Result  |
|--------------------|-------|----------|
| uint16_t messageID | 0x01  | distance |
|                    | 0x02  | profile  |
|                    | 0x03  | status   |


<br/>
<br/>


* `sendRange(uint8_8 auto, uint16_t start, uint16_t range)`

Set the range that Ping will scan for the bottom. If manual mode is set, you may specify the start distance and range that Ping will scan, or set 0 to leave those values unchanged.

|    Argument    |   Value     |               Result               |
|----------------|-------------|------------------------------------|
| uint8_t auto   | 0           | Automatic scanning range           |
|                | 1           | Manual scanning range              |
| uint16_t start | 0           | Ignore                             |
|                | 1 - 60000   | Set start distance in mm           |
| uint16_t range | 0           | Ignore                             |
|                | 500 - 60000 | Set length of scanning range in mm |

<br/>
<br/>
