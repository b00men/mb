#!/usr/bin/env bash


STOP="killall -9 /usr/local/bin/mb"
RETVAL=0
case "$1" in
start)
cd /etc/mb/conf.d
for ID in *; do
	echo -n "mb_$ID: "
	/usr/bin/spawn-fcgi -P /var/log/nginx/mb_$ID.pip -u www-data -g www-data -s /tmp/mb_$ID-fcgi.sock -- /usr/local/bin/mb -c /etc/mb/conf.d/$ID
done
RETVAL=$?
;;
stop)
$STOP
RETVAL=$?
;;
restart)
$STOP
$START
RETVAL=$?
;;
*)
echo "Usage: mb-spawn-daemon {start|stop|restart}"
exit 1
;;
esac
exit $RETVAL
