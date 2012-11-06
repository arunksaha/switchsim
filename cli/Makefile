TRGTS = clicommon climain cliglobal cliconfig cliport cliepu clisimul

all: $(TRGTS)

clicommon:
	cd ./common &&  make all

climain:
	cd ./main ; make all

cliglobal:
	cd ./global ; make all

cliconfig:
	cd ./config ; make all

cliport:
	cd ./port ; make all

cliepu:
	cd ./epu ; make all

clisimul:
	cd ./simul ; make all

clean:
	find . -name *.o | xargs rm -f

