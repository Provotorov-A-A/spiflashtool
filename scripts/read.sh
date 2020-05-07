./OUTPUT/bin/spiflashtool.exe -read -filename ./dump.bin -address 0 -size 1k -port COM15 -baudrate 57600 -verbose ; od -x ./dump.bin
