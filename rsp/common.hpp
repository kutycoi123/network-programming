#ifndef STATUS_HPP
#define STATUS_HPP

# define RET_SUCCESS 0x01
# define RET_SET_NOT_EXIST 0x02
# define RET_ITEM_NOT_EXIST 0x03
# define RET_ITEM_ALREADY_EXIST 0x04

# define OPT_CRT_SET 0x01
# define OPT_ADD_ITEM 0x02
# define OPT_REM_ITEM 0x03
# define OPT_GET_ITEMS 0x04
# define OPT_CLR_SET 0x05
# define OPT_REM_SET 0x06
# define OPT_GET_SIZE 0x07
# define OPT_GET_SETS 0x08

#define DEFAULT_PORT 9090
#define MAX_BUFFER_SIZE 1024
#define LISTENQ_SIZE 10

#define OPT_SIZE 1
#define RET_SIZE 1
#define KEY_SIZE 32
#define LEN_SIZE 4
#define LEN_WORD_SIZE 4
#define ITEM_SIZE 4
#define INT_SIZE 4

#endif