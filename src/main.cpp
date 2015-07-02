#include "memcard.h"

void writebmp16(char* filename, int w, int h, ARGB pixels[16][16])
{
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * w * h;
	if(img) free(img);
	img = (unsigned char *)malloc(3 * w * h);
	memset(img,0,3 * w * h);
	ARGB blender;
	blender.r = 0x80;
	blender.g = 0x80;
	blender.b = 0x80;
	blender.a = 0x80;
	for(int i=0; i < w; i++)
	{
		for(int j=0; j < h; j++)
		{
			int x = i; 
			int y = (h - 1) - j;
			ARGB data = pixels[y][x];
			img[(x+y*w)*3+2] = data.r;
			img[(x+y*w)*3+1] = data.g;
			img[(x+y*w)*3+0] = data.b;
		}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0, 0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

	f = fopen(filename,"wb");
	
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for(int k=0; k<h; k++)
	{
		fwrite(img + (w * (h - k - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}
	
	free(img);
	fclose(f);
}

char* SJIStoASCII(char* sjis){
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

ARGB getRGB(unsigned char index, char* palette)
{
	ARGB color;
	// 16-bit 5-5-5 BGR GGGRRRRR xBBBBBGG
	unsigned char _index = index * 2;
	color.r = (palette[_index] & 0x1F) << 3; 
	color.g = (((palette[_index + 1] & 0x03) << 6) | ((palette[_index] & 0xE0) >> 2));
	color.b = (palette[_index + 1] & 0x7C) << 1;
	return color;
}

int main(int argc, char* argv[]) {
	printf("Starting Memrex...\n");
    Memcard* mc = new Memcard();
    mc->Read(argv[1]);
	for(int i = 0; i < 15; i++)
	{	
		DIRENTRY* dir = mc->GetDirEntry(i);
		if (dir->blockAllocationState == 0x51) {
			TITLE* title = mc->GetTitle(i);
			//printf("Block %d: %s (%x %d) %.*s\n", i + 1, (char *)&dir->filename,  title->iconDisplayFlag, title->blockNumber, 32, SJIStoASCII(title->title));
			int iconCount = (title->iconDisplayFlag & 0xF);
			
			for(int j=1; j <= iconCount; j++)
			{
				ICON* icon = mc->GetIcon(i, j);
				ARGB pixels[16][16];

				for(int y=0; y < 16; y++)
				{
					unsigned char* row = icon->data[y];
					for(int x=0; x < 8; x++)
					{
						unsigned char pixa = (row[x] >> 4);
						unsigned char pixb = (row[x] & 0xF);
						
						pixels[y][(x * 2) + 1] = getRGB(pixa, title->palette);
						pixels[y][(x * 2) + 0] = getRGB(pixb, title->palette);
					}
				}
				
				char buf[20];
				sprintf(buf, "%.*s-%d.bmp", 10, dir->filename.gamecode, j);
				writebmp16(buf, 16, 16, pixels);

			}
			
			
		}		
	}
	printf("Shutting down...\n");
    delete(mc);
    return 0;
}





