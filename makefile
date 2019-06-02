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

all: t2fs

apidisk:
	$(CC) -c $(SRC_DIR)/t2fs.c -o $(BIN_DIR)/t2fs.o -Wall

t2fs:
	$(CC) -c $(SRC_DIR)/t2fs.c -o $(BIN_DIR)/t2fs.o -Wall

libt2fs:
	ar rcs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/t2fs.o $(LIB_DIR)/apidisk.o

clean:
	find $(BIN_DIR)/*.o ! -name 'apidisk.o' -type f -exec rm -f {} +
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*~ $(TEST_DIR)/*~ $(TEST_BIN_DIR)/*~ $(SRC_DIR)/*~ $(INC_DIR)/*~ *~


