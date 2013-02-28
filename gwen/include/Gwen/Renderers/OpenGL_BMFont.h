/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#ifndef GWEN_RENDERERS_OPENGL_BMFONT_H
#define GWEN_RENDERERS_OPENGL_BMFONT_H

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/OpenGL.h"

namespace Gwen
{
	namespace Renderer
	{

		class OpenGL_BMFont : public Gwen::Renderer::OpenGL
		{
			public:

				OpenGL_BMFont();
				~OpenGL_BMFont();

				void Init();

				void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text );
				Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text );

			protected:

				void CreateBMFont();
				void DestroyBMFont();

				Gwen::Texture*	m_pFontTexture;
				float			m_fFontScale[2];
				float			m_fLetterSpacing;

		};

	}
}
#endif
