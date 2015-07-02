#include "memcard.h"

void Memcard::Read(char* filepath) {
    FILE* fp = fopen(filepath, "r");

	if (fp == NULL) {
		printf("Unable to open %s!\n", filepath);
		//printf("%d!\n", errno);
		return;
	}
	
    char * fileext = &filepath[strlen(filepath) - 4];
    //printf("%s\n", fileext);

    if (strcasecmp(fileext, ".mem") == 0) {
        fseek(fp, 64, SEEK_SET);
    } else {
        fseek(fp, 0, SEEK_SET);
    }

    fread(directory, 1, BLOCK_SIZE, fp);
    fread(data, BLOCK_SIZE, 14, fp);
   
    fclose(fp);
}

DIRENTRY* Memcard::GetDirEntry(int index) {
    return (DIRENTRY*) (directory + FRAME_SIZE + (index * FRAME_SIZE));
}

TITLE* Memcard::GetTitle(int block) {
    return (TITLE*) (data[block]);
}

ICON* Memcard::GetIcon(int block, int frame) {
    return (ICON*) (data[block] + (frame * FRAME_SIZE));
}

Memcard::~Memcard() {

}

