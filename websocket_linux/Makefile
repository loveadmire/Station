#build web_socket server and client
NAME_FUN1 = server
NAME_FUN2 = client

TARGETS = $(NAME_FUN1) $(NAME_FUN2)

CC=gcc
LIB_PATH =  -L ./lib/
LDFLAG= -lpthread
INC=-I ./

all:$(TARGETS)
$(NAME_FUN1):server_main.c
	$(CC) -g  -o $(NAME_FUN1) server_main.c websocket_common.c  $(INC) $(LIB_PATH) $(LDFLAG)

$(NAME_FUN2):client_main.c
	$(CC) -g  -o $(NAME_FUN2) client_main.c websocket_common.c  $(INC) $(LIB_PATH) $(LDFLAG)
clean:
	rm -rf  $(NAME_FUN1) $(NAME_FUN2)
