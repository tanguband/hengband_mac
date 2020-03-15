## Makefile.am -- Process this file with automake to process Makefile.in

AUTOMAKE_OPTIONS = foreign subdir-objects nostdinc

bin_PROGRAMS = hengband

hengband_SOURCES = \
	artifact.c artifact.h \
	\
	angband.h autopick.c autopick.h\
	\
	avatar.h avatar.c birth.c birth.h \
	\
	bldg.c bldg.h chest.c chest.h chuukei.c chuukei.h \
	\
	view/display-characteristic.c view/display-characteristic.h \
	view/display-player-stat-info.c view/display-player-stat-info.h \
	view/display-player-misc-info.c view/display-player-misc-info.h \
	view/display-player-middle.c view/display-player-middle.h \
	view/display-player.c view/display-player.h \
	view/status-first-page.c view/status-first-page.h \
	view/display-util.c view/display-util.h \
	\
	player/process-death.c player/process-death.h \
	player/race-resistances.c player/race-resistances.h \
	player/permanent-resistances.c player/permanent-resistances.h \
	player/temporary-resistances.c player/temporary-resistances.h \
	player/process-name.c player/process-name.h \
	\
	io/gf-descriptions.c io/gf-descriptions.h \
	io/tokenizer.c io/tokenizer.h \
	io/interpret-pref-file.c io/interpret-pref-file.h \
	io/player-status-dump.c io/player-status-dump.h \
	io/special-class-dump.c io/special-class-dump.h \
	io/read-pref-file.c io/read-pref-file.h \
	io/exit-panic.c io/exit-panic.h \
	io/write-diary.c io/write-diary.h \
	signal-handlers.c signal-handlers.h uid-checker.c uid-checker.h \
	character-dump.c character-dump.h core.c core.h files.c files.h \
	core/show-file.c core/show-file.h \
	\
	dungeon.c dungeon.h \
	floor.c floor.h floor-events.c floor-events.h \
	floor-generate.c floor-generate.h \
	floor-save.h floor-save.c floor-town.h floor-town.c \
	floor-streams.c floor-streams.h \
	\
	feature.c feature.h \
	geometry.c geometry.h \
	\
	gamevalue.h \
	grid.c grid.h h-basic.h h-config.h h-define.h \
	history.h history.c \
	h-system.h h-type.h inet.c inet.h init.c init.h \
	japanese.c japanese.h english.c english.h \
	load.c load.h \
	dungeon-file.c dungeon-file.h \
	main.c main-x11.c main-gcu.c \
	mind.c mind.h \
	\
	monster.h monsterrace.c monsterrace.h \
	monster/monster-util.c monster/monster-util.h \
	monster/monster-direction.c monster/monster-direction.h \
	monster/monster-attack.c monster/monster-attack.h \
	monster/monster-move.c monster/monster-move.h \
	monster/monster-object.c monster/monster-object.h \
	monster/monster-runaway.c monster/monster-runaway.h \
	monster/monster-safety-hiding.c monster/monster-safety-hiding.h \
	monster/monster-sweep-grid.c monster/monster-sweep-grid.h \
	monster/monster-update.c monster/monster-update.h \
	monster/quantum-effect.c monster/quantum-effect.h \
	monsterrace-hook.c monsterrace-hook.h \
	monster-dist-offsets.c monster-dist-offsets.h \
	monster-process.c monster-process.h \
	monster-status.c monster-status.h monster1.c monster2.c \
	mspells1.c mspells2.c mspells3.c mspells4.c monster-spell.h \
	\
	mutation.h mutation.c \
	\
	object-boost.c object-boost.h \
	object-curse.c object-curse.h object-broken.c object-broken.h \
	object-hook.c object-hook.h object1.c object2.c object.h \
	objectkind.c objectkind.h objectkind-hook.c objectkind-hook.h \
	object-flavor.c object-flavor.h object-ego.c object-ego.h \
	\
	patron.h patron.c \
	\
	creature.h creature.c \
	player-move.c player-move.h player-damage.c player-damage.h \
	horror-descriptions.c horror-descriptions.h \
	player-status.c player-status.h player-effects.c player-effects.h \
	player-skill.c player-skill.h player-class.c player-class.h \
	player-race.c player-race.h player-personality.c player-personality.h \
	player-sex.c player-sex.h player-inventory.c player-inventory.h \
	\
	quest.h quest.c\
	\
	racial.c racial.h \
	realm.c realm.h \
	realm-arcane.c realm-arcane.h realm-chaos.c realm-chaos.h \
	realm-craft.c realm-craft.h realm-crusade.c realm-crusade.h \
	realm-daemon.c realm-daemon.h realm-death.c realm-death.h \
	realm-hex.c realm-hex.h realm-hissatsu.c realm-hissatsu.h \
	realm-life.c realm-life.h realm-nature.c realm-nature.h realm-song.c \
	realm-song.h realm-sorcery.c realm-sorcery.h realm-trump.c realm-trump.h \
	report.c report.h rooms.c rooms-city.c rooms-city.h \
	rooms-fractal.c rooms-fractal.h \
	rooms.h rooms-normal.c rooms-normal.h rooms-pitnest.c rooms-pitnest.h \
	rooms-special.c rooms-special.h rooms-trap.c rooms-trap.h \
	rooms-vault.c rooms-vault.h \
	save.c save.h scores.c scores.h selfinfo.c selfinfo.h \
	snipe.c snipe.h \
	\
	rumor.h rumor.c \
	\
	spells.h \
	spells1.c spells2.c spells3.c \
	spells-diceroll.c spells-diceroll.h \
	spells-floor.c spells-floor.h \
	spells-object.c spells-object.h \
	spells-status.c spells-status.h \
	spells-summon.c spells-summon.h \
	spells-world.c spells-world.h \
	\
	sort.c sort.h \
	store.h store.c term.c term.h trap.c trap.h util.c util.h \
	wild.h wild.c wizard1.c wizard2.c \
	warning.c warning.h world.c world.h \
	\
	view-mainwindow.c view-mainwindow.h targeting.c targeting.h \
	\
	z-form.c z-form.h z-rand.c z-rand.h z-term.c z-term.h z-util.c z-util.h \
	z-virt.c z-virt.h \
	\
	cmd/feeling-table.c cmd/feeling-table.h \
	cmd/monster-group-table.c cmd/monster-group-table.h \
	cmd/diary-subtitle-table.c cmd/diary-subtitle-table.h \
	cmd/object-group-table.c cmd/object-group-table.h \
	cmd/cmd-activate.c cmd/cmd-activate.h cmd/cmd-basic.c cmd/cmd-basic.h \
	cmd/cmd-dump.c cmd/cmd-dump.h cmd/cmd-eat.c cmd/cmd-eat.h \
	cmd/cmd-gameoption.c cmd/cmd-gameoption.h cmd/cmd-help.c cmd/cmd-help.h \
	cmd/cmd-hissatsu.c cmd/cmd-hissatsu.h cmd/cmd-item.c cmd/cmd-item.h \
	cmd/cmd-magiceat.c cmd/cmd-magiceat.h cmd/cmd-mane.c cmd/cmd-mane.h \
	cmd/cmd-pet.c cmd/cmd-pet.h cmd/cmd-quaff.c cmd/cmd-quaff.h \
	cmd/cmd-read.c cmd/cmd-read.h cmd/cmd-save.c cmd/cmd-save.h \
	cmd/cmd-smith.c cmd/cmd-smith.h cmd/cmd-spell.c cmd/cmd-spell.h \
	cmd/cmd-usestaff.c cmd/cmd-usestaff.h cmd/cmd-zaprod.c cmd/cmd-zaprod.h \
	cmd/cmd-zapwand.c cmd/cmd-zapwand.h \
	\
	combat/melee.h combat/melee1.c combat/shoot.c combat/shoot.h

