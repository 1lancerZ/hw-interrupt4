make
# insmod tasklet_interrupt.ko irq=31 devname="tasklet_dev"
make install
lsmod | grep tasklet_interrupt
# rmmod tasklet_interrupt.ko
make uninstall
dmesg | tail -n5
make clean
