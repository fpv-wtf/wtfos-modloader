

all: jni/*
	ndk-build	

install: all
	install -d ipk/data/opt/etc/preload.d/
	install libs/armeabi-v7a/lib*.so ipk/data/opt/etc/preload.d/

ipk: all
	$(MAKE) -C ipk clean
	$(MAKE) install
	$(MAKE) -C ipk

clean:
	$(MAKE) -C ipk clean