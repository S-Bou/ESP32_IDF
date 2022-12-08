
//All info is on README.md file on each project

//Go to the directory of projects 

cd D:\Projects\ESP32_IDF

//Create folder to the project

md nombrecarpeta

//Copy project to other location

cd %userprofile%\esp
xcopy /e /i %IDF_PATH%\examples\get-started\hello_world hello_world

xcopy /e /i D:\Programas\Espressif\Esp-idf\frameworks\esp-idf-v4.4.3\examples\peripherals\i2c\i2c_simple

//Set target to ESp32 and open menuconfig

cd %userprofile%\esp\hello_world
idf.py set-target esp32
idf.py menuconfig

//Bild project 

idf.py build

//Flash project and open monitor to debug

idf.py -p COM3 flash monitor