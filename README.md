C gpio experiments on the Firefly ROC-RK3328-CC board.

The following 8 tests are included: ( see below for tests summary )
1. uart test
2. led test
3. button test
4. pwm led test
5. i2c lcd test
6. tongsong
7. servo
8. spi oled test

-------------------------------------------------------------------
To compile and flash to sd card:
cd roc-rk3328-cc-c-gpio
Download OS from https://www.armbian.com/renegade/
select Armbian Buster.
unxz Armbian_20.11.6_Renegade_buster_current_5.9.14.img.xz
Use balenaEtcher to burn img to sd card.
eject sd card.
Plugin sd card to PC.
generate pwm.dtbo, spi.dtbo and uart.dtbo:
dtc -O dtb -o pwm.dtbo -b 0 -@ pwm.dts
fdtdump pwm.dtbo
sudo mkdir /media/$USER/7077008c-18aa-4156-b4fb-1dc8497aad69/boot/overlay-user
copy pwm.dtbo, spi.dtbo and uart.dtbo to:
/media/$USER/7077008c-18aa-4156-b4fb-1dc8497aad69/boot/overlay-user
sudo cp armbianEnv.txt /media/$USER/7077008c-18aa-4156-b4fb-1dc8497aad69/boot
sync
sudo umount /media/$USER/7077008c-18aa-4156-b4fb-1dc8497aad69
eject sd card.
Plugin the sd card to Firefly ROC-RK3328-CC board.
Plugin ethernet cable to Firefly ROC-RK3328-CC board
Connect ROC-RK3328-CC gpio Pin 36 to serial USB cable TX.
Connect ROC-RK3328-CC gpio pin 38 to serial USB cable RX. 
Connect ROC-RK3328-CC gpio pin 39 to serial USB cable ground. 
Type "script ~/outputfile.txt" on PC.
Plugin serial USB cable to PC.
Type "sudo screen /dev/ttyUSB0 1500000" on PC.
Power on Firefly ROC-RK3328-CC board.
set user, password, locale and time zone.
sudo apt-get update
sudo apt-get upgrade
sudo shutdown -h now
Power on Firefly ROC-RK3328-CC board.
vi nosleep.sh ( add following line to disable sleep feature ).
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target
./nosleep.sh
sudo dmesg -n 1
sudo vi /etc/rc.local ( add sudo dmesg -n 1 ).
sudo apt-get install python-dev python-pip python-setuptools python3-dev python3-pip python3-setuptools dnsutils apache2 vsftpd ftp build-essential git libssl-dev nmap net-tools libssl-dev dkms libncurses5-dev libncursesw5-dev
sudo cat /proc/device-tree/spi@ff190000/status
sudo cat /proc/device-tree/pwm@ff1b0020/status
sudo cat /proc/device-tree/i2c@ff160000/status
ls /sys/class/pwm
sudo apt-get install i2c-tools
sudo i2cdetect -y 1

-------------------------------------------------------------------------
Install libgpiod:
sudo apt-get install libgpiod-dev
see libgpiod examples https://github.com/starnight/libgpiod-example/blob/master/libgpiod-input/main.c
cd ~/roc-rk3328-cc-c-gpio
make gpio_test
sudo ./gpio_test
When done all tests, hit 'q' to exit tests.
sudo shutdown -h now
Power off the Firefly ROC-RK3328-CC board.
Unplug serial USB cable from PC.
Type "exit" on PC.

-------------------------------------------------------------------------
Here are the summary of the tests: ( see rk3328_gpio.png )
These tests used Seeed Grove  starter kit LED, button, buzzer, Grove-LCD RGB Backlight V3.0 JHD1313M2, Analog Servo and Adafruit SSD1306 128x32 SPI OLED Display.
1. uart test.
   This test will send uart1 tx to uart1 rx for loopback.
   It sends ' ' to '~' to uart1 tx and receive ' ' to '~' from uart1 rx.
   Connect gpio pin 8 to pin 10.
2. led test.
   This test will blink led 5 times. 
   Connect gpio pin 32 to led control. 
   Connect gpio pin 2 to led 5V. 
   Connect gpio pin 9 to led ground.
3. button test. 
   Connect gpio pin 32 to led control. 
   Connect gpio pin 2 to led 5V. 
   Connect gpio pin 9 to led ground. 
   Connect gpio pin 22 to button control.
   Connect gpio pin 4 to button 5V.
   Connect gpio pin 6 to button ground.
4. pwm led test.
   This test will dim led 10 times.
   Connect gpio pin 12 to led control.
   Connect gpio pin 2 to led 5V.
   Connect gpio pin 9 to led ground.
5. i2c lcd test.
   This test will change lcd backlight color for 5 cycles.
   Then it will display two sentences on lcd display.
   Connect gpio pin 27 to lcd display SDA.
   Connect gpio pin 28 to lcd display SCL.
   Connect gpio pin 2 to lcd display 5V.
   Connect gpio pin 9 to lcd display ground.
6. tongsong.
   This test will generate song using buzzer.
   Connect gpio pin 12 to buzzer control.
   Connect gpio pin 2 to buzzer 5V.
   Connect gpio pin 9 to buzzer ground. 
7. servo.
   This test will turn servo 90 degree - 180 degree - 90 degree - 0 degree etc.
   Connect gpio pin 12 to servo control.
   Connect gpio pin 2 to servo 5V.
   Connect gpio pin 9 to servo ground.
8. spi oled test.
   This test will show some ascii characters on the oled display.
   Connect gpio pin 32 to oled display DC.
   Connect gpio pin 24 to oled display CS.
   Connect gpio pin 19 to oled display TX.
   Connect gpio pin 23 to oled display CLK.
   Connect gpio pin 1 to oled display 3.3V.
   Connect gpio pin 9 to oled display ground.

-----------------------------------------------------------------------------