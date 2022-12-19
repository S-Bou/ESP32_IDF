Getting Started with the ESP32 and the ESP-IDF

https://docs.losant.com/guides/getting-started-with-esp32-and-esp-idf/ 

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#get-started-prerequisites 

Now you can run get_idf to set up or refresh the esp-idf environment in any terminal session.

Inside of the esp directory created in Step 2, navigate to the esp-idf directory (this was included in the downloaded repository).
(to add ~ symbol press “option”+”Ñ”)

cd ~/esp/esp-idf

install.sh installs the tools used by ESP-IDF, such as the compiler, debugger, Python packages, etc.

./install.sh

export.sh - A script the ESP-IDF provides to make the tools usable from the command line.

. ./export.sh


Copy the project get-started/hello_world to ~/esp directory:
cd ~/esp

cp -r $IDF_PATH/examples/get-started/hello_world .

Navigate to your hello_world directory, set ESP32 as the target, and run the project configuration utility menuconfig.
cd ~/esp/hello_world
idf.py set-target esp32
idf.py menuconfig

You must replace <your port> with the value of the serial port to the ESP32 device.
	
ls /dev/cu.*

idf.py -p /dev/cu.usbserial-0001 flash monitor


