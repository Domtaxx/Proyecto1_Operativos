fdisk -l
mount /dev/sda1 /media/usb
cp /media/usb/main ~
echo 0 >/sys/class/fpga-bridge/lwhps2fpga/enable