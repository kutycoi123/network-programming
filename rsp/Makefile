CC=g++
CFLAGS=-g -Wall
RM=rm
OBJ_DIR=build
BIN_DIR=bin

# define any directories containing header files other than /usr/include
INCLUDES = -I../include

# define library paths in addition to /usr/lib
LFLAGS = -L../lib

# define any libraries to link into executable
LIBS =

OBJS_COMMON=$(OBJ_DIR)/net.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/request.o $(OBJ_DIR)/response.o
OBJS_SERVER=$(OBJS_COMMON) $(OBJ_DIR)/set.o $(OBJ_DIR)/server.o $(OBJ_DIR)/server_handler.o $(OBJ_DIR)/rset_server.o
OBJS_CLIENT=$(OBJS_COMMON) $(OBJ_DIR)/client.o $(OBJ_DIR)/client_handler.o $(OBJ_DIR)/rset_client.o

all: rset_server rset_client

rset_server: $(OBJS_SERVER)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $(BIN_DIR)/$@ $(LFLAGS) $(LIBS)

rset_client: $(OBJS_CLIENT)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $(BIN_DIR)/$@ $(LFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p '$(@D)'
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -rf $(OBJ_DIR)/*.o
	$(RM) -rf $(BIN_DIR)/rset_client
	$(RM) -rf $(BIN_DIR)/rset_server