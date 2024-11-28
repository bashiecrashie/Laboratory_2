compiler=gcc
client_source=client.c
server_source=server.c
server_build=lab_2_server
client_build=lab_2_client

all: client server

client: $(client_source)
	$(compiler) $(client_source) -o $(client_build)
	cp -v -r $(client_build) /home/mark/Desktop/Shared

server: $(server_source)
	$(compiler) $(server_source) -o $(server_build)

