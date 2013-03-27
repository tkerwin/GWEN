/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#ifndef GWEN_RENDERERS_OPENGL_BMFONT_H
#define GWEN_RENDERERS_OPENGL_BMFONT_H

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/OpenGL.h"
#include "GL/glew.h"

namespace Gwen {

	namespace BMFont {

		struct txdata {
			float x, y;
			float tx,ty;
			GLubyte r,g,b,a;
			txdata(float x, float y, float tx, float ty, GLubyte r, GLubyte g, GLubyte b, GLubyte a  )
			{
				this->x = x;
				this->y = y;
				this->tx = tx;
				this->ty = ty;
				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;
			}
		};


		class KerningInfo
		{
		public:
			short First;
			short Second;
			short Amount;

			KerningInfo() :  First( 0 ), Second( 0 ), Amount( 0 )	{ }
		};


		class CharDescriptor
		{
		public:
			short x, y;
			short Width;
			short Height;
			short XOffset;
			short YOffset;
			short XAdvance;
			short Page;

			CharDescriptor() : x( 0 ), y( 0 ), Width( 0 ), Height( 0 ), XOffset( 0 ), YOffset( 0 ),
				XAdvance( 0 ), Page( 0 )
			{ }
		};
	}
}

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

				void Begin();
				void End();

			protected:

				void CreateBMFont();
				void DestroyBMFont();

				Gwen::Texture*	m_pFontTexture;

				int LineHeight;
				int Base;
				int Width;
				int Height;
				int Pages;
				int Outline;
				int KernCount;
				bool UseKern;
				std::map<int,BMFont::CharDescriptor> Chars;
				std::vector<BMFont::KerningInfo> KernLengths;
				int fcolor;
				GLuint ftexid;
				float fscale;
				int fblend;
				void ReplaceExtension(std::string &str, std::string rep);
				char* replace_str(char *str, char *orig, char *rep);
				void Render_String(int len);
				bool ParseFont(char *);
				int GetKerningPair(int, int);
				std::vector<BMFont::txdata> txlist;
				
				float m_fLetterSpacing;
				float m_fFontScale[2];
		public:

			//
			// Self Initialization
			//

			virtual bool InitializeContext( Gwen::WindowProvider* pWindow );
			virtual bool ShutdownContext( Gwen::WindowProvider* pWindow );
			virtual bool PresentContext( Gwen::WindowProvider* pWindow );
			virtual bool ResizedContext( Gwen::WindowProvider* pWindow, int w, int h );
			virtual bool BeginContext( Gwen::WindowProvider* pWindow );
			virtual bool EndContext( Gwen::WindowProvider* pWindow );
		};

	}
}
#endif
