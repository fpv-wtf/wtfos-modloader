# modloader for wtfos
modloader, along with it's companion `modmanager`, allow you to enable and disable of loading of shared libraries from /opt/etc/preload.d/ into vendor processes.

## Usage

Install a mod through package management or place your shared library into **/opt/etc/preload.d/lib${your_mod_name}.so**.

Then, to enable your_mod_name on the DIY GUI process run the following:

    modmanager enable diy_glasses your_mod_name

To later disable it run:

    modmanager disable diy_glasses your_mod_name

To list enabled mods for DIY GUI run:

    modmanager list diy_glasses
Note that most packages will probably enable their respective mod for you by default on install.

**Shortcuts**

Since the DIY GUI has different process names on V1 and V2 Goggles the following shortcuts are available for you:

 - **diy_glasses** - dji_glassess on V1 and dji_gls_wm150 on V2
 - **fpv_glasses** - dji_glasses on V2

## For developers
This simply allows you to inject your shared library into the vendor's service processes, what you do from there is up to you. You could:

 - Use function overriding to replace any function calls for any other shared libraries with your own implementations
 - Use your shared library constructor and destructor methods to run code before the vendor process starts or at shutdown

See the [debug menu enabler](tweak-enable-debug-menu/) for a full example of simple GUI lib method replacement (a NOP in this case) including a Makefile that will produce a complete .ipk for you.

In addition to the injectable library itself, you will also need to take care to update all the files in ipk/control/ with appropriate values for your library. Note that the debug menu enabler project uses a dinit unit to allow the user to toggle the mod via configurator, located at ipk/data/opt/etc/dinit.d/tweak-enable-debug-menu and enables that unit during postinst.  

## Main menu injection
Packages can include commands to be executed in the main menu by populating a .desktop file in /opt/share/applications/ with the Name, Exec, Icon and (custom) Icon_Active keys being respected. Icon and Icon_Active are optional.

When your application needs to take over the framebuffer it should do so by launching a dinit unit with an Exec value like `dinitctl start my-unit-name`. An optional `fbwrapper` utility is included to aid in starting and stopping the DJI glasses process.

See the [examples directory](menu_hook/examples/) for a .desktop file and dinit unit.

## How this works
DJI's services (i.e. when you start something with `setprop dji.something_service 1`) are managed by Android init, which only reads it's config files once during bootup from the / ramfs before we ever have a chance to modify them.

Thus, to inject shared libraries into the various services, we move the service binaries out of place (in the loopmount /system image) and replace them with symlinks to modloader.

When init executes the symlink, modloader determines the executed binaries name, inspects the folder /opt/etc/preload.d/${service_binary_name}/ for symlinks to files to load and populates **LD_PRELOAD** according to .so-s found in an alphabetic order. It then proceeds to call the original service binary by appending "_original" to the name determined earlier.

When mods are found for a binary, a final entry to LD_PRELOAD is appended for libmodloader_final.so, which gets loaded last after any other libraries, and clears the processes LD_PRELOAD environment variable. This is done to ensure any modifications don't get loaded needlessly into DJI's excessive shell spawns.