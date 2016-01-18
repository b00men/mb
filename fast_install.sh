#/bin/bash
if [[ `whoami` != "root" ]]
then
	echo You need to be root to perform this command. 
	exit 1
fi
wget http://ftp.fr.debian.org/debian/pool/main/libg/libgcrypt11/libgcrypt11_1.5.0-5+deb7u3_amd64.deb
wget http://ftp.fr.debian.org/debian/pool/main/i/icu/libicu48_4.8.1.1-12+deb7u3_amd64.deb
dpkg -i libgcrypt11_1.5.0-5+deb7u3_amd64.deb libicu48_4.8.1.1-12+deb7u3_amd64.deb
echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list
apt-get update
apt-get -y install git nginx cmake g++ libcppcms-dev libcppdb-dev mysql-server libcppdb-mysql0 libboost-dev libmagick++-dev libboost-regex-dev gettext spawn-fcgi
mkdir build
cd build
cmake ..
make
make install

cd ../model
echo "Prepare to create database."
echo "MySQL root passwd:"
echo "CREATE DATABASE IF NOT EXISTS mb" | mysql -u root -p && echo "Database create."
mysql -u root -p mb < mysql.sql
cp /usr/local/share/mb/message_board /etc/nginx/sites-available/message_board
ln -s /etc/nginx/sites-available/message_board /etc/nginx/sites-enabled/message_board
service nginx restart
cp /usr/local/share/mb/config.js.sample /usr/local/share/mb/config.js
cat /usr/local/share/mb/config.js | grep -B 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" > /tmp/config.js
cppcms_make_key --hmac sha1 --cbc aes | grep key >> /tmp/config.js
cat /usr/local/share/mb/config.js | grep -A 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" >> /tmp/config.js
if [ "`cat /tmp/config.js | wc -l`" != 2 ]
then
	echo "cbc & hmac keys successfull generated!"
	cat /tmp/config.js > /usr/local/share/mb/config.js
	rm /tmp/config.js
else
	echo "Not found hmac_key in /usr/local/share/mb/config.js. Keys generate aborting!"
	rm /tmp/config.js
fi

echo "MySQL database name:"
read DATABASE; echo
echo "MySQL login"
read USER; echo
echo "MySQL password:"
read -s PASSWD; echo
echo "$DATABASE $USER $PASSWD"
sed -i -e "s/name_of_datebase/$DATABASE/" -e "s/user_of_datebase/$USER/" -e "s/password_of_database/$PASSWD/" /usr/local/share/mb/config.js
sed -i 's/\/\/\"connection_string\" \: \"mysql/\"connection_string\" \: \"mysql/' /usr/local/share/mb/config.js
