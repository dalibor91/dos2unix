
prefix=c:/usr/local
ENABLE_NLS=1

ifdef $(ENABLE_NLS)
LDFLAGS_EXTRA = -lintl -liconv
# Using GnuWin32 gettext,iconv
ZIPOBJ_EXTRA = bin/libintl3.dll bin/libiconv2.dll
# Using MinGW gettext,iconv
#ZIPOBJ_EXTRA = bin/libintl-8.dll bin/libiconv-2.dll
endif

all:
	$(MAKE) all EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK="cp -f"

install:
	$(MAKE) install EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK="cp -f"

uninstall:
	$(MAKE) uninstall EXE=.exe prefix=$(prefix)

clean:
	$(MAKE) clean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

dist:
	$(MAKE) dist-zip EXE=.exe prefix=$(prefix) VERSIONSUFFIX="-win32" ZIPOBJ_EXTRA="${ZIPOBJ_EXTRA}" ENABLE_NLS=$(ENABLE_NLS)

