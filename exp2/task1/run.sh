make
insmod workqueue_test.ko times=5
lsmod | grep workqueue_test
sleep 16
rmmod workqueue_test.ko
dmesg | tail -n20
make clean
