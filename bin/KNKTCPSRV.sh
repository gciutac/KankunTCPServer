#!/bin/bash
#
# Get function from functions library
. /etc/init.d/functions
# Start the server
start() {
        initlog -c "echo -n Starting Kankun TCP server: "
        /tmp/MyKankunTCPSrv/knkonoffsrv $TCPPORT $PWDCNTFILE &
        ### Create the lock file ###
        touch /var/lock/subsys/knkonoffsrv
        success $"knkonoffsrv server startup"
        echo
}
# Stop the server knkonoffsrv
stop() {
        initlog -c "echo -n Stopping Kankun TCP server: "
        killproc knkonoffsrv
        ### Now, delete the lock file ###
        rm -f /var/lock/subsys/knkonoffsrv
        echo
}
### main logic ###
source ./knkonoffsrv.cfg

case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  status)
        status knkonoffsrv
        ;;
  restart|reload)
        stop
        start
        ;;
  *)
        echo $"Usage: $0 {start|stop|restart|reload|status}"
        exit 1
esac
exit 0