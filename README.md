Tores Sketch

Arduino scetch to talk to i2c connected devices.

Commands:

scan
----
Scans for devices on i2c bus and returns their addresses.


dump addr size
--------------
Reads bytes and returns them formated on screen.
addr - Address of the device.
size - Number of bytes to read.
       Just block of 16 bytes.


read addr size
--------------
Reads bytes and returns them as they are.
Used to save a dump file.

addr - Address of the device.
size - Number of bytes to read.
       Just block of 16 bytes.


write addr memPos value
-----------------------
Writes a single byte on a device at the given position.

addr   - Address of the device.
memPos - Memory position to write to.
value  - The value to wite.
