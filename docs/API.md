#Ping API Reference

##Accessor Methods

* `float getDepth()`

Returns the most recent smoothed depth reading in mm
<br/>
<br/>

* `float getConfidence()`

Returns the confidence in the depth measurement, as a percentage
<br/>
<br/>

* `float getInstantDepth()`

Returns the best guess for this individual ping in mm. It is recommended to use getDepth() instead
<br/>
<br/>

##Control Methods

* `setQuiet()`

Configures Ping in quiet mode, which is best for using on Arduino. Quiet mode runs at the highest possible ping rate, but only returns a depth measurement when prompted to do so with `update()` . 
<br/>
<br/>

* `setSpeedOfSound(float speed)`

Sets the speed of sound in water. This is necessary to get an accurate depth reading. 
<br/>
<br/>


* `setRange(uint8_8 auto, uint16_t start, uint16_t range)`

Set the range that Ping will scan for the bottom. Set `auto` to 1 for auto mode, 0 for manual. If manual mode is set, you may specify the start depth and range that Ping will scan, or set 0 to leave that value unchanged.
<br/>
<br/>
