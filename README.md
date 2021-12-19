## Tcpdump Evasion

##### The code presented here is a C code that creates a shared library for hooking the pcap_loop function that was originally used by libpcap.
The shared library can hide traffic that uses a specific tcp port specified by the int: mystery_tcp_port (The default is 4200).

After compiling the library, the command `make install` injects library path in the file /etc/ld.so.preload. This file calls the shared library before any other library every time you run a program on your machine.

#### Compile and Install 
To compile the shared library just use:
```make```

To install the shared library as a preloaded library in the machine use:
```sudo make install```

And then you can check if the hook succeeded using:
```sudo make test```
 And check if you see the hook message everywhere...

#### Uninstall and Clean 
If you wish to unset the shared librliary as a preloaded library:
```sudo make uninstall```

To clean the compiling directory:
```make clean```
