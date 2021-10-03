
#include "SSCore.h"
#include "SSFontManager.h"
#include "ft2build.h"
#include FT_FREETYPE_H



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

	

	for(FT_ULong charCode = FT_Get_First_Char(fontFace, &index); index != 0; charCode = FT_Get_Next_Char(fontFace, charCode, &index))
	{
		FT_Set_Char_Size(fontFace, 256 * 64, 0, dpiX, dpiY);

		FT_Error loadErr = FT_Load_Glyph(fontFace, index, FT_LOAD_DEFAULT);
		
		if(loadErr != 0)
		{
			continue;
		}		

		FT_GlyphSlot glygh = fontFace->glyph;
		
		std::vector<XMFLOAT2> points;

		for(int c = 0; c < glygh->outline.n_contours; ++c)
		{
			auto start = (c == 0) ? 0 : glygh->outline.contours[c - 1] + 1;
			auto end = glygh->outline.contours[c];

			for(int p = start ; p <= end; ++p)
			{
				FT_Vector& point = glygh->outline.points[p];
				
				auto x = point.x >> 6;
				auto y = point.y >> 6;

				XMFLOAT2 newPoint(x,y);
				points.push_back(newPoint);
			}

			XMFLOAT2 point(static_cast<float>( glygh->outline.points[start].x >> 6), static_cast<float>( glygh->outline.points[start].y >> 6) );
			points.push_back(point);
		}		

		SSFontCharacter newChar;
		
		newChar.mCharCode = charCode;
		newChar.mPoints = std::move(points);

		mFontMap[charCode] = newChar;
	}

	FT_Done_Face(fontFace);

	bInitialized = true;
}


SSFontCharacter& SSFontManager::GetCharacterInfo(unsigned long charCode)
{
	return mFontMap[charCode];
}
