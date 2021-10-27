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

all : data.o entry.o list.o table.o serialization.o client_stub.o

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

client_stub.o: $(INCLUDE)client_stub-private.h $(INCLUDE)client_stub.h
	$(FLAG) $(SRC)client_stub.c -o $(OBJ)client_stub.o

client-lib.o: $(INCLUDE)client_stub-private.h $(INCLUDE)client_stub.h $(INCLUDE)network_client.h $(INCLUDE)data.h $(INCLUDE)entry.h
	$(FLAG) $(SRC)client_stub.c -o $(OBJ)client_stub.o
	$(FLAG) $(SRC)network_client.c -o $(OBJ)network_client.o
	ld -r $(OBJ)client_stub.o $(OBJ)network_client.o $(OBJ)data.o $(OBJ)entry.o -o $(OBJ)client-lib.o

table-client: $(OBJ)client_stub.o $(OBJ)network_client.o
	$(CC) -g $(OBJ)main.o $(OBJ)table.o $(OBJ)entry.o -o $(BIN)main	

clean:	
	rm -rf $(BIN)*
	rm $(OBJ)*.o