#include "memcard.h"

int main(int argc, char* argv[]) {
    char path[255] = "ff9e.mem";
    Memcard* mc = new Memcard();
    mc->Read(path);
    mc->GetDirEntry(0);
    delete(mc);
    return 0;
}



