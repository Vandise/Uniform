AC_INIT(uniform, version-0.1)

AC_PROG_CC([gcc])

export LD_LIBRARY_PATH='/usr/local/include:/usr/local/lib:/mingw64/include:/mingw64/lib:/usr/include:/usr/lib'
export CFLAGS='-I/usr/local/lib -I/usr/local/include -I/c/MinGW/msys/1.0/include'

AC_SUBST(CFLAGS, '-I/usr/local/lib -I/usr/local/include -I/mingw64/include -I/usr/include -Iinc')
AC_SUBST(ARCHIVER, 'ar')

AC_CHECK_PROG(GIT,[git],[yes],[no])
test "$GIT" == "no" && AC_MSG_ERROR([Required program 'git' not found.])

function require_awry {
  local install_attempted=$1
  local libpresent=0

  AC_CHECK_HEADERS(awry/awry.h, [libpresent=1], [libpresent=0])

  if test "$libpresent" == 1
  then
    return
  fi

  if test "$libpresent" == 0
  then
    AC_MSG_NOTICE([Awry not found. Attempting to install Awry])
    mkdir .tmp
    cd .tmp/
    git clone https://github.com/Awry-CC/awry
    cd awry && autoconf && ./configure && make && make install
    cd ..
    cd ..
  fi
}

require_awry 0

AC_OUTPUT(Makefile)