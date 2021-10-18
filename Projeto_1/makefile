#/********* Grupo 9 ********
#* 44898 - José Alves      *
#* 46670 - Tiago Lourenço  *
#* 51779 - Ruhan Azevedo   *
#***************************/

CC=gcc
INCLUDE = Projeto\ 1/include/
OBJ = object/
SRC = source/
BIN = binary/
TEST = test/
FLAG = gcc -g -Wall -I include/ -c

all : data.o entry.o list.o table.o serialization.o test_data.o test_entry.o test_list.o test_table.o test_serialization.o main.o test_data test_entry test_list test_table test_serialization main

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

test_data.o: $(INCLUDE)data.h
	$(FLAG) $(TEST)test_data.c -o $(OBJ)test_data.o

test_entry.o: $(INCLUDE)entry.h $(INCLUDE)data.h
	$(FLAG) $(TEST)test_entry.c -o $(OBJ)test_entry.o

test_list.o: $(INCLUDE)data.h $(INCLUDE)entry.h  $(INCLUDE)list.h 
	$(FLAG) $(TEST)test_list.c -o $(OBJ)test_list.o

test_table.o: $(INCLUDE)table-private.h $(INCLUDE)table.h $(INCLUDE)data.h $(INCLUDE)entry.h
	$(FLAG) $(TEST)test_table.c -o $(OBJ)test_table.o

test_serialization.o: $(INCLUDE)data.h $(INCLUDE)entry.h $(INCLUDE)serialization.h
	$(FLAG) $(TEST)test_serialization.c -o $(OBJ)test_serialization.o

#this is temporary
main.o: $(INCLUDE)table-private.h $(INCLUDE)table.h $(INCLUDE)data.h $(INCLUDE)entry.h
	$(FLAG) $(TEST)main.c -o $(OBJ)main.o

test_data: $(OBJ)test_data.o $(OBJ)data.o
	$(CC) $(OBJ)test_data.o $(OBJ)data.o -o $(BIN)test_data

test_entry: $(OBJ)test_entry.o $(OBJ)data.o $(OBJ)entry.o
	$(CC) $(OBJ)test_entry.o $(OBJ)data.o $(OBJ)entry.o -o $(BIN)test_entry

test_list: $(OBJ)test_list.o $(OBJ)list.o $(OBJ)entry.o
	$(CC) $(OBJ)test_list.o $(OBJ)list.o $(OBJ)entry.o -o $(BIN)test_list

test_table: $(OBJ)test_table.o $(OBJ)table.o $(OBJ)entry.o
	$(CC) -g $(OBJ)test_table.o $(OBJ)table.o $(OBJ)entry.o -o $(BIN)test_table

test_serialization: $(OBJ)test_serialization.o $(OBJ)data.o $(OBJ)entry.o $(OBJ)serialization.o
	$(CC) -g $(OBJ)test_serialization.o $(OBJ)data.o $(OBJ)entry.o $(OBJ)serialization.o -o $(BIN)test_serialization

#this is temporary
main: $(OBJ)main.o $(OBJ)table.o $(OBJ)entry.o
	$(CC) -g $(OBJ)main.o $(OBJ)table.o $(OBJ)entry.o -o $(BIN)main	

clean:	
	rm -rf $(BIN)*
	rm $(OBJ)*.o