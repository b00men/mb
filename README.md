# message board
mb is a message board on cppcms

### Installation

**For compile&work on debian8.amd64 with mysql, nginx and fcgi**

install [libgcrypt11](https://packages.debian.org/wheezy/libgcrypt11) and [libicu48](https://packages.debian.org/wheezy/libicu48>) manual (for apt.cppcms.com)

    wget http://ftp.fr.debian.org/debian/pool/main/libg/libgcrypt11/libgcrypt11_1.5.0-5+deb7u3_amd64.deb
    wget http://ftp.fr.debian.org/debian/pool/main/i/icu/libicu48_4.8.1.1-12+deb7u3_amd64.deb
    dpkg -i libgcrypt11_1.5.0-5+deb7u3_amd64.deb libicu48_4.8.1.1-12+deb7u3_amd64.deb

Then exicute:

    echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list
    apt-get update
    apt-get install git nginx cmake g++ libcppcms-dev libcppdb-dev mysql-server libcppdb-mysql0 libboost-dev libmagick++-dev libboost-regex-dev gettext spawn-fcgi
    git clone https://github.com/b00men/mb.git
    cd mb
    cmake .
    make
    make install
    cp /usr/local/share/mb/config.js.sample /usr/local/share/mb/config.js

cppcms_make_key --hmac sha1 --cbc aes
echo "CREATE DATABASE IF NOT EXISTS mb" | mysql -u root -p
mysql -u root -p mb < mysql.sql
ln -s /etc/nginx/sites-available/message_board /etc/nginx/sites-enabled/message_board
