CC = gcc
RM = rm
CFLAGS = -std=c18 -lpthread -Wall -Wconversion -D_XOPEN_SOURCE=500 -Werror -Wextra -Wpedantic -Wwrite-strings -O2
objects_client = client.o
objects_serveur= server.o
executable_client = client
executable_serveur= server

all: $(executable_client) $(executable_serveur) 

$(executable_client): $(objects_client)
	$(CC) $(objects_client) -o $(executable_client)
$(executable_serveur): $(objects_serveur)
	$(CC) $(objects_serveur) -o $(executable_serveur)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(objects_client) $(objects_serveur) $(executable_client) $(executable_serveur)
