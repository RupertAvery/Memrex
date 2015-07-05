#include "font.h"


FT_Library ft;
FT_Face face;
FT_GlyphSlot g;

int init_freetype() {

	if(FT_Init_FreeType(&ft)) {
	  fprintf(stderr, "Could not init freetype library\n");
	  return 1;
	}


	if(FT_New_Face(ft, "FreeSans.ttf", 0, &face)) {
	  fprintf(stderr, "Could not open font\n");
	  return 1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	FT_GlyphSlot g = face->glyph;

	return 0;
}

void render_text(const char *text, float x, float y, float sx, float sy) {
  const char *p;

  for(p = text; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;
 
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      g->bitmap.width,
      g->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      g->bitmap.buffer
    );
 
    float x2 = x + g->bitmap_left * sx;
    float y2 = -y - g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;
 
    GLfloat box[4][4] = {
        {x2,     -y2    , 0, 0},
        {x2 + w, -y2    , 1, 0},
        {x2,     -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };
 
    //glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
    x += (g->advance.x >> 6) * sx;
    y += (g->advance.y >> 6) * sy;
  }
}
