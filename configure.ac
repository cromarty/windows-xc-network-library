AC_PREREQ([2.69])
AC_INIT([mynetlib],[1.0.0],[mike.ray@btinternet.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_SRCDIR([src/wins.c])
AC_CONFIG_MACRO_DIR([m4])

dnl *************************************************
dnl current:revision:age
dnl change (without API) = c:r+1:a
dnl change API = c+1:0:a
dnl add API = c+1:0:a+1
dnl remove API = c+1:0:0
dnl *************************************************


eval LIBTOOL_VERSION_INFO="1:0:0"
AC_SUBST(LIBTOOL_VERSION_INFO)

AC_DISABLE_STATIC

dnl Test for programs

AC_PROG_CC
AM_PROG_AR
AC_PROG_MAKE_SET

LT_INIT

dnl Check for headers

AC_CONFIG_HEADERS([config.h])


AC_CHECK_HEADERS([windows.h],[],[AC_MSG_ERROR([could not find windows.h'])])
AC_CHECK_HEADERS([windowsx.h],[],[AC_MSG_ERROR([could not find windowsx.h'])])
AC_CHECK_HEADERS([winsock2.h],[],[AC_MSG_ERROR([Could not find winsock2.h'])])

dnl Check for libraries

AC_CHECK_LIB([wsock32], [gethostbyname], [], [AC_MSG_ERROR([WSAStartup was not found in libmswsock])])
#AC_SEARCH_LIBS([gethostbyname], [mswsock],[],[Did not find gethostbyname])


dnl Do not build static and shared libraries together

if test "$enable_static" = "$enable_shared" -a "$enable_static" = "yes"; then
cat <<EOF
  Please, do not try to compile static and shared libraries together.
  See INSTALL file for more details (do not use --enable-shared=yes with
  --enable-static=yes).
EOF
  exit 1
fi

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_OUTPUT
