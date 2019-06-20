#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TEST_DIR=./teste
TEST_BIN=./teste/bin


# testes:
FORMAT=test_format_fs
READ=test_read
HASH=test_hashtable
WRITE_BLOCK=test_write_block
BITMAP=test_bitmap
CREATE=test_create2
WRITEFILE=test_write_file

all: helper hashtable t2fs libt2fs

apidisk:
	$(CC) -c $(SRC_DIR)/t2fs.c -o $(BIN_DIR)/t2fs.o -Wall

t2fs:
	$(CC) -c $(SRC_DIR)/t2fs.c -o $(BIN_DIR)/t2fs.o -Wall

hashtable:
	$(CC) -c $(SRC_DIR)/hashtable.c -o $(BIN_DIR)/hashtable.o -Wall

helper:
	$(CC) -c $(SRC_DIR)/helper.c -o $(BIN_DIR)/helper.o -Wall

libt2fs:
	ar rcs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/t2fs.o $(LIB_DIR)/apidisk.o $(BIN_DIR)/helper.o $(BIN_DIR)/hashtable.o

clean:
	find $(LIB_DIR)/*.o ! -name 'apidisk.o' -type f -exec rm -f {} +
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*~ $(TEST_DIR)/*~ $(TEST_BIN_DIR)/*~ $(SRC_DIR)/*~ $(INC_DIR)/*~ *~

# testes:

#compile

$(FORMAT): all $(TEST_DIR)/$(FORMAT).c
	$(CC) -o $(TEST_BIN)/$(FORMAT).o $(TEST_DIR)/$(FORMAT).c -L$(LIB_DIR) -lt2fs -Wall

$(READ): all $(TEST_DIR)/$(READ).c
	$(CC) -o $(TEST_BIN)/$(READ).o $(TEST_DIR)/$(READ).c -L$(LIB_DIR) -lt2fs -Wall

$(HASH): all $(TEST_DIR)/$(HASH).c
	$(CC) -o $(TEST_BIN)/$(HASH).o $(TEST_DIR)/$(HASH).c -L$(LIB_DIR) -lt2fs -Wall

$(CREATE): all $(TEST_DIR)/$(CREATE).c
	$(CC) -o $(TEST_BIN)/$(CREATE).o $(TEST_DIR)/$(CREATE).c -L$(LIB_DIR) -lt2fs -Wall

$(WRITE_BLOCK): all $(TEST_DIR)/$(WRITE_BLOCK).c
	$(CC) -o $(TEST_BIN)/$(WRITE_BLOCK).o $(TEST_DIR)/$(WRITE_BLOCK).c -L$(LIB_DIR) -lt2fs -Wall

$(BITMAP): all $(TEST_DIR)/$(BITMAP).c
	$(CC) -o $(TEST_BIN)/$(BITMAP).o $(TEST_DIR)/$(BITMAP).c -L$(LIB_DIR) -lt2fs -Wall

$(WRITEFILE): all $(TEST_DIR)/$(WRITEFILE).c
	$(CC) -o $(TEST_BIN)/$(WRITEFILE).o $(TEST_DIR)/$(WRITEFILE).c -L$(LIB_DIR) -lt2fs -Wall

#compile & execute

execute_$(FORMAT): $(FORMAT)
	clear
	$(TEST_BIN)/$(FORMAT).o

execute_$(READ): $(READ)
	clear
	$(TEST_BIN)/$(READ).o

execute_$(HASH): $(HASH)
	clear
	$(TEST_BIN)/$(HASH).o

execute_$(WRITE_BLOCK): $(WRITE_BLOCK)
	clear
	$(TEST_BIN)/$(WRITE_BLOCK).o

execute_$(BITMAP): $(BITMAP)
	clear
	$(TEST_BIN)/$(BITMAP).o

execute_$(CREATE): $(CREATE)
	clear
	$(TEST_BIN)/$(CREATE).o

execute_$(WRITEFILE): $(WRITEFILE)
	clear
	$(TEST_BIN)/$(WRITEFILE).o