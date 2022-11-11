CC=g++
SRC_DIR=src
FILES=$(SRC_DIR)/Entity.cpp $(SRC_DIR)/Entity.hpp $(SRC_DIR)/Sim_params.hpp $(SRC_DIR)/main.cpp $(SRC_DIR)/Population.cpp $(SRC_DIR)/Population.hpp  $(SRC_DIR)/utils.cpp $(SRC_DIR)/utils.hpp 
CFLAGS=-std=c++11 -Wall -lgsl

main: $(FILES)
	$(CC) $(FILES) -o main.exe $(CFLAGS)