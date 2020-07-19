# IMCUnlock
This mod allows you to manage the internal user partitions on the PSP2 by modifying EMMC's Master Boot Record.

![ref0](https://github.com/SKGleba/IMCUnlock/raw/master/ss/installer.jpg)


# Installation:
1) Download and install the provided VPK file.

2) Make a backup of all important files in system's user partitions (ur0/vd0/tm0).

3) Open IMCUnlock, accept the agreement, and then choose the option that suits you:
   
   - "100MB ux0 partition" - creates a ~100MB imc partition, requires formatting it via settings or safemode.

   - "Hybrid ur0-pd0 and 2.5GB ux0" - ur0 becomes imc, makes pd0 and ur0 share the same data region (pd0 = ur0).
 
   - "Default 2xxx storage configuration" - a ~1GB imc partition with 1.5GB ur0, requires formatting imc via settings or safemode.

   - "Default 1xxx storage configuration" - no imc, 2.5GB ur0, select ONLY with a memory card inserted.

   - "Dump/Flash the user partition table" - Creates/Flashes a partition table, to use with /pc/mbrtool. 

4) After flashing reboot and format the new partition(s).
![ref1](https://github.com/SKGleba/IMCUnlock/raw/master/ss/fmenu.jpg)

5) Reboot again, the new partition(s) should now be accessible
![ref2](https://github.com/SKGleba/IMCUnlock/raw/master/ss/sysinfo.jpg)

# FAQ:
 - Q: What is the largest possible IMC?
   - A: Around 3GB without breaking the important partitions.
 - Q: Does the new partition stay after firmware update/reinstall?
   - A: Yes, the only way to remove the mod is via the IMCUnlock.
 - Q: Can I update when a new firmware comes out?
   - A: No, you should wait for someone to confirm that it works/or that it breaks the vita.
 - Q: My device is stuck on bootlogo/health warning/spinning circle, help!
   - A: Start in safe mode and restore the system.
 - Q: Why the vita does a soft reboot when I remove/insert the mc?
   - A: Patches are applied.
 - Q: Is it backwards-compatible?
   - A: Yes.
 - Q: Is there any way to recover if I bricked the MBR with a customized partition table?
   - A: Yes, but only if you have enso_ex v4+ installed and running.
 
 # Useful notes:
- Only pick the "1XXX" option if you have a sony memory card inserted.
- If you dont know which option you should pick - go for the 100MB one.
- /pc/mbrtool lets advanced users create their own user partition table.
- If you have enso_ex v4+ installed you should be able to recover from any possible lock.
- To have a ~3GB imc create a small hybrid ur0-ud0(-pd0) and give the rest to imc.
- With any serious issues DM me on twitter (twitter.com/skgleba)
