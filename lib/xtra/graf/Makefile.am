## Makefile.am -- Process this file with automake to process Makefile.in

angband_files = \
	8x8.bmp \
	16x16.bmp \
	16x16-mask.bmp

angband4_conf = list.txt

EXTRA_DIST = \
  delete.me \
  $(angband_files) \
  $(angband4_conf) \
  osx_bmp2png.py

if SET_GID
angbanddir = @DEFAULT_LIB_PATH@/xtra/graf

angband_DATA = \
  $(angband_files)
endif

if COCOA
# APPNAME to APPRES duplicate what's in src/Makefile.am.  It would be nice to
# avoid that, but until then, they should match.
APPNAME = $(PACKAGE_NAME)
APPDIR = $(APPNAME).app
APPBNDL = $(bindir)/$(APPDIR)
APPCONT = $(APPBNDL)/Contents
APPRES = $(APPCONT)/Resources

appgrafdir = $(APPRES)/lib/xtra/graf
appgraf_DATA = \
	$(angband4_conf)

install-data-local:
	@mkdir -p $(appgrafdir)
	@mkdir -p $(appgrafdir)/old
	python $(srcdir)/osx_bmp2png.py $(srcdir)/8x8.bmp \
		$(appgrafdir)/old/8x8.png
	@mkdir -p $(appgrafdir)/adam-bolt
	python $(srcdir)/osx_bmp2png.py $(srcdir)/16x16.bmp \
		$(appgrafdir)/adam-bolt/16x16.png \
		--mask $(srcdir)/16x16-mask.bmp

distclean-local:
	-rm $(appgrafdir)/old/8x8.png
	-rm $(appgrafdir)/adam-bolt/16x16.png
endif
