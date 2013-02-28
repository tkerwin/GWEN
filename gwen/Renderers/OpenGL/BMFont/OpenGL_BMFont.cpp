
#include "Gwen/Renderers/OpenGL_BMFont.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include <math.h>
#include "GL/glew.h"

#include "FontData.h"


namespace Gwen
{
	namespace Renderer
	{
		OpenGL_BMFont::OpenGL_BMFont()
		{
			m_fLetterSpacing = 1.0f/16.0f;
			m_fFontScale[0] = 1.5f;
			m_fFontScale[1] = 1.5f;
			m_pFontTexture = NULL;
		}

		void OpenGL_BMFont::Init()
		{
			CreateDebugFont();
		}

		OpenGL_BMFont::~OpenGL_BMFont()
		{
			DestroyDebugFont();
		}


		void OpenGL_BMFont::CreateDebugFont()
		{
			if ( m_pFontTexture ) return;

			m_pFontTexture = new Gwen::Texture();

			// Create a little texture pointer..
			GLuint* pglTexture = new GLuint;

			// Sort out our GWEN texture
			m_pFontTexture->data = pglTexture;
			m_pFontTexture->width = 256;
			m_pFontTexture->height = 256;

			// Create the opengl texture
			glGenTextures( 1, pglTexture );
			glBindTexture( GL_TEXTURE_2D, *pglTexture );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			unsigned char* texdata = new unsigned char[256*256*4];
			for (int i=0;i<256*256;i++)
			{
				texdata[i*4] = 255;
				texdata[i*4+1] = 255;
				texdata[i*4+2] = 255;
				texdata[i*4+3] = sGwenFontData[i];
			}

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_pFontTexture->width, m_pFontTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)texdata );
			delete[]texdata;
		}

		void OpenGL_BMFont::DestroyDebugFont()
		{
			if ( !m_pFontTexture ) return;

			GLuint* tex = (GLuint*)m_pFontTexture->data;
			if ( !tex ) return;

			glDeleteTextures( 1, tex );
			delete tex;

			m_pFontTexture->data = NULL;

			delete m_pFontTexture;
			m_pFontTexture = NULL;
		}

		void OpenGL_BMFont::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text )
		{
			float fSize = pFont->size * Scale();

			if ( !text.length() )
				return;

			Gwen::String converted_string = Gwen::Utility::UnicodeToString( text );

			float yOffset=0.0f;
			for ( int i=0; i<text.length(); i++ )
			{
				char ch = converted_string[i];
				float curSpacing = sGwenDebugFontSpacing[ch] * m_fLetterSpacing * fSize * m_fFontScale[0];
				Gwen::Rect r( pos.x + yOffset, pos.y-fSize*0.5, (fSize * m_fFontScale[0]), fSize * m_fFontScale[1] );

				if ( m_pFontTexture )
				{
					float uv_texcoords[8]={0.,0.,1.,1.};

					if ( ch >= 0 )
					{
						float cx= (ch%16)/16.0;
						float cy= (ch/16)/16.0;
						uv_texcoords[0] = cx;			
						uv_texcoords[1] = cy;
						uv_texcoords[4] = float(cx+1.0f/16.0f);	
						uv_texcoords[5] = float(cy+1.0f/16.0f);
					}

					DrawTexturedRect( m_pFontTexture, r, uv_texcoords[0], uv_texcoords[5], uv_texcoords[4], uv_texcoords[1] );
					yOffset+=curSpacing;
				} 
				else
				{
					DrawFilledRect( r );
					yOffset+=curSpacing;

				}
			}

		}

		Gwen::Point OpenGL_BMFont::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text )
		{
			Gwen::Point p;
			float fSize = pFont->size * Scale();

			Gwen::String converted_string = Gwen::Utility::UnicodeToString( text );
			float spacing = 0.0f;

			for ( int i=0; i<text.length(); i++ )
			{
				char ch = converted_string[i];
				spacing += sGwenDebugFontSpacing[ch];
			}

			p.x = spacing*m_fLetterSpacing*fSize * m_fFontScale[0];
			p.y = pFont->size * Scale();
			return p;
		}

		void OpenGL_BMFont::Begin()
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glAlphaFunc( GL_GREATER, 1.0f );	
			//glEnable(GL_ALPHA_TEST);

			glActiveTextureARB(GL_TEXTURE3_ARB);
			glDisable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE2_ARB);
			glDisable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glDisable(GL_TEXTURE_2D);

			glEnable ( GL_BLEND );
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		void OpenGL_BMFont::End()
		{
			Flush();
			glPopAttrib();
		}

		bool OpenGL_BMFont::InitializeContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

		bool OpenGL_BMFont::ShutdownContext( Gwen::WindowProvider* pWindow )
		{
			return false;
		}

		bool OpenGL_BMFont::PresentContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

		bool OpenGL_BMFont::ResizedContext( Gwen::WindowProvider* pWindow, int w, int h )
		{
			return true;
		}

		bool OpenGL_BMFont::BeginContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

		bool OpenGL_BMFont::EndContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

	}
}