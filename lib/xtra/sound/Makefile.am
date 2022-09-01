## Makefile.am -- Process this file with automake to process Makefile.in

# unused_sound_files has files that are not mentioned in sound.cfg.
EXTRA_DIST = \
	readme.txt \
	sound.cfg \
	$(sound_files) \
	$(unused_sound_files)

sound_files = \
	bite.wav \
	breath.wav \
	buy.wav \
	claw.wav \
	critical.wav \
	crush.wav \
	damage_over_time.wav \
	destitem.wav \
	down.wav \
	drop1.wav \
	drop2.wav \
	eat1.wav \
	eat2.wav \
	eat3.wav \
	explode.wav \
	evil.wav \
	fail.wav \
	fall.wav \
	flee.wav \
	glass.wav \
	hit1.wav \
	hit2.wav \
	hit3.wav \
	hitwall1.wav \
	hitwall2.wav \
	hitwall3.wav \
	hungry1.wav \
	hungry2.wav \
	kill1.wav \
	kill2.wav \
	level.wav \
	miss1.wav \
	miss2.wav \
	miss3.wav \
	moan.wav \
	n_kill.wav \
	opendoor1.wav \
	opendoor2.wav \
	opendoor3.wav \
	pain.wav \
	quaff.wav \
	reflect.wav \
	scroll.wav \
	sell.wav \
	shoot.wav \
	shoot_hit1.wav \
	shoot_hit2.wav \
	show.wav \
	shutdoor1.wav \
	shutdoor2.wav \
	shutdoor3.wav \
	slash.wav \
	slime.wav \
	stairway1.wav \
	stairway2.wav \
	sting.wav \
	store1.wav \
	store2.wav \
	study.wav \
	summon.wav \
	take_off.wav \
	teleport.wav \
	terrain_damage.wav \
	touch.wav \
	tplevel.wav \
	tpother.wav \
	wail.wav \
	warn.wav \
	wield.wav \
	zap.wav

unused_sound_files = \
	apple1-r.wav \
	decision25r.wav \
	enemy-advent1-r.wav \
	se_maoudamashii_battle07.wav \
	se_maoudamashii_retro22.wav \
	se_maoudamashii_se_drink01.wav \
	se_maoudamashii_voice_monster01.wav \
	sea-lion2-r.wav \
	sword-clash1-r.wav \
	sword-gesture3-r.wav \
	sword-slash3-r.wav

if COCOA
# APPNAME to APPRES duplicate what's in src/Makefile.am.  It would be nice to
# avoid that, but until then, they should match.
APPNAME = $(PACKAGE_NAME)
APPDIR = $(APPNAME).app
APPBNDL = $(bindir)/$(APPDIR)
APPCONT = $(APPBNDL)/Contents
APPRES = $(APPCONT)/Resources

appsounddir = $(APPRES)/lib/xtra/sound
appsound_DATA = \
	sound.cfg $(sound_files)
endif
