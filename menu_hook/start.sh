#!/bin/bash
set -e
adb connect 192.168.42.5
adb shell "setprop dji.glasses_wm150_service 0 && busybox killall dji_gls_wm150"
ndk-build
adb push libs/armeabi-v7a/lib*.so /tmp
adb shell "LD_PRELOAD=\"/tmp/lib$1.so /opt/lib/libmodloader_final.so\" dji_gls_wm150_original -g"