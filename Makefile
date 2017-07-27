# Makefile for mod_mockphp.c (gmake)

# Set to "apxs2" on Debian/Ubuntu
APXS=apxs

# Show all compilation warnings
CFLAGS+= -Wc,-Wall

default:
	@echo mod_mockphp:
	@echo
	@echo following options available:
	@echo \"make mockphp\" to compile the module to ".libs/"
	@echo \"make install\" to install the module
	@echo
	@echo Using apxs on : \"$(APXS)\"


mockphp: mod_mockphp.so
	@echo make done
	@echo type \"make install\" to install mod_mockphp

mod_mockphp.so: mod_mockphp.c
	$(APXS) $(CFLAGS) -c mod_mockphp.c
#	ld -Bshareable -o mod_mockphp.so .libs/mod_mockphp.o -lm

mod_mockphp.c:

install: mod_mockphp.so
	$(APXS) $(CFLAGS) -i -n mod_mockphp .libs/mod_mockphp.so

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo *.loT .libs/