EXTRA_hengband_SOURCES = \
	angband.ico angband.rc ang_eng.rc maid-x11.c main-win.c \
	main-cap.c main-xaw.c \
	makefile.bcc makefile.std \
	readdib.c readdib.h wall.bmp

cocoa_xcode_files = \
	cocoa/AppDelegate.m \
	cocoa/Base.lproj/MainMenu.xib
cocoa_icon_files = \
	cocoa/hengband_Icons.icns \
	cocoa/Save.icns \
	cocoa/Edit.icns \
	cocoa/Data.icns
cocoa_plist_template = cocoa/Angband-Cocoa.xml
cocoa_plist_strings_template = cocoa/Angband-Cocoa.strings
cocoa_plist_files = \
	cocoa/CommandMenu.plist
cocoa_en_nib_files = \
	cocoa/Base.lproj/MainMenu.nib
cocoa_en_strings_files = \
	cocoa/en.lproj/Localizable.strings \
	cocoa/en.lproj/CommandMenu.strings \
	cocoa/en.lproj/GraphicsMenu.strings
cocoa_ja_strings_files = \
	cocoa/ja.lproj/MainMenu.strings \
	cocoa/ja.lproj/Localizable.strings \
	cocoa/ja.lproj/CommandMenu.strings \
	cocoa/ja.lproj/GraphicsMenu.strings

