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

#all : data.o entry.o list.o table.o serialization.o client-lib.o table_client.o table_client
all : proto.h-c proto.h-c.o client-lib.o table_client.o table_client

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

#client_stub.o: $(INCLUDE)client_stub-private.h $(INCLUDE)client_stub.h
#	$(FLAG) $(SRC)client_stub.c -o $(OBJ)client_stub.o
		

proto.h-c:
	protoc --c_out=. sdmessage.proto


proto.h-c.o: 
	$(CC) -c sdmessage.pb-c.c


client-lib.o: $(INCLUDE)client_stub-private.h $(INCLUDE)client_stub.h $(INCLUDE)network_client.h $(INCLUDE)data.h $(INCLUDE)entry.h $(INCLUDE)list-private.h $(INCLUDE)list.h $(INCLUDE)table-private.h $(INCLUDE)table.h
	$(FLAG) $(SRC)data.c -o $(OBJ)data.o
	$(FLAG) $(SRC)entry.c -o $(OBJ)entry.o
	$(FLAG) $(SRC)list.c -o $(OBJ)list.o
	$(FLAG) $(SRC)table.c -o $(OBJ)table.o
	$(FLAG) $(SRC)serialization.c -o $(OBJ)serialization.o
	$(FLAG) $(SRC)client_stub.c -o $(OBJ)client_stub.o
	$(FLAG) $(SRC)network_client.c -o $(OBJ)network_client.o
	ld -r $(OBJ)client_stub.o $(OBJ)network_client.o $(OBJ)data.o $(OBJ)entry.o -o $(OBJ)client-lib.o

table_client.o: $(SRC)table_client.c  
	$(FLAG) $(SRC)table_client.c -o $(OBJ)table_client.o

table_client: $(OBJ)client-lib.o $(OBJ)table_client.o proto/sdmessage.pb-c.o
	$(CC) -g $(OBJ)client-lib.o $(OBJ)table_client.o proto/sdmessage.pb-c.o -I/proto -L/home/aluno-di/SD -lprotobuf-c -o $(BIN)table_client

clean:	
	rm sdmessage.pb-c.*
	rm -rf $(BIN)*
	rm $(OBJ)*.o
