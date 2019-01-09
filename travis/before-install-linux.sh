#!/bin/bash

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 90
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 90

c++ --version
g++ --version
gcc --version

sudo apt-get install -y equivs openjdk-8-jdk

wget -O buck.deb https://github.com/facebook/buck/releases/download/v2018.10.29.01/buck.2018.10.29.01_all.deb
sudo dpkg -i buck.deb
buck --version

wget https://github.com/LoopPerfect/buckaroo/releases/download/$BUCKAROO_VERSION/buckaroo-linux -O buckaroo-client
chmod +x ./buckaroo-client
./buckaroo-client version