EXTRA_DIST = \
	gcc-wrap \
	$(cocoa_xcode_files) \
	$(cocoa_icon_files) \
	$(cocoa_plist_template) \
	$(cocoa_plist_strings_template) \
	$(cocoa_plist_files) \
	$(cocoa_en_nib_files) \
	$(cocoa_en_strings_files) \
	$(cocoa_ja_strings_files)

if COCOA
hengband_SOURCES += main-cocoa.m grafmode.h grafmode.c cocoa/AppDelegate.h
AM_CFLAGS = -mmacosx-version-min=10.8 -Wunguarded-availability
AM_OBJCFLAGS = -fobjc-arc -mmacosx-version-min=10.8 -Wunguarded-availability
hengband_LDFLAGS = -framework cocoa $(AM_LDFLAGS)
hengband_LINK = MACOSX_DEPLOYMENT_TARGET=10.8 $(OBJCLINK) $(hengband_LDFLAGS) $(LDFLAGS) -o $@
APPNAME = $(PACKAGE_NAME)
APPEXE = hengband
APPDIR = $(APPNAME).app
BUNDLE_IDENTIFIER = jp.osdn.hengband
BUNDLE_VERSION = $(PACKAGE_VERSION)
BUNDLE_DISPLAY_NAME = $(APPNAME)
BUNDLE_NAME = $(BUNDLE_DISPLAY_NAME)
BUNDLE_DISPLAY_NAME_JA = 変愚蛮怒
BUNDLE_NAME_JA = $(BUNDLE_DISPLAY_NAME_JA)
# Be careful with characters (like '&') in the copyright that have special
# meanings to sed.
COPYRIGHT = Copyright © Mr. Hoge and many others
# For now, using the line that appears in news_j.txt.  Is that appropriate?
# The ampersand is a kanji so it doesn't seem to be treated as a special
# character by sed.
COPYRIGHT_JA = Mr.hoge (echizen＠users.sourceforge.jp) ＆ 多くの方々
APPBNDL = $(bindir)/$(APPDIR)
APPCONT = $(APPBNDL)/Contents
APPBIN = $(APPCONT)/MacOS
APPRES = $(APPCONT)/Resources
appbin_PROGRAMS = $(APPEXE)
appbindir = $(APPBIN)
dist_appicon_DATA = $(cocoa_icon_files)
appicondir = $(APPRES)
appplist_DATA = $(cocoa_plist_files)
appplistdir = $(APPRES)
appennib_DATA = $(cocoa_en_nib_files)
appennibdir = $(APPRES)/Base.lproj
appen_DATA = $(cocoa_en_strings_files)
appendir = $(APPRES)/en.lproj
appja_DATA = $(cocoa_ja_strings_files)
appjadir = $(APPRES)/ja.lproj
else
EXTRA_hengband_SOURCES += main-cocoa.m grafmode.h grafmode.c \
	cocoa/AppDelegate.h
