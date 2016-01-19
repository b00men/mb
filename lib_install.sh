#!/usr/bin/env bash
function check_permission {
	if [ `whoami` != "root" ]
	then
		echo "You need to be root to perform this script."
		exit 1
	fi
}

function set_var {
	echo
	echo "This install script can create one or several sites on mb.
	Compose message board ID for current site.
	ID can include alphabets, digs and must be less 32 chars.
	Remember, if ID exist, config and database will be rewrited!"
	echo
	MB_ID="-"
	while [ "`echo $MB_ID | sed '/^[[:alnum:]]\{,32\}$/d'`" != "" ]
	do
	read -p "Enter mb ID or press [Enter] to default: " MB_ID
	done

	if [ -z "$MB_ID" ]
	then
		MB_ID="default"
		MB_PORT="8060"
	else
		while [ -z "`echo $MB_PORT | sed -n -e '/^[1-9][0-9]\{1,3\}$/p' -e '/^[1-3][0-9]\{4\}$/p' -e '/^4[0-9][0-1][0-5][0-1]$/p'`" ]
		do
		read -n32 -p "Enter mb port: " MB_PORT
		done
	fi

	ARCH=`uname -r | sed 's/.*-//'`
	DEB_WEB="nginx spawn-fcgi"
	DEB_DB="mysql-server libcppdb-mysql0"
	DEB_BUILD="cmake g++ libcppcms-dev libcppdb-dev libboost-dev libmagick++-dev libboost-regex-dev gettext"
	DATABASE="mb_$MB_ID"
	DBUSER="mb_$MB_ID"
	PASSWD=`date +%s | sha256sum | base64 | head -c 32`
}

function install_depending {
	echo
	echo -n "Add apt.cppcms.com to sources and apt-get update... "
	echo "deb http://apt.cppcms.com/ wheezy main" > /etc/apt/sources.list.d/cppcms.list &&
	apt-get update -qq && echo done || exit 1
	echo

	if dpkg --list libgcrypt11 libicu48 > /dev/null
	then
	    #echo "Libgcrypt11 and libicu48 avalible in repo or already install"
		DEB_ibgcrypt_libicu="libgcrypt11 libicu48"
	else
		echo -n "Download libgcrypt11 and libicu48... "
		if wget -q http://ftp.fr.debian.org/debian/pool/main/libg/libgcrypt11/libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb && wget -q http://ftp.fr.debian.org/debian/pool/main/i/icu/libicu48_4.8.1.1-12+deb7u3_$ARCH.deb && echo done || exit 1
		then
			echo
			echo "Manual install libgcrypt11 and libicu48:"
			dpkg -i libgcrypt11_1.5.0-5+deb7u3_$ARCH.deb libicu48_4.8.1.1-12+deb7u3_$ARCH.deb || exit 1
			echo
		fi
	fi

	echo -n "Install dependets packages... "
	apt-get install -y --force-yes -qq $DEB_WEB $DEB_DB $DEB_BUILD $DEB_ibgcrypt_libicu && echo done || exit 1
	echo
}

function build_and_install {
	echo "Build and install message board."
	mkdir -p build &&
	cd build &&
	cmake .. &&
	make &&
	make install || exit 1
	echo
}

function config_mysql {
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
}

function config_nginx {
	echo -n "Configure and enable site tamplate for nginx... "
	cp /usr/local/share/mb/message_board /etc/nginx/sites-available/$DATABASE &&
	ln -sf /etc/nginx/sites-available/$DATABASE /etc/nginx/sites-enabled/$DATABASE &&
	sed -i "s/8060/$MB_PORT/g" /etc/nginx/sites-available/$DATABASE &&
	sed -i "s/message_board_name/$MB_ID/g" /etc/nginx/sites-available/$DATABASE &&
	sed -i "s/mb-fcgi\.sock/mb_$MB_ID-fcgi.sock/" /etc/nginx/sites-available/$DATABASE &&
	echo done || exit 1
	echo -n "Reload nginx... "
	service nginx reload && echo done || exit 1
	echo
}

function config_mb_daemon {
	echo "Configure message board daemon."
	echo -n "Create mb-daemon... "
	cp ../mb-daemon /etc/init.d/ && echo done || exit 1
	echo "Enable mb-daemon... "
	systemctl enable mb-daemon  && echo done || exit 1
	echo
}

function config_mb {
	echo "Configure message board."
	echo -n "Create config file... "
	mkdir -p /etc/mb/conf.d &&
	cp /usr/local/share/mb/config.js.sample /etc/mb/conf.d/$MB_ID && echo done || exit 1
	echo -n "Generate cbc and hmac keys... "
	cat /etc/mb/conf.d/$MB_ID | grep -B 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" > /tmp/config.js &&
	cppcms_make_key --hmac sha1 --cbc aes | grep key >> /tmp/config.js &&
	cat /etc/mb/conf.d/$MB_ID| grep -A 1000 "\"hmac_key\"" | grep -v -e "\"hmac_key\"" -e "\"cbc_key\"" >> /tmp/config.js &&
	if [ "`cat /tmp/config.js | wc -l`" != 2 ]
	then
		cat /tmp/config.js > /etc/mb/conf.d/$MB_ID &&
		rm /tmp/config.js && echo done || exit 1
	else
		echo "Not found hmac_key in /etc/mb/conf.d/$MB_ID! Aborted."
		exit 1
	fi || exit 1
	echo -n "Add to config fcgi sock file... "
	sed -i "s/mb-fcgi\.sock/mb_$MB_ID-fcgi.sock/" /etc/mb/conf.d/$MB_ID && echo done || exit 1
	echo -n "Add to config MySQL connection and upload dir... "
	sed -i -e "s/name_of_mb/$MB_ID/g" -e "s/name_of_datebase/$DATABASE/g" -e "s/user_of_datebase/$DBUSER/g" -e "s/password_of_database/$PASSWD/g" /etc/mb/conf.d/$MB_ID &&
	sed -i 's/\/\/\"connection_string\" \: \"mysql/\"connection_string\" \: \"mysql/' /etc/mb/conf.d/$MB_ID &&
	echo done || exit 1
	echo -n "Create upload directory... "
	rm -Rf /srv/mb/$MB_ID/media/uploads &&
	mkdir -p /srv/mb/$MB_ID/media/uploads &&
	chown -R www-data:www-data /srv/mb/$MB_ID && echo done || exit 1
}

function run_and_epilogue {
	echo "Message board successfull install and configure!"
	echo -n "Message board start. "
	service mb-daemon restart || exit 1
	echo "Try http://localhost:$MB_PORT to connect"
}