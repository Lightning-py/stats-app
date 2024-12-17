#
Stats-app is a simple Qt-based application for device's temperature monitoring. It made ONLY for Linux and ONLY for nvidia GPUs.

## Building
```bash
sudo apt install qt6-base-dev # Install Qt6

git clone github.com/Lightning-py/stats-app
cd stats-app
mkdir -p build
cd build
cmake -S .. -B .
make
make install
```

You can add .desktop file for simple usage

## Usage
When application starts it appears in the left top corner of your current screen and shows CPU and GPU temperature. If temperature value is -1, it means that program can't find a way to determine your device's temperature. 
For example, app built for nvidia GPU, so if you don't have one it will show -1.
When devices temperature exit the safe state stats-app will send you notification (if device's temperature don't enter the safe state in 10 seconds, notification will apper again and again).
