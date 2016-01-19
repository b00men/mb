#!/usr/bin/env bash
if [ `whoami` != "root" ]
then
	echo You need to be root to perform this command.
	exit 1
fi

ARCH=`uname -r | sed 's/.*-//'`
MB_ID="-"
DEB_WEB="nginx spawn-fcgi"
DEB_DB="mysql-server libcppdb-mysql0"
DEB_BUILD="cmake g++ libcppcms-dev libcppdb-dev libboost-dev libmagick++-dev libboost-regex-dev gettext"
DATABASE="mb_default"
DBUSER="mb_default"
PASSWD=`date +%s | sha256sum | base64 | head -c 32`

echo
echo "This install script can create one or several sites on mb.
Compose message board id for current site.
ID can include alphabets, digs and must be less 32 chars.
Remember, if ID exist, config and database will be rewrited!"
echo
while [ "`echo $MB_ID | sed 's/[[:alnum:]]//g'`" != "" ]
do
read -n32 -p "Message board ID or press [Enter] to default: " MB_ID
done

if [ "$MB_ID" != "" ]
then
	DATABASE="mb_$MB_ID"
	DBUSER="mb_$MB_ID"
fi

echo
echo -n "Add apt.cppcms.com to sources and apt-get update... "
echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list &&
apt-get update -qq && echo done || exit 1
echo

if dpkg --list libgcrypt11 libicu48 > /dev/null
then
        echo "Libgcrypt11 and libicu48 avalible in repo or already install"
	DEB_ibgcrypt_libicu="libgcrypt11 libicu48"
else
	echo -n "Download libgcrypt11 and libicu48... "
	if wget -q http://ftp.fr.debian.org/debian/pool/main/libg/libgcrypt11/libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb && wget -q http://ftp.fr.debian.org/debian/pool/main/i/icu/libicu48_4.8.1.1-12+deb7u3_$ARCH.deb && echo done || exit 1
	then
		echo
		echo "Manual install libgcrypt11 and libicu48:"
		dpkg -i libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb libicu48_4.8.1.1-12+deb7u3_$ARCH.deb || exit 1
	fi
fi
echo

echo -n "Install dependets packages... "
apt-get install -y --force-yes -qq $DEB_WEB $DEB_DB $DEB_BUILD $DEB_ibgcrypt_libicu && echo done || exit 1
echo

echo "Build and install message board."
mkdir -p build &&
cd build &&
cmake .. &&
make &&
make install || exit 1
echo

echo "Configure MySQL."
cd ../model &&
echo "GRANT USAGE ON *.* TO '$DBUSER'@'localhost';
DROP USER '$DBUSER'@'localhost';
CREATE DATABASE IF NOT EXISTS $DATABASE; USE '$DATABASE';
CREATE USER '$DBUSER'@'localhost' IDENTIFIED BY '$PASSWD';
GRANT ALL PRIVILEGES ON $DATABASE . * TO '$DBUSER'@'localhost';
FLUSH PRIVILEGES;" >  mysql_ready.sql &&
cat mysql.sql >> mysql_ready.sql || (echo "Cannot create tmp sql file!" ; exit 1)
echo -n "MySQL root passwd for create mb database: "
mysql -u root -p < mysql_ready.sql || exit 1
echo

echo -n "Copy and enable site tamplate for nginx (port 8060 default)... "
cp /usr/local/share/mb/message_board /etc/nginx/sites-available/message_board && ln -sf /etc/nginx/sites-available/message_board /etc/nginx/sites-enabled/message_board && echo done || exit 1
echo -n "Reload nginx... "
service nginx reload && echo done || exit 1
echo

echo "Configure message board."
echo -n "Create config file... "
cp /usr/local/share/mb/config.js.sample /usr/local/share/mb/config.js && echo done || exit 1
echo -n "Generate cbc and hmac keys... "
cat /usr/local/share/mb/config.js | grep -B 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" > /tmp/config.js &&
cppcms_make_key --hmac sha1 --cbc aes | grep key >> /tmp/config.js &&
cat /usr/local/share/mb/config.js | grep -A 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" >> /tmp/config.js &&
if [ "`cat /tmp/config.js | wc -l`" != 2 ]
then
	cat /tmp/config.js > /usr/local/share/mb/config.js &&
	rm /tmp/config.js && echo done || exit 1
else
	echo "Not found hmac_key in /usr/local/share/mb/config.js! Aborted."
	exit 1
fi || exit 1
echo -n "Add to config MySQL connection... "
sed -i -e "s/name_of_datebase/$DATABASE/" -e "s/user_of_datebase/$DBUSER/" -e "s/password_of_database/$PASSWD/" /usr/local/share/mb/config.js &&
sed -i 's/\/\/\"connection_string\" \: \"mysql/\"connection_string\" \: \"mysql/' /usr/local/share/mb/config.js &&
echo done || exit 1

echo "Message board successfull install and configure!"
echo -n "Message board start... "
.././mb-spawn-daemon start && echo done || exit 1
echo "Try http://localhost:8060 to connect"
