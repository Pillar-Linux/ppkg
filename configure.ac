#Extract values from META file
AC_INIT(m4_esyscmd(grep ^Name: META | cut -d ':' -f 2 | tr -d ' \n'),
        m4_esyscmd(grep ^Version: META | cut -d ':' -f 2 | tr -d ' \n'))

AM_INIT_AUTOMAKE([subdir-objects])

AC_LANG(C)

AC_PROG_CC

AC_CONFIG_FILES([Makefile])

#check headers
AC_CHECK_HEADERS([dirent.h], [], [AC_MSG_ERROR([missing dirent.h])])
AC_CHECK_HEADERS([getopt.h], [], [AC_MSG_ERROR([missing getopt.h])])
AC_CHECK_HEADERS([limits.h], [], [AC_MSG_ERROR([missing limits.h])])
AC_CHECK_HEADERS([stdio.h], [], [AC_MSG_ERROR([missing stdio.h])])
AC_CHECK_HEADERS([stdlib.h], [], [AC_MSG_ERROR([missing stdlib.h])])
AC_CHECK_HEADERS([string.h], [], [AC_MSG_ERROR([missing string.h])])
AC_CHECK_HEADERS([sys/stat.h], [], [AC_MSG_ERROR([missing sys/stat.h])])
AC_CHECK_HEADERS([sys/types.h], [], [AC_MSG_ERROR([missing sys/types.h])])
AC_CHECK_HEADERS([unistd.h], [], [AC_MSG_ERROR([missing unistd.h])])
AC_CHECK_HEADERS([toml.h], [], [AC_MSG_ERROR([missing toml.h])])

#check libraries
AC_CHECK_FILE([/usr/local/lib/libtoml.a], [], [AC_MSG_ERROR([missing libtoml.a])])

#check programs
AC_CHECK_PROG(GIT, [git], [git], [AC_MSG_ERROR([missing git])])
AC_CHECK_PROG(SVN, [svn], [svn], [AC_MSG_ERROR([missing svn])])
AC_CHECK_PROG(CURL, [curl], [curl], [AC_MSG_ERROR([missing curl])])

#extractors
AC_CHECK_PROG(TAR, [tar], [tar], [AC_MSG_ERROR([missing tar])])
AC_CHECK_PROG(BSDTAR, [bsdtar], [bsdtar], [AC_MSG_ERROR([missing bsdtar])])

AC_OUTPUT
