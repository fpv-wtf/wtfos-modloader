NAME = $(shell cat ./control/control | grep Package | cut -d" " -f2)
ARCH = $(shell cat ./control/control | grep Architecture | cut -d" " -f2)
VERSION = $(shell cat ./control/control | grep Version | cut -d" " -f2)
IPK_NAME = "${NAME}_${VERSION}_${ARCH}.ipk"

all:
	mkdir -p tmp
	echo "2.0" > tmp/debian-binary
	cp -r control tmp/
	cd tmp/control && tar --owner=0 --group=0 -czvf ../control.tar.gz .
	cd tmp/ && tar --owner=0 --group=0 -czvf "../${IPK_NAME}" ./control.tar.gz ./debian-binary

clean:
	rm -rf tmp