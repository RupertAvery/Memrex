#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"

#define BLOCK_STATE_INUSE 0x50
#define BLOCK_STATE_FREE 0xA0
#define BLOCK_SIZE 0x2000
#define FRAME_SIZE 0x80
#define MAGIC_NOT_FOUND_ERR 0x01

struct FILENAME {
    char region[2];
    char gamecode[10];
    char extra[8];
};

struct DIRENTRY {
    int blockAllocationState;
    unsigned int filesize;
    short nextBlockNo;
    FILENAME filename;
    char zero;
    char unused[96];
    char checksum;
};

struct ICON {
    char data[8][16];
};

struct CLUT {
    unsigned short color;
};

struct TITLE {
    char id[2];
    char iconDisplayFlag;
    char blockNumber;
    char title[64]; 
    char reserved1[12];
    char reserved2[16];
    CLUT palette[16];
};

struct DATAFRAME {
    char data[FRAME_SIZE];
};

class Memcard {
    private:
        FILE* _fp;
        char directory[BLOCK_SIZE];
        char data[BLOCK_SIZE][14];
        unsigned long mc_data_start; 

    public:
        void Read(char* filepath);
        DIRENTRY* GetDirEntry(int index);
        
    	~Memcard();

};
