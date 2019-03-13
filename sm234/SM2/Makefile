CC=gcc -g  $(CFLAGS) -DLINUX -w
SHARED_FLAG = -fPIC -shared

PROJECT = sm2test

INC  = -I./ ../../inc/openssl
OBJ = sm2.o sm2test.o

MODULE =  -lcrypto -ldl -lssl

all : ${OBJ} $(PROJECT)

$(PROJECT) :
	$(CC) ${OBJ} -o $(PROJECT) $(INC) $(MODULE)

clean:
	rm -rf *.o *.a *.so

%.o : %.cpp
	$(CC) $(SHARED_FLAG) -c $< $(INC)
%.o : %.c
	$(CC) $(SHARED_FLAG) -c $< $(INC)
