# Makefile for mod_mockphp.c (gmake)
APXS=$(shell which apxs) 
APXS2=$(shell which apxs2)

default:
	@echo mod_mockphp:
	@echo
	@echo following options available:
	@echo \"make mockphp\" to compile the module
#	@echo \"make test\" to test the module
	@echo \"make install\" to install the module
	@echo
	@echo Using apxs on : \"$(APXS)\"


mockphp: mod_mockphp.so
	@echo make done
#	@echo type \"make test\" to test mod_mockphp
	@echo type \"make install\" to install mod_mockphp

#test: mockphp
#	@./gen_tests.sh
#	cd t && $(MAKE) test
#	@echo all done


mod_mockphp.so: mod_mockphp.c
	$(APXS) -c -o $@ mod_mockphp.c
	ld -Bshareable -o mod_mockphp.so mod_mockphp.o -lm

mod_mockphp.c:

install: mod_mockphp.so
	$(APXS) -i -n mod_mockphp mod_mockphp.so

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo *.loT .libs/ 
#	cd t && make clean
