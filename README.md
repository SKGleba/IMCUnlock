# IMCUnlock
MBR Magic by SKGleba

![ref0](https://cdn.discordapp.com/attachments/466454495258476545/466463541185216512/IMG_20180711_063013.jpg)

# This guide applies ONLY to Playstation Vita 1XXX models, commonly called "Phat".
If you have a Playstation Vita Slim/TV check out https://github.com/SKGleba/IMCExtend
# Introduction:
This mod allows you to _permanently_ add a internal ux0 storage to a PSVita Phat by modifying boot data regions.

![ref1](https://cdn.discordapp.com/attachments/466454244929699861/466458976901529600/IMG_20180710_180418.jpg)

# Installation:
1) Download and install the provided VPK file. If you are using ENSO NSKBL hack, make sure that the correct enso installer is installed (You can grab one from here: https://github.com/SKGleba/IMCUnlock/tree/master/enso-installers), DO NOT SKIP THAT STEP!.

2) Make a backup of all important ur0 files (they should be safe, but better to be sure)

3) Open IMCUnlock, accept the agreement, navigate to "Options->Backup", and select "Backup vd0". Eventually you may backup pd0/tm0/id_data too. Dumps are saved to ux0:data/%name%.img

4) Disable all unnecessary plugins and reboot.

5) Now, we will install the mod. Open IMCUnlock, accept the agreement, select "Set IMC storage size" and the option that suits you:
   - 0MB - Default on 1XXX units.
   
   - 100MB - Creates a ~100MB imc partition, pd0 is resized to 150MB, requires formatting the new partition via settings or safemode.

   - 300MB - pd0 becomes imc.
 
   - 1GB - Default on 2XXX and TV units, creates a ~1GB imc partition, ur0 is resized to 1.5GB, requires formatting the new partition via settings or safemode.

   - 1.5GB - Creates a ~1.5GB imc partition, ur0 is resized to 1GB, requires formatting the new partition via settings or safemode.

   - 2.5GB - pd0 becomes ur0, and imc takes ur0's place. Remember to backup important data from ur0 before choosing this option.

   - 2.8GB - pd0 becomes ur0, ud0 and ur0 are trimmed to 160MB, a ~2.8GB imc partition is created, requires formatting the new partition via settings or safemode, at reboot enter safemode by holding PS+R+POWER and restore the system (you need to do it only one time). Remember to backup important data from ur0 before choosing this option.

   - Hybrid pd0 and imc - makes pd0 and imc share the same data region (pd0 = imc).

   - Hybrid pd0 and ur0, imc 2.5GB - ur0 becomes imc, makes pd0 and ur0 share the same data region (pd0 = ur0). Remember to backup important data from ur0 before choosing this option.

   - Hybrid ud0 and imc - makes ud0 and imc share the same data region (ud0 = imc).

   - Hybrid ud0 and ur0, imc 2.5GB - ur0 becomes imc, makes ud0 and ur0 share the same data region (ud0 = ur0). Remember to backup important data from ur0 before choosing this option.

6) A prompt asking if you are sure that you want to flash IMCUnlock will appear. After preesing "Yes" the installer will start the flash process.

7) DO NOT TOUCH THE DEVICE UNTIL IT FINISHES

8) After flashing, the following screen should appear:
![ref6](https://cdn.discordapp.com/attachments/466454495258476545/466462385499275274/IMG_20180711_063154.jpg)

9) If you are a user of ENSO NSKBL hack - launch the enso installer and install enso (DO NOT SKIP THAT STEP!), otherwise - reboot.

10) If your vita reboots fine - thats it! You just permanently enabled internal ux0 storage on your phat vita.

# FAQ:
 - Q: What do you mean by "permanent"? Does it stay after fw update/reinstall?
   - A: Yes, the only way to remove the mod is to uninstall via the provided manager.
 - Q: Can I update when a new firmware comes out?
   - A: No, you should wait for someone to confirm that it works/or that it breaks the vita.
 - Q: Everything flashed fine, but i dont have the additional partition.
   - A: Reset tai config, and try again.
 - Q: My device is stuck on bootlogo/health warning/spinning circle, help!
   - A: If enso logo - reinstall the firmware, If sony logo/health warning/spinning circle - remove sony mc, boot to safemode, select "Format Memory Card". Alternatively, system restore may help.
 - Q: Why the vita does a soft reboot when i remove/insert the mc?
   - A: Patches are applied.
 - Q: How can i restore a backupped partition image?
   - A: "Options->Restore"
 - Q: Im getting a critical error at boot, and i cant restore the firmware via safemode.
   - A: Something happened while the installer was running, its a unrecoverable BRICK.
 - Q: You did a great job! How can i help/donate you?
   - A: paypal.me/skgleba (thx! ^_^)
 
 # Useful notes:
- If you dont know which option you should pick - go for the 100MB one.
- You should uninstall IMCUnlock from a external storage device i.e sony mc or sd2vita, because the risk of softlocking the vita is high.
- You can not use unofficial enso mods that are not listed as "supported".
- I recommend doing a backup of pd0:
- The 300MB storage is in reality a ~250MB one.
- For a video "guide"/demonstration check out:
   - https://www.youtube.com/watch?v=UvpwwdN4IFQ
   - https://www.youtube.com/watch?v=eKRxQ_4bJVQ
   - https://www.youtube.com/watch?v=OlMw_JOoeY8
- If you choose a option that removes pd0 you can get back the BGM by using https://github.com/SKGleba/IMCUnlock/tree/master/pd0-spoof
- For any serious issue/help DM me on twitter (twitter.com/skgleba)
