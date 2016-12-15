#Ping API Reference

##Accessor Methods

* `uint32_t  getDepth()`

Returns the most recent smoothed depth reading in mm
<br/>
<br/>

* `uint8_t getConfidence()`

Returns the confidence in the depth measurement, as a percentage
<br/>
<br/>

* `uint32_t getInstantDepth()`

Returns the best guess for this individual ping in mm. It is recommended to use getDepth() instead
<br/>
<br/>

##Control Methods

These methods are to control the device. Leave any argument as 0 to ignore, or leave as the default. 

* `sendConfig(uint8_t rate, uint16_t cWater)`

Sends configuration options to Ping. See [here](http://keisan.casio.com/exec/system/1258122391) for determining the speed of sound in water. This depends on salinity and temperature!

|    Argument     |    Value    |             Result             |
|-----------------|-------------|--------------------------------|
| uint8_t rate    | 0           | Default / unchanged            |
|                 | 1           | Single                         |
|                 | 2           | Continuous with Automatic rate |
| uint16_t cWater | 0           | Default / unchanged            |
|                 | 1000 - 2000 | Sets the speed of sound in m/s |

<br/>
<br/>

* `sendRequest(uint16_t messageID)`

Requests a message from Ping. See the [Serial Protocol](http://github.com/bluerobotics/ping-python/blob/master/docs/Format.md) for message formats. 

|      Argument      | Value |  Result  |
|--------------------|-------|----------|
| uint16_t messageID | 0x01  | altitude |
|                    | 0x02  | profile  |
|                    | 0x03  | status   |


<br/>
<br/>


* `sendRange(uint8_8 auto, uint16_t start, uint16_t range)`

Set the range that Ping will scan for the bottom. If manual mode is set, you may specify the start depth and range that Ping will scan, or set 0 to leave those values unchanged.

|    Argument    |   Value     |               Result               |
|----------------|-------------|------------------------------------|
| uint98_8 auto  | 0           | Automatic scanning range           |
|                | 1           | Manual scanning range              |
| uint16_t start | 0           | Default / unchanged                |
|                | 1 - 60000   | Set start depth in mm              |
| uint16_t range | 0           | Default / unchanged                |
|                | 500 - 60000 | Set length of scanning range in mm |

<br/>
<br/>
