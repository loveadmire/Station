LBNAME = Sm4Andorid
PRGNAME = Sm4Andorid
PKG  = com.hzlz
PKGDIR = com/hzlz

CC = g++
SRC = *.cpp
CFLAGS = -I/home/NDK_ROOT/jdk1.7.0/include -I/home/NDK_ROOT/jdk1.7.0/include/linux -fPIC -D_REENTRANT
LDFLAGS =
all: $(PRGNAME) $(LIBNAME)

$(PRGNAME):
	javac $(PKGDIR)/$(PRGNAME).java
	javah  $(PKG).$(PRGNAME)
	
#$(LIBNAME): *.o
#	$(CC) -shared -o $(LIBNAME).so *.o $(LDFLAGS)

*.o: $(SRC)
#	$(CC) -c $(SRC)  $(CFLAGS)

clean:
	rm -rf *.o *~ $(LIBNAME).so $(PKGDIR)/*.class
	rm -rf com_hzlz_tlsgatway.h 
