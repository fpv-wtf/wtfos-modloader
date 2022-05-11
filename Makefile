

all: modloader_final/jni/*
	NDK_PROJECT_PATH=./modloader_final ndk-build	

install: all
	install -d ipk/data/opt/bin/
	install -d ipk/data/opt/lib/
	install -d ipk/data/opt/etc/preload.d/
	install modloader modmanager ipk/data/opt/bin/
	install modloader_final/libs/armeabi-v7a/libmodloader_final.so ipk/data/opt/lib/

ipk: all
	$(MAKE) -C ipk clean
	$(MAKE) install
	$(MAKE) -C ipk
	mv ipk/*.ipk ./

tweaks: tweak-*/jni/*
	$(MAKE) -C tweak-enable-debug-menu ipk
	mv tweak-enable-debug-menu/ipk/*.ipk ./


clean:
	$(MAKE) -C ipk clean
	rm -f *.ipk