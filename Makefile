
.PHONY: modloader-o3


all: */jni/*
	NDK_PROJECT_PATH=./modloader_final ndk-build	
	NDK_PROJECT_PATH=./menu_hook ndk-build	

install: all
	install -d ipk/data/opt/bin/
	install -d ipk/data/opt/lib/
	install -d ipk/data/opt/etc/preload.d/
	install -d ipk/data/opt/share/applications/
	install modloader modmanager ipk/data/opt/bin/
	install menu_hook/fbwrapper ipk/data/opt/bin/

	install modloader_final/libs/armeabi-v7a/libmodloader_final.so ipk/data/opt/lib/
	install menu_hook/libs/armeabi-v7a/libmenu_hook.so ipk/data/opt/lib/

ipk: all
	$(MAKE) -C ipk clean
	$(MAKE) install
	$(MAKE) -C ipk
	mv ipk/*.ipk ./

tweaks: tweak-*/jni/*
	$(MAKE) -C tweak-enable-debug-menu ipk
	mv tweak-enable-debug-menu/ipk/*.ipk ./

modloader-o3:
	$(MAKE) -C modloader-o3 ipk
	mv modloader-o3/ipk/*.ipk ./

clean:
	$(MAKE) -C ipk clean
	rm -f *.ipk