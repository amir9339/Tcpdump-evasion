# Tcpdump Evasion

### The code presented here is a C code that create a shared libary for hooking pcap_loop function that originally used by libpcap.
#### The shared libary can hide traffic that used specific tcp port specified by the int: *mystery_tcp_port* (The default is 4200).
#### After compiling the libary, the command `make install` injects it in the file **/etc/ld.so.preload**. This file calls the shared libary before any other libary everytime you running a program in your machine.

###### Compile and Install 
To compile the shared libary just use: 
`make`

To install the shared libary as a preloaded libary in the machine use:
`sudo make install`

And than you can check if the hook succeeded using:
`sudo make test`
 And check if you see the hook message everywhere...

######
###### Uninstall and Clean 
If you wish to unset the shared libary as a preloaded libary:
`sudo make uninstall`

To clean the compiling directory:
`make clean`
