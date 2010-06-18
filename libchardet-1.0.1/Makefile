# $Id: Makefile.in,v 1.1.1.1 2009/02/21 20:08:02 oops Exp $
srcdir          = .
top_srcdir      = .
builddir        = .
top_builddir    = .

build           = i686-pc-linux-gnu
build_cpu       = i686
build_vendor    = pc
build_os        = linux-gnu
host            = i686-pc-linux-gnu
host_cpu        = i686
host_vendor     = pc

prefix          = /usr/local
exec_prefix     = ${prefix}
mandir          = ${prefix}/man

PROG            = chardet
VERSION         = 1.0.1
RM              = /bin/rm -f
MKDIR           = /bin/mkdir -p
INSTALL         = /bin/install -c

all: libs

libs:
	$(MAKE) -C src

install: install-man
	$(MAKE) -C src install

install-man:
	$(MKDIR) $(DESTDIR)$(mandir)/ko/man3
	for i in `ls man/ko/*`; do \
		$(INSTALL) -m644 $$i $(DESTDIR)$(mandir)/ko/man3/; \
	done

clean:
	$(MAKE) -C src clean

distclean: clean
	-$(RM) -r libtool config.log config.status Makefile $(PROG)-config.h *~
	-$(RM) -r src/{$(PROG)-config*,*~,Makefile}
