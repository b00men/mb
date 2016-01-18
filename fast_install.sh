#!/usr/bin/env bash
if [ `whoami` != "root" ]
then
	echo You need to be root to perform this command. 
	exit 1
fi
ARCH=`uname -r | sed 's/.*-//'`
echo "Download and manual install libgcrypt11 and libicu48:"
wget -q http://ftp.fr.debian.org/debian/pool/main/libg/libgcrypt11/libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb
wget -q http://ftp.fr.debian.org/debian/pool/main/i/icu/libicu48_4.8.1.1-12+deb7u3_$ARCH.deb
dpkg -i libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb libicu48_4.8.1.1-12+deb7u3_$ARCH.deb
echo

echo "Add apt.cppcms.com to sources and install dependets packeges:"
echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list
apt-get update -qq
apt-get install -y --force-yes -qq git nginx cmake g++ libcppcms-dev libcppdb-dev mysql-server libcppdb-mysql0 libboost-dev libmagick++-dev libboost-regex-dev gettext spawn-fcgi
echo

echo "Build and install message board:"
mkdir -p build
cd build
cmake ..
make
make install
echo


echo "Configure MySQL."
cd ../model
echo "MySQL root passwd for database creating:"
echo "CREATE DATABASE IF NOT EXISTS mb" | mysql -u root -p && echo "Database create."
mysql -u root -p mb < mysql.sql
echo

echo "Copy site tamplate for nginx:"
cp /usr/local/share/mb/message_board /etc/nginx/sites-available/message_board
ln -s /etc/nginx/sites-available/message_board /etc/nginx/sites-enabled/message_board
echo "Reload nginx:"
service nginx reload
echo

echo "Configure message board:"
cp /usr/local/share/mb/config.js.sample /usr/local/share/mb/config.js
cat /usr/local/share/mb/config.js | grep -B 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" > /tmp/config.js
cppcms_make_key --hmac sha1 --cbc aes | grep key >> /tmp/config.js
cat /usr/local/share/mb/config.js | grep -A 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" >> /tmp/config.js
if [ "`cat /tmp/config.js | wc -l`" != 2 ]
then
	echo "cbc & hmac keys successfull generated."
	cat /tmp/config.js > /usr/local/share/mb/config.js
	rm /tmp/config.js
else
	echo "Not found hmac_key in /usr/local/share/mb/config.js. Keys generate aborting!"
	rm /tmp/config.js
fi

echo -n "MySQL database name:"
read DATABASE; echo
echo -n "MySQL login"
read USER; echo
echo -n "MySQL password:"
read -s PASSWD; echo
sed -i -e "s/name_of_datebase/$DATABASE/" -e "s/user_of_datebase/$USER/" -e "s/password_of_database/$PASSWD/" /usr/local/share/mb/config.js
sed -i 's/\/\/\"connection_string\" \: \"mysql/\"connection_string\" \: \"mysql/' /usr/local/share/mb/config.js
echo

