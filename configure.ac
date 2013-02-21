#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ASP], [0.1], [doug@dougfrazer.com, bmoscon@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([glut], [glutInit], [], AC_MSG_ERROR([libglut is not installed.]))
AC_CHECK_LIB([GL], [glClear], [], AC_MSG_ERROR([libGL is not installed.]))
AC_CHECK_LIB([GLU], [glClear], [], AC_MSG_ERROR([libGLU is not installed.]))
# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([socket strerror])

AC_OUTPUT([Makefile])