hengband_LINK = $(LINK)
endif

DEFAULT_INCLUDES = -I$(srcdir) \
	-I$(srcdir)/cmd \
	-I$(srcdir)/combat \
	-I$(srcdir)/core \
	-I$(srcdir)/io \
	-I$(srcdir)/monster \
	-I$(srcdir)/player \
	-I$(srcdir)/view \
	-I$(top_builddir)/src
CFLAGS += $(XFT_CFLAGS)
LIBS += $(XFT_LIBS)
COMPILE = $(srcdir)/gcc-wrap $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
OBJCCOMPILE = $(srcdir)/gcc-wrap $(OBJC) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_OBJCFLAGS) $(OBJCFLAGS)

install-exec-hook:
if SET_GID
	chgrp "@GAMEGROUP@" "$(DESTDIR)$(bindir)/hengband"
	chmod g+s "$(DESTDIR)$(bindir)/hengband"
endif

if COCOA
# Suppress the warnings from the EUC-JP encoded strings used when building
# the Japanese version.
AM_CFLAGS += -Wno-invalid-source-encoding
AM_OBJCFLAGS += -Wno-invalid-source-encoding

# Create lib/info within the application bundle since not currently
# installing anything to it.  Other relevant lib directories should
# be created from the Makefiles in the lib directory.
install-data-local:
	@mkdir -p $(APPCONT)
	@mkdir -p $(appendir)
	@mkdir -p $(appjadir)
	@mkdir -p $(APPRES)/lib/info
	sed -e 's/\$$VERSION\$$/$(PACKAGE_VERSION)/' \
		-e 's/\$$COPYRIGHT\$$/$(COPYRIGHT)/' \
		-e 's/\$$NAME\$$/$(APPNAME)/' \
		-e 's/\$$EXECUTABLE\$$/$(APPEXE)/' \
		-e 's/\$$BUNDLE_VERSION\$$/$(BUNDLE_VERSION)/' \
		-e 's/\$$BUNDLE_IDENTIFIER\$$/$(BUNDLE_IDENTIFIER)/' \
		-e 's/\$$BUNDLE_DISPLAY_NAME\$$/$(BUNDLE_DISPLAY_NAME)/' \
		-e 's/\$$BUNDLE_NAME\$$/$(BUNDLE_NAME)/' \
		$(srcdir)/$(cocoa_plist_template) > $(APPCONT)/Info.plist
	sed -e 's/\$$VERSION\$$/$(PACKAGE_VERSION)/' \
		-e 's/\$$COPYRIGHT\$$/$(COPYRIGHT)/' \
		-e 's/\$$BUNDLE_DISPLAY_NAME\$$/$(BUNDLE_DISPLAY_NAME)/' \
		-e 's/\$$BUNDLE_NAME\$$/$(BUNDLE_NAME)/' \
		$(srcdir)/$(cocoa_plist_strings_template) > $(appendir)/InfoPlist.strings
	sed -e 's/\$$VERSION\$$/$(PACKAGE_VERSION)/' \
		-e 's/\$$COPYRIGHT\$$/$(COPYRIGHT_JA)/' \
		-e 's/\$$BUNDLE_DISPLAY_NAME\$$/$(BUNDLE_DISPLAY_NAME_JA)/' \
		-e 's/\$$BUNDLE_NAME\$$/$(BUNDLE_NAME_JA)/' \
		$(srcdir)/$(cocoa_plist_strings_template) > $(appjadir)/InfoPlist.strings

distclean-local:
	-rm $(APPCONT)/Info.plist
	-rm $(appendir)/InfoPlist.strings
	-rm $(appjadir)/InfoPlist.strings

endif
