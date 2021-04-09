## what is this ?
this is a set x11 window opacity tool
using qt5
#### （Contact author）
send emails liuminghang0821@gmail.com
#### Linux build-dep
sudo apt install qt5-qmake qtbase5-dev qttools5-dev-tools libxcb-ewmh-dev libqt5x11extras5-dev libx11-xcb-dev
sudo apt install pkg-config, dpkg-dev,
#### Linux build 
mkdir build
cd build
qmake ..
make 
make install

# debian build 
dpkg-buildpakage -b -us -uc

