TITLE_ID = SKGIMCULK
TARGET   = IMCUnlockv3
OBJS     = main.o utils.o

LIBS = -lvita2d -lSceDisplay_stub -lSceLibKernel_stub -lSceAppMgr_stub -lSceGxm_stub -lSceIofilemgr_stub \
	-lSceSysmodule_stub -ltaihen_stub -lSceCtrl_stub -lScePgf_stub -lScePvf_stub -lSceShellSvc_stub \
	-lSceCommonDialog_stub -lScePower_stub -lSceVshBridge_stub -lfreetype -lpng -ljpeg -lz -lm -lc

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -fno-lto
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "IMCUnlock" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
    -a sce_sys/icon0.png=sce_sys/icon0.png \
    -a kp/inarw.skprx=kp \
    -a o/0000.img=o/0000.img \
    -a o/0100.img=o/0100.img \
    -a o/0300.img=o/0300.img \
    -a o/1000.img=o/1000.img \
    -a o/1500.img=o/1500.img \
    -a o/2500.img=o/2500.img \
    -a o/2800.img=o/2800.img \
    -a o/pd0h.img=o/pd0h.img \
    -a o/pr0h.img=o/pr0h.img \
    -a o/ud0h.img=o/ud0h.img \
    -a o/rd0h.img=o/rd0h.img \
    -a p/0000.img=p/0000.img \
    -a p/0100.img=p/0100.img \
    -a p/0300.img=p/0300.img \
    -a p/1000.img=p/1000.img \
    -a p/1500.img=p/1500.img \
    -a p/2500.img=p/2500.img \
    -a p/2800.img=p/2800.img \
    -a p/pd0h.img=p/pd0h.img \
    -a p/pr0h.img=p/pr0h.img \
    -a p/ud0h.img=p/ud0h.img \
    -a p/rd0h.img=p/rd0h.img \
    -a q/0000.img=q/0000.img \
    -a q/0100.img=q/0100.img \
    -a q/0300.img=q/0300.img \
    -a q/1000.img=q/1000.img \
    -a q/1500.img=q/1500.img \
    -a q/2500.img=q/2500.img \
    -a q/2800.img=q/2800.img \
    -a q/pd0h.img=q/pd0h.img \
    -a q/pr0h.img=q/pr0h.img \
    -a q/ud0h.img=q/ud0h.img \
    -a q/rd0h.img=q/rd0h.img \
    -a zsb.img=zsb.img \
    -a sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
    -a sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \$@

eboot.bin: $(TARGET).velf
	vita-make-fself -c $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo

vpksend: $(TARGET).vpk
	curl -T $(TARGET).vpk ftp://$(PSVITAIP):1337/ux0:/
	@echo "Sent."

send: eboot.bin
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	@echo "Sent."
