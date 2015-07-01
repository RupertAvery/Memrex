#include "memcard.h"

void Memcard::Read(char* filepath) {
    _fp = fopen(filepath, "r");

    char * fileext = &filepath[strlen(filepath) - 4];
    printf("%s", fileext);

    if (strcasecmp(fileext, ".mem") == 0) {
        fseek(_fp, 64, SEEK_SET);
    } else {
        fseek(_fp, 0, SEEK_SET);
    }

    fread(directory, 1, BLOCK_SIZE, _fp);
    fread(data, BLOCK_SIZE, 14, _fp);
   
    fclose(_fp);
}

DIRENTRY* Memcard::GetDirEntry(int index) {
    return (DIRENTRY*) (directory + FRAME_SIZE + (index * FRAME_SIZE));
}

Memcard::~Memcard() {

}

