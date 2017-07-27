# Makefile for mod_mockphp.c (gmake)

# Debian/Ubuntu binary is "apxs2" instead of "apxs"
APXS := $(shell { command -v apxs2 || command -v apxs; } 2>/dev/null)
ifndef APXS
	$(error Could not determine the 'apxs' binary path !)
endif

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

mod_mockphp.so:
	$(APXS) $(CFLAGS) -c mod_mockphp.c
#	ld -Bshareable -o mod_mockphp.so .libs/mod_mockphp.o -lm

install: mod_mockphp.so
	$(APXS) $(CFLAGS) -i -n mod_mockphp .libs/mod_mockphp.so

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo *.loT .libs/
