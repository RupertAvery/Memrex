#include "memcardview.h"

void MemCardView::DrawIcon(int index) {
	glPushMatrix();
	glTranslatef(0.5f, -0.5f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, textures[index]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(  1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(  1.0f,  1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f,  1.0f, 0.0f);
	glEnd();
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void MemCardView::setPixel(unsigned char* pixels, int pointer, unsigned char index, char* palette)
{
	// 16-bit 5-5-5 BGR GGGRRRRR xBBBBBGG
	unsigned char _index = index * 2;
	pixels[pointer + 0] = (palette[_index] & 0x1F) << 3; 
	pixels[pointer + 1] = (((palette[_index + 1] & 0x03) << 6) | ((palette[_index] & 0xE0) >> 2));
	pixels[pointer + 2] = (palette[_index + 1] & 0x7C) << 1;
}

void MemCardView::BuildTexture(GLuint *texture, ICON* icon, char *palette) {
	unsigned char pixels[3 * 16 * 16];

	glGenTextures(1, texture);

	printf("Building texture %d\n", *texture);
    
	for(int y=0; y < 16; y++)
	{
		unsigned char* row = icon->data[y];
		for(int x=0; x < 8; x++)
		{
			unsigned char pixa = (row[x] >> 4);
			unsigned char pixb = (row[x] & 0xF);
			int position = ((y * 16) + (x * 2)) * 3;
			setPixel(pixels, position + 0, pixb, palette);
			setPixel(pixels, position + 3, pixa, palette);
		}
	}


	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16 ,16, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
} 

int MemCardView::getIconCount()
{
	return textureCount;
}
 
char* MemCardView::SJIStoASCII(char* sjis)
{
	char* str = (char *)malloc(32);
	for(int i=0; i<32; i++)
	{
		unsigned char nextbyte = (unsigned char)sjis[(i*2) + 1];
		unsigned char thisbyte = (unsigned char)sjis[i * 2];

		switch(thisbyte)
		{
			case 0x00:
				str[i] = '\0';
				break;
			case 0x81:
				if(nextbyte==0x40) 
					str[i] = ' ';
				else if(nextbyte==0x43) 
					str[i] = ',';
				else if(nextbyte==0x44) 
					str[i] = '.';
				else if(nextbyte==0x46) 
					str[i] = ':';
				else if(nextbyte==0x47) 
					str[i] = ';';
				else if(nextbyte==0x48) 
					str[i] = '?';
				else if(nextbyte==0x49) 
					str[i] = '!';
				else if(nextbyte==0x5E) 
					str[i] = '/';
				else if(nextbyte==0x5F) 
					str[i] = '\\';
				else if(nextbyte==0x7B) 
					str[i] = '+';
				else if(nextbyte==0x7C) 
					str[i] = '-';
				else if(nextbyte==0x93) 
					str[i] = '%';
				else if(nextbyte==0x94) 
					str[i] = '#';
				else if(nextbyte==0x95) 
					str[i] = '&';
				else 
					{
						str[i] = '^';
						printf("%x", nextbyte);
					}
				break;
			case 0x82:
				if(nextbyte>=0x4F && nextbyte<=0x58) 
					str[i] = '0' + nextbyte - 0x4F;
				else if(nextbyte>=0x60 && nextbyte<=0x79) 
					str[i] = 'A' + nextbyte - 0x60;
				else if(nextbyte>=0x81 && nextbyte<=0x9A) 
					str[i] = 'a' + nextbyte - 0x81;
				else
					{
						str[i] = '_';
						printf("%x", nextbyte);
					}
				break;
			default:
				{
					str[i] = '?';
					printf("-%x%x-", thisbyte, nextbyte);
				}
				break;
		}
	}
	return str;
}


void MemCardView::Load(char * path) {
	Memcard* mc = new Memcard();

	mc->Read(path);
	printf("Loading %s\n", path);

	textureCount = 0;

	for(int i = 0; i < 15; i++)
	{	
		DIRENTRY* dir = mc->GetDirEntry(i);

		if (dir->blockAllocationState == 0x51) {
			TITLE* title = mc->GetTitle(i);
			printf("Block %d: %s (%x %d) %.*s\n", i + 1, (char *)&dir->filename,  title->iconDisplayFlag, title->blockNumber, 32, SJIStoASCII(title->title));
			//int iconCount = (title->iconDisplayFlag & 0xF);

			BuildTexture(&textures[textureCount], mc->GetIcon(i, 1), title->palette);
		
			textureCount++;

		}		
	}
	delete(mc);
}

MemCardView::~MemCardView() {
	if(textureCount > 0)
	{
    	glDeleteTextures(textureCount, textures);
	}
}



  
