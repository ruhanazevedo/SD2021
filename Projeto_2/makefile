#/********* Grupo 9 ********
#* 44898 - José Alves      *
#* 46670 - Tiago Lourenço  *
#* 51779 - Ruhan Azevedo   *
#***************************/

CC=gcc
INCLUDE = include/
OBJ = object/
SRC = source/
BIN = binary/
FLAG = gcc -g -Wall -I include/ -c
PROTO_DIR = proto/
LIB = lib/


all : data.o entry.o list.o table.o serialization.o client_stub.o network_client.o client-lib.o table_client.o table_skel.o network_server.o server-lib.o table_server.o table_client table_server
proto: proto.h-c proto.h-c.o 


data.o: $(INCLUDE)data.h
	$(FLAG) $(SRC)data.c -o $(OBJ)data.o

entry.o: $(INCLUDE)entry.h $(INCLUDE)data.h
	$(FLAG) $(SRC)entry.c -o $(OBJ)entry.o

list.o: $(INCLUDE)list-private.h $(INCLUDE)list.h
	$(FLAG) $(SRC)list.c -o $(OBJ)list.o

table.o: $(INCLUDE)table-private.h $(INCLUDE)table.h $(INCLUDE)data.h $(INCLUDE)list-private.h $(INCLUDE)list.h
	$(FLAG) $(SRC)table.c -o $(OBJ)table.o

serialization.o: $(INCLUDE)data.h $(INCLUDE)entry.h $(INCLUDE)serialization.h
	$(FLAG) $(SRC)serialization.c -o $(OBJ)serialization.o
		

proto.h-c:
	protoc --c_out=./$(INCLUDE) sdmessage.proto

proto.h-c.o: 
	$(FLAG) -c $(INCLUDE)sdmessage.pb-c.c -o $(INCLUDE)sdmessage.pb-c.o


client_stub.o: $(INCLUDE)client_stub.h
	$(FLAG) $(SRC)client_stub.c -o $(OBJ)client_stub.o

network_client.o: $(INCLUDE)network_client.h
	$(FLAG) $(SRC)network_client.c -o $(OBJ)network_client.o


client-lib.o: $(INCLUDE)client_stub-private.h $(INCLUDE)client_stub.h $(INCLUDE)network_client.h $(INCLUDE)data.h $(INCLUDE)entry.h $(INCLUDE)list-private.h $(INCLUDE)list.h $(INCLUDE)table-private.h $(INCLUDE)table.h
	ld -r $(OBJ)client_stub.o $(OBJ)network_client.o $(OBJ)data.o $(OBJ)entry.o -o $(LIB)client-lib.o

table_client.o: $(SRC)table_client.c  
	$(FLAG) $(SRC)table_client.c -o $(OBJ)table_client.o

table_skel.o: $(INCLUDE)table_skel.h 
	$(FLAG) $(SRC)table_skel.c -o $(OBJ)table_skel.o

network_server.o: $(INCLUDE)network_server.h
	$(FLAG) $(SRC)network_server.c -o $(OBJ)network_server.o

server-lib.o: $(INCLUDE)table_skel.h $(INCLUDE)network_server.h 
	ld -r $(OBJ)table_skel.o $(OBJ)network_server.o $(OBJ)table.o $(OBJ)entry.o $(OBJ)data.o -o $(LIB)server-lib.o


table_server.o: $(SRC)table_server.c $(INCLUDE)network_server.h $(INCLUDE)table_skel.h
	$(FLAG) $(SRC)table_server.c -o $(OBJ)table_server.o


table_client: $(LIB)client-lib.o $(OBJ)table_client.o $(INCLUDE)sdmessage.pb-c.o
	$(CC) -g $(LIB)client-lib.o $(OBJ)table_client.o $(INCLUDE)sdmessage.pb-c.o -I$(INCLUDE) -L/usr/local/lib/ -lprotobuf-c -o $(BIN)table_client

table_server: $(LIB)server-lib.o $(OBJ)table_server.o $(INCLUDE)sdmessage.pb-c.o 
	$(CC) -g $(LIB)server-lib.o $(OBJ)table_server.o $(INCLUDE)sdmessage.pb-c.o -I$(INCLUDE) -L/usr/local/lib/ -lprotobuf-c -o $(BIN)table_server 

# delete proto file dependencies
pclean:
	rm $(INCLUDE)sdmessage.pb-c.*
	rm $(INCLUDE)sdmessage.pb-c.*

clean:	
	rm -rf $(BIN)*
	rm $(OBJ)*.o
	rm $(LIB)*.o
