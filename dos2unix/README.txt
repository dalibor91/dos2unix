
dos2unix - DOS/Mac to Unix and vice versa text file format converter.


FILE LIST

        README.txt      : This file.
        INSTALL.txt     : How to build and install.
        COPYING.txt     : distribution license.
        NEWS.txt        : Basic change log for users.
        ChangeLog.txt   : Detailed change log for programmers.
        TODO.txt        : Things to do.
        BUGS.txt        : Known bugs and instructions on reporting new ones.
        dos2unix.txt    : Dos2unix manual, text format.
        dos2unix.ps     : Dos2unix manual, PostScript format.
        dos2unix.pdf    : Dos2unix manual, PDF format.
        dos2unix.htm    : Dos2unix manual, HTML format.


HISTORY

        This is an update of Benjamin Lin's implementations of dos2unix and
        unix2dos.  Benjamin Lin's implementations of dos2unix and unix2dos are
        part of several Linux distributions such as RedHat, Fedora, Suse and
        others.  This update includes all RedHat patches and fixes several
        other problems.  Internationalization has been added and ports to DOS,
        Windows, Cygwin and OS/2 Warp have been made.

        These implementations of dos2unix and unix2dos have been modelled after
        dos2unix/unix2dos under SunOS/Solaris.  They have similar conversion
        modes, namely ascii, 7bit and iso.  The first versions were made by
        John Birchfield in 1989, and in 1995 rewritten from scratch by Benjamin
        Lin. Mac to Unix conversion was added by Bernd Johannes Wuebben in
        1998, Unix to Mac conversion by Erwin Waterlander in 2010.

        Features

        * Native language support: Dutch, English, Esperanto, German, Russian,
          Spanish.
        * Automatically skips binary and non-regular files
        * In-place, paired, or stdio mode conversion.
        * Keep original file dates option.
        * 7-bit and iso conversion modes like SunOS dos2unix.
        * Conversion of Windows UTF-16 files to Unix UTF-8.
        * Handles Unicode Byte Order Mark (BOM)
        * Secure.


AUTHORS

        Erwin Waterlander       version 3.2-6.0.4  2009-2013
        Christian Wurll         version 3.1        1998
        Bernd Johannes Wuebben  version 3.0        1998
        Benjamin Lin            version 1.1-2.3    1994-1995
        John Birchfield         version 1.0        1989

TRANSLATORS

        Translation of the messages:

        Since verion 6.0.5 dos2unix is part of the Translation Project (TP).
        All translations go via the Translation Project.
        See http://translationproject.org/domain/dos2unix.html

        Brazilian Portuguese  Enrico Nicoletto <Tranlation Project>           Version 6.0.5
        Chinese (traditional) mail6543210 <mail6543210@yahoo.com.tw>          Version 6.0.5
        Danish                Thomas Pryds <thomas@pryds.eu>                  Version 6.0.5
        Dutch                 Erwin Waterlander <waterlan@xs4all.nl>          Version 4.0   - 6.0.4
        Dutch                 Benno Schulenberg <Tranlation Project>          Version 6.0.5
        Esperanto             Rugxulo <rugxulo@gmail.com>                     Version 5.1   - 6.0.4
        Esperanto             Benno Schulenberg <Tranlation Project>          Version 6.0.5
        French                Frédéric Marchal <fmarchal@perso.be>            Version 6.0.5
        German                Philipp Thomas <pth@suse.de>                    Version 5.0   - 6.0.3
        German                Lars Wendler <polynomial-c@gentoo.org>          Version 6.0.4
        Polish                Jakub Bogusz <Tranlation Project>               Version 6.0.5
        Russian               Андрей Углик (Andrei Uhlik) <uglika@gmail.com>  Version 6.0.4
        Serbian               Мирослав Николић <miroslavnikolic@rocketmail.com> Version 6.0.5
        Spanish               Julio A. Freyre-Gonzalez <jfreyreg@gmail.com>   Version 5.3.1 - 6.0.4
        Ukrainian             Yuri Chornoivan <Tranlation Project>            Version 6.0.5
        Vietnamese            Trần Ngọc Quân <Tranlation Project>             Version 6.0.5

        Translation of the manual:

        Dutch                 Erwin Waterlander <waterlan@xs4all.nl>           Version 5.1.1 - 6.0.5
	German                Mario Blättermann <mario.blaettermann@gmail.com> Version 6.0.5
        Spanish               Julio A. Freyre-Gonzalez <jfreyreg@gmail.com>    Version 5.3.1 - 6.0.5


ACKNOWLEDGEMENTS

        Rugxulo               query_con_codepage(), code page detection in DOS.
        Rugxulo               DJGPP stubs for mac2unix and unix2mac.
        Jari Aalto            Improvements man page.
        Jari Aalto            Improvements Makefile.
        Ben Pfaff             Treat form feeds as valid text.
        Marc Gianzero         Darwin OS port.
        Elbert Pol            OS/2 port.
        Tim Waugh             Various patches.
        Mike Harris           Man page update.
        Bernard RosenKraenzer segfault patch.
        Tim Powers            mkstemp patch.
        Miloslav Trmac        safeconv patch.
        Charles Wilson        Follow symbolic links.
        Charles Wilson        Makefile and code cleanup for Cygwin.
        Christopher Williams  Maintain file ownership in old file mode.
        Steven H. Levine      Support wildcard expansion on OS/2.
        Steve Hay             Support wildcard expansion on win64.
        Michael Schindler     Fixed compiler warning.
        Somsak Pattanaprateep Print line number when binary symbol is found.
        Justin Lecher         Makefile improvement.
        F.J. Brandelik        Reported problems when win32 version processes
                              huge files on a network drive.

FINDUTILS

        Windows users who are looking for the Unix 'find' and 'xargs' commands
        can find them in the findutils package for Windows at the ezwinports
        project page at: http://sourceforge.net/projects/ezwinports/files/

        A DOS version of the findutils package can be obtained from the DJGPP
        project (http://www.delorie.com/djgpp/) at
        ftp://ftp.delorie.com/pub/djgpp/current/v2gnu/

CONTACT INFORMATION

        Project page             : http://waterlan.home.xs4all.nl/dos2unix.html
        SourceForge project page : http://sourceforge.net/projects/dos2unix/
        Freecode project page    : http://freecode.com/projects/dos2unix/

        Erwin Waterlander
        waterlan@xs4all.nl
        http://waterlan.home.xs4all.nl/

