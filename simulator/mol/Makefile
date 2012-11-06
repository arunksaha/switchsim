TRGTS = molcommon molport molepu

all: $(TRGTS)

molcommon:
	cd ./common &&  make all

molport:
	cd ./port ; make all

molepu:
	cd ./epu ; make all

clean:
	find . -name *.o | xargs rm -f

