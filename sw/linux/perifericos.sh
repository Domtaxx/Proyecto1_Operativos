fdisk -l
mount /dev/sda1 /media/usb
cp /media/usb/perifericos /home/
echo 0 >/sys/class/fpga-bridge/lwhps2fpga/enable