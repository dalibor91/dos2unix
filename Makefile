include dos2unix/version.mk

SVNREPO = svn://svn.code.sf.net/p/dos2unix/code
SVNSSHREPO = svn+ssh://svn.code.sf.net/p/dos2unix/code

all: help

# Target: help - Display available make targets
help:
	@awk  '/^# +Target:/ {sub("# +Target: +",""); print}' Makefile | sort

# Documentation files are created and included in the source
# package for users that don't have the proper tools installed.
# Documentation creation only works properly on Cygwin or
# a modern Linux distribution.


RELEASE_DIR_DOS2UNIX = dos2unix-$(DOS2UNIX_VERSION)
RELEASE_DIR_D2U = d2u$(DOS2UNIX_VERSION_SHORT)

# Target: dist - Create source code distribution packages
dist:
	rm -rf ../${RELEASE_DIR_DOS2UNIX}
	rm -rf ../${RELEASE_DIR_D2U}
	svn export ${SVNREPO}/trunk/dos2unix ../${RELEASE_DIR_DOS2UNIX}
	# Include doc files, to make it easier to build dos2unix.
	cd ../${RELEASE_DIR_DOS2UNIX} ; $(MAKE) man txt html
	# Make sure .po files are up to date.
	cd ../${RELEASE_DIR_DOS2UNIX} ; $(MAKE) merge
	# cleanup.
	cd ../${RELEASE_DIR_DOS2UNIX} ; $(MAKE) mostlyclean
	# fix some file mods.
	cd ../${RELEASE_DIR_DOS2UNIX} ; chmod -x Makefile *.txt *.c *.h *.mak *.mk po/*.* man/nl/man1/*.*
	# Create DOS source package.
	cd .. ; cp -Rp $(RELEASE_DIR_DOS2UNIX) $(RELEASE_DIR_D2U)
	# DJGPP's GNU make 3.79 has trouble with makefiles in DOS format.
	cd ../$(RELEASE_DIR_D2U) ; unix2dos --keepdate w*.mak *.txt *.c *.h
	cd ../$(RELEASE_DIR_D2U) ; unix2dos --keepdate --add-bom ChangeLog.txt README.txt
	cd ../$(RELEASE_DIR_D2U) ; unix2dos --keepdate man/man1/*.txt  man/*/man1/*.txt  $(wildcard man/nonlatin/*/man1/*.txt)
	cd ../$(RELEASE_DIR_D2U) ; unix2dos --keepdate man/man1/*.htm* man/*/man1/*.htm* $(wildcard man/nonlatin/*/man1/*.htm*)
	# Create doc package for people who are not able to create it.
	cd .. ; tar cvzf ${RELEASE_DIR_DOS2UNIX}-doc.tar.gz \
		${RELEASE_DIR_DOS2UNIX}/man/man1/*.txt \
		${RELEASE_DIR_DOS2UNIX}/man/man1/*.htm* \
		${RELEASE_DIR_DOS2UNIX}/man/man1/*.1 \
		${RELEASE_DIR_DOS2UNIX}/man/*/man1/*.txt \
		${RELEASE_DIR_DOS2UNIX}/man/*/man1/*.htm* \
		${RELEASE_DIR_DOS2UNIX}/man/*/man1/*.1 \
		$(wildcard ${RELEASE_DIR_DOS2UNIX}/man/nonlatin/*/man1/*.txt) \
		$(wildcard ${RELEASE_DIR_DOS2UNIX}/man/nonlatin/*/man1/*.htm*) \
		$(wildcard ${RELEASE_DIR_DOS2UNIX}/man/nonlatin/*/man1/*.1)
	# Remove generated doc files from Unix package.
	cd ../${RELEASE_DIR_DOS2UNIX} ; $(MAKE) clean
	# Create the package.
	cd .. ; tar cvzf ${RELEASE_DIR_DOS2UNIX}.tar.gz ${RELEASE_DIR_DOS2UNIX}
	cd .. ; rm -f ${RELEASE_DIR_D2U}s.zip
	cd .. ; zip -r ${RELEASE_DIR_D2U}s.zip ${RELEASE_DIR_D2U}

# Target: tag - Create a tag copy of trunk
tag:
	svn copy ${SVNSSHREPO}/trunk \
	         ${SVNSSHREPO}/tags/dos2unix-${DOS2UNIX_VERSION} \
	    -m "Tagging dos2unix release ${DOS2UNIX_VERSION}."

BRANCH=dos2unix-60

branch:
	svn copy ${SVNSSHREPO}/trunk \
	         ${SVNSSHREPO}/branches/${BRANCH} \
	    -m "Branching ${BRANCH}."


# Get latest changes of trunk into branch.
merge_from_trunk:
	svn merge ${SVNSSHREPO}/trunk

# Merge branch into trunk.
# After this the branch is unusable for further work.
# Copy a new branch if needed.
merge_to_trunk:
	svn merge --reintegrate ${SVNSSHREPO}/branches/${BRANCH}

# Delete branch
delete_branch:
	svn delete ${SVNSSHREPO}/branches/${BRANCH} -m "Delete branch ${BRANCH}"


