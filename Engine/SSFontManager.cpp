
#include "Core.h"
#include "SSFontManager.h"



void SSFontManager::Initialize(int dpiX, int dpiY)
{
	FT_Library lib;
	FT_Face fontFace;

	auto error = FT_Init_FreeType(&lib);

	if(error)
	{
		
	}

	if(FT_New_Face(lib, "./Resource/Font/consola.ttf", 0, &fontFace) != 0)
	{
		check(false);
	}

	FT_UInt index = 0;

	std::vector<std::pair<int, int>> vertices;

	for(FT_ULong charCode = FT_Get_First_Char(fontFace, &index); index != 0; charCode = FT_Get_Next_Char(fontFace, charCode, &index))
	{
		FT_Set_Char_Size(fontFace, 256 * 64, 0, dpiX, dpiY);

		FT_Error loadErr = FT_Load_Glyph(fontFace, index, FT_LOAD_DEFAULT);
		
		if(loadErr != 0)
		{
			continue;
		}		

		FT_GlyphSlot glygh = fontFace->glyph;

		for(int c = 0; c < glygh->outline.n_contours; ++c)
		{
			auto start = (c == 0) ? 0 : glygh->outline.contours[c - 1] + 1;
			auto end = glygh->outline.contours[c];

			for(int p = start ; p <= end; ++p)
			{
				FT_Vector& point = glygh->outline.points[p];
				
				auto x = point.x >> 6;
				auto y = point.y >> 6;

				vertices.push_back(std::pair<int,int>(x,y));
			}
		}
	}

	
}
