kern := $(shell uname -s)
os := $(shell if test $(kern) = "Darwin"; then echo "osx"; elif test $(kern) = "Linux"; then echo "linux"; else echo "unknown"; fi)
make_f := make -f Makefile.$(os)

vlc-plugins:
	$(make_f) vlc-plugins

clean:
	$(make_f) clean
