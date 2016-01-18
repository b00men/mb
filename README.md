# message board
mb is a message board on cppcms

### Installation

**For compile&work on debian8.amd64 with mysql, nginx and fcgi**

install [libgcrypt11](https://packages.debian.org/wheezy/libgcrypt11) and [libicu48](https://packages.debian.org/wheezy/libicu48>) manual (for apt.cppcms.com)

Then exicute:

    echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list
    apt-get update
    apt-get install git nginx cmake g++ libcppcms-dev libcppdb-dev mysql-server libcppdb-mysql0 libboost-dev libmagick++-dev libboost-regex-dev gettext spawn-fcgi
    git clone https://github.com/b00men/mb.git
    cd mb
    cmake
    make
    make install
    cp /usr/local/share/mb/config.js.sample /usr/local/share/mb/config.js

cppcms_make_key --hmac sha1 --cbc aes
