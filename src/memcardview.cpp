#include "memcardview.h"
//#include "font.h"
//#include <initializer_list>

GLfloat texCoords[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};

void MemCardView::Render(SDL_Window * window) {
	int size = 64;
	int padding = 16;

	//for (int i = 0; i < view->getIconCount(); i++)
	//{
	//	if ((i > 0) && (i % 3 == 0))
	//	{
	//		//glTranslatei( -2.5f * 3, -2.5f, 0.0f ); 
	//	}
	//	view->DrawIcon(i);
	//	glTranslatef((float)(size + padding), 0.0f, 0.0f);
	//}

	for (int i = 0; i < getIconCount(); i++)
	{
		x[i] += dx[i];
		if (x[i] < 0) dx[i] = -dx[i];
		if (x[i] + 64 > display_width) dx[i] = -dx[i];
		y[i] += dy[i];

		if (y[i] < 0) dy[i] = -dy[i];
		if (y[i] + 64 > display_height) dy[i] = -dy[i];
	}

	//glTranslatef(display_width / 2, display_height / 2, 0.0f);

	for (int i = 0; i < getIconCount(); i++)
	{
		glPushMatrix();
		glTranslatef(x[i], y[i], 0.0f);
		DrawIcon(i);
		glPopMatrix();
	}

	//t++;
	
}

void MemCardView::SetIconSize(int iconSize)
{
	GLNUM v[4][2] = {
		{ 0, 0 },
		{ 0, iconSize },
		{ iconSize, 0 },
		{ iconSize, iconSize }
	};

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 2; j++){
			vertices[i][j] = v[i][j];
		}
	}
}

void MemCardView::AnimateIcons()
{
	for (int i = 0; i < iconCount; i++){
		icon[i].currentFrame++;
		if (icon[i].currentFrame == icon[i].frameCount)
		{
			icon[i].currentFrame = 0;
		}
	}
}


void MemCardView::DrawIcon(int index) {
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, icon[index].texture[icon[index].currentFrame]);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glVertexPointer(2, GLTYPE, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

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

	//printf("Building texture %d\n", *texture);

	for (int y = 0; y < 16; y++)
	{
		unsigned char* row = icon->data[y];
		for (int x = 0; x < 8; x++)
		{
			unsigned char pixa = (row[x] >> 4);
			unsigned char pixb = (row[x] & 0xF);
			int position = ((y * 16) + (x * 2)) * 3;
			setPixel(pixels, position + 0, pixb, palette);
			setPixel(pixels, position + 3, pixa, palette);
		}
	}


	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

int MemCardView::getIconCount()
{
	return iconCount;
}

char* MemCardView::SJIStoASCII(char* sjis)
{
	char* str = (char *)malloc(32);
	for (int i = 0; i < 32; i++)
	{
		unsigned char nextbyte = (unsigned char)sjis[(i * 2) + 1];
		unsigned char thisbyte = (unsigned char)sjis[i * 2];

		switch (thisbyte)
		{
		case 0x00:
			str[i] = '\0';
			break;
		case 0x81:
			if (nextbyte == 0x40)
				str[i] = ' ';
			else if (nextbyte == 0x43)
				str[i] = ',';
			else if (nextbyte == 0x44)
				str[i] = '.';
			else if (nextbyte == 0x46)
				str[i] = ':';
			else if (nextbyte == 0x47)
				str[i] = ';';
			else if (nextbyte == 0x48)
				str[i] = '?';
			else if (nextbyte == 0x49)
				str[i] = '!';
			else if (nextbyte == 0x5E)
				str[i] = '/';
			else if (nextbyte == 0x5F)
				str[i] = '\\';
			else if (nextbyte == 0x7B)
				str[i] = '+';
			else if (nextbyte == 0x7C)
				str[i] = '-';
			else if (nextbyte == 0x93)
				str[i] = '%';
			else if (nextbyte == 0x94)
				str[i] = '#';
			else if (nextbyte == 0x95)
				str[i] = '&';
			else
			{
				str[i] = '^';
				printf("%x", nextbyte);
			}
			break;
		case 0x82:
			if (nextbyte >= 0x4F && nextbyte <= 0x58)
				str[i] = '0' + nextbyte - 0x4F;
			else if (nextbyte >= 0x60 && nextbyte <= 0x79)
				str[i] = 'A' + nextbyte - 0x60;
			else if (nextbyte >= 0x81 && nextbyte <= 0x9A)
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

	iconCount = 0;
	int totalBlocksUsed = 0;

	for (int i = 0; i < 15; i++)
	{
		DIRENTRY* dir = mc->GetDirEntry(i);

		//printf("Allocation: %x\n", dir->blockAllocationState);

		if (dir->blockAllocationState == 0x51) {
			TITLE* title = mc->GetTitle(i);
			printf("Block %d: %s (%x %d) %.*s\n", i + 1, (char *)&dir->filename, title->iconDisplayFlag, title->blockNumber, 32, SJIStoASCII(title->title));
			int frameCount = (title->iconDisplayFlag & 0xF);
			if(frameCount > 0 && frameCount < 4)
			{
				icon[iconCount].frameCount = frameCount;
				icon[iconCount].currentFrame = 0;
				for (int frame = 0; frame < frameCount; frame++)
				{
					BuildTexture(&icon[iconCount].texture[frame], mc->GetIcon(i, frame + 1), title->palette);
				}
				iconCount++;
			}

			int blocksUsed = 1;

			DIRENTRY* next = dir;

			while (next->nextBlockNo > 0) {
				blocksUsed++;
				next = mc->GetDirEntry(next->nextBlockNo);
			}

			printf("Used: %d\n", blocksUsed);
			totalBlocksUsed += blocksUsed;
		}
	}

	printf("TOTAL: %d\n", totalBlocksUsed);



	delete(mc);
}

void MemCardView::InitView(int width, int height) {
	srand(time(NULL));

	for (int i = 0; i < getIconCount(); i++)
	{
		printf("%d", rand() % 2);
		dx[i] = ((rand() % 2) == 1) ? 1 : -1;
		dy[i] = ((rand() % 2) == 1) ? 1 : -1;
		x[i] = rand() % (width - 64);
		y[i] = rand() % (height - 64);
	}
	display_width = width;
	display_height = height;
}

MemCardView::~MemCardView() {
	if (iconCount > 0)
	{
		for (int i = 0; i < iconCount; i++)
		{
			glDeleteTextures(icon[i].frameCount, icon[i].texture);
		}
	}
}




