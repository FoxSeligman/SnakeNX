.PHONY: all clean

all:
	@$(MAKE) -C antimony/
	@$(MAKE) -C Snake/

clean:
	@$(MAKE) -C antimony/ clean
	@$(MAKE) -C Snake/ clean
