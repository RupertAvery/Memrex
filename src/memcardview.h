#include "memcard.h"
#include "GL/gl.h"
#include "GL/glu.h"

class MemCardView {
	GLuint textures[15];
	int textureCount;
	void setPixel(unsigned char* pixels, int pointer, unsigned char index, char* palette);
    void BuildTexture(GLuint *texture, ICON* icon, char* palette);
	char* SJIStoASCII(char* sjis);

public:
	int getIconCount();
	void DrawIcon(int index);
	void Load(char * path);
	~MemCardView();
};
