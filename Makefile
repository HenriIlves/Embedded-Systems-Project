.PHONY: all clean flash flash-master flash-slave

all:
	$(MAKE) -C master
	$(MAKE) -C slave

flash: flash-master flash-slave

flash-master:
	$(MAKE) -C master flash

flash-slave:
	$(MAKE) -C slave flash

clean:
	$(MAKE) -C master clean
	$(MAKE) -C slave clean
