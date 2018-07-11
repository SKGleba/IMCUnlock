# IMCUnlock
MBR Magic by SKGleba

![ref0](https://cdn.discordapp.com/attachments/466454495258476545/466463541185216512/IMG_20180711_063013.jpg)

# This guide applies ONLY to Playstation Vita 1XXX models, commonly called "Phat".
# Introduction:
This mod allows you to _permanently_ add a internal ux0 storage to a PSVita Phat by modifying boot data regions.

![ref1](https://cdn.discordapp.com/attachments/466454244929699861/466458976901529600/IMG_20180710_180418.jpg)

The only downside is that you "lose" pd0 - "Welcome Park" and default BGM (it becomes either internal ux0 or ur0).

![ref2](https://cdn.discordapp.com/attachments/466454244929699861/466460781756809216/IMG_20180711_062701.jpg)

# Installation:
1) Download and install the provided VPK file. If you are using ENSO NSKBL hack, make sure that the correct enso installer is installed.

2) Make a backup of all important ur0 files (they should be safe, but better to be sure)

3) Open IMCUnlock, accept the agreement, navigate to "Options->Backup", and select "Backup vd0". Eventually you may backup pd0/tm0/id_data too. Dumps are saved to ux0:data/%name%.img

4) Disable all unnecessary plugins and reboot.

5) Now, we will install the mod. Open IMCUnlock, accept the agreement and:
  - If you want to proceed with the default install (pd0 removed, ur0 trimmed to 300mb, and internal ux0 of size ~2.5GB created) - select "Enable internal ux storage"
  
  - If you want to proceed with the ur0-untouched install (pd0 removed, and internal ux0 of size ~300MB created) - navigate to "Options->Storage size", and select "300MB"
  
6) A prompt asking if you are sure that you want to flash IMCUnlock will appear. After preesing "Yes" the installer will start the flash process.

7) DO NOT TOUCH THE DEVICE UNTIL IT FINISHES

8) After flashing, the following screen should appear:
![ref6](https://cdn.discordapp.com/attachments/466454495258476545/466462385499275274/IMG_20180711_063154.jpg)

9) If you are a user of ENSO NSKBL hack - launch the enso installer and install enso, otherwise - reboot.

10) If your vita reboots fine - thats it! You just permanently enabled internal ux0 storage on your phat vita.

# FAQ:
 - Q: What do you mean by "permanent"? Does it stay after fw update/reinstall?
   - A: Yes, the only way to remove the mod is to uninstall via the provided manager.
 - Q: Can I update when a new firmware comes out?
   - A: No, you should wait for someone to confirm that it works/or that it breaks the vita.
 - Q: My device is stuck on bootlogo/health warning/spinning circle, help!
   - A: If enso logo - reinstall the firmware, If sony logo/health warning/spinning circle - remove sony mc, boot to safemode, select "Format Memory Card". Alternatively, system restore may help.
 - Q: Why the vita does a soft reboot when i remove/insert the mc?
   - A: Patches are applied. (TBH idk, something related to the parser)
 - Q: How can i restore a backupped partition image?
   - A: "Options->Restore"
 - Q: Im getting a critical error at boot, and i cant restore the firmware via safemode.
   - A: Something happened while the installer was running, its a unrecoverable BRICK.
 - Q: Is this tool compatible with XXXXX?
   - A: No
 - Q: You did a great job! How can i help/donate you?
   - A: paypal.me/skgleba (thx! ^_^)
 
 # Useful notes:
- You can not use unofficial enso mods that are not listed as "supported".
- I recommend doing a backup of pd0:
- The 300MB storage is in reality a ~250MB one.
- For any serious issue/help DM me on twitter (twitter.com/skgleba)
