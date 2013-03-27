
#include "Gwen/Renderers/OpenGL_BMFont.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include <math.h>

#include <iostream>
#include <fstream>
#include <strstream>

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
			CreateBMFont();
		}

		OpenGL_BMFont::~OpenGL_BMFont()
		{
			DestroyBMFont();
		}


		void OpenGL_BMFont::CreateBMFont()
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

		void OpenGL_BMFont::DestroyBMFont()
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

		// will be cool some day
/*
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
				f=&Chars[ch];

				CurX = x + f->XOffset;
				CurY = y + f->YOffset;
				DstX = CurX + f->Width;
				DstY = CurY + f->Height;

				Gwen::Rect r( pos.x + yOffset, pos.y-fSize*0.5, (fSize * m_fFontScale[0]), fSize * m_fFontScale[1] );

				if ( m_pFontTexture )
				{
					float uv_texcoords[8]={0.,0.,1.,1.};

					if ( ch >= 0 )
					{
						float cx= 
						float cy= (ch/16)/16.0;
						uv_texcoords[0] = cx;			
						uv_texcoords[1] = cy;
						uv_texcoords[4] = float(cx+1.0f/16.0f);	
						uv_texcoords[5] = float(cy+1.0f/16.0f);
					}

					DrawTexturedRect( m_pFontTexture, r, uv_texcoords[0], uv_texcoords[5], uv_texcoords[4], uv_texcoords[1] );
					yOffset+=curSpacing;
				} 
			}
		}
*/

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


//! Font loading and processing
/*

		bool OpenGL_BMFont::ParseFont(char *fontfile )
		{
			std::ifstream Stream(fontfile); 
			std::string Line;
			std::string Read, Key, Value;
			std::size_t i;
			int first,second,amount;

			BMFont::KerningInfo K;
			BMFont::CharDescriptor C;

			while( !Stream.eof() )
			{
				std::stringstream LineStream;
				std::getline( Stream, Line );
				LineStream << Line;

				//read the line's type
				LineStream >> Read;
				if( Read == "common" )
				{
					//this holds common data
					while( !LineStream.eof() )
					{
						std::stringstream Converter;
						LineStream >> Read;
						i = Read.find( '=' );
						Key = Read.substr( 0, i );
						Value = Read.substr( i + 1 );

						//assign the correct value
						Converter << Value;
						if( Key == "lineHeight" )
						{Converter >> LineHeight;}
						else if( Key == "base" )
						{Converter >> Base;}
						else if( Key == "scaleW" )
						{Converter >> Width;}
						else if( Key == "scaleH" )
						{Converter >> Height;}
						else if( Key == "pages" )
						{Converter >> Pages;}
						else if( Key == "outline" )
						{Converter >> Outline;}
					}
				}

				else if( Read == "char" )
				{
					//This is data for each specific character.
					int CharID = 0;

					while( !LineStream.eof() )
					{
						std::stringstream Converter;
						LineStream >> Read;
						i = Read.find( '=' );
						Key = Read.substr( 0, i );
						Value = Read.substr( i + 1 );

						//Assign the correct value
						Converter << Value;
						if( Key == "id" )
						{Converter >> CharID;}
						else if( Key == "x" )
						{	Converter >> C.x;}      
						else if( Key == "y" )
						{	Converter >> C.y;}      
						else if( Key == "width" )
						{	Converter >> C.Width;}        
						else if( Key == "height" )
						{	Converter >> C.Height;}         
						else if( Key == "xoffset" )
						{	Converter >> C.XOffset;}         
						else if( Key == "yoffset" )
						{	Converter >> C.YOffset;}        
						else if( Key == "xadvance" )
						{	Converter >> C.XAdvance;}         
						else if( Key == "page" )
						{	Converter >> C.Page;}           
					}

					Chars.insert(std::map<int,CharDescriptor>::value_type( CharID,C ));

				}
				else if( Read == "kernings" )
				{
					while( !LineStream.eof() )
					{
						std::stringstream Converter;
						LineStream >> Read;
						i = Read.find( '=' );
						Key = Read.substr( 0, i );
						Value = Read.substr( i + 1 );

						//assign the correct value
						Converter << Value;
						if( Key == "count" )
						{Converter >> KernCount; }
					}
				}

				else if( Read == "kerning" )
				{
					while( !LineStream.eof() )
					{
						std::stringstream Converter;
						LineStream >> Read;
						i = Read.find( '=' );
						Key = Read.substr( 0, i );
						Value = Read.substr( i + 1 );

						//assign the correct value
						Converter << Value;
						if( Key == "first" )
						{Converter >> K.First; Converter >> first; }

						else if( Key == "second" )
						{Converter >> K.Second; Converter >> second; }

						else if( Key == "amount"  )
						{Converter >> K.Amount; Converter >> amount;}
					}
					//LOG_DEBUG("Done with this pass");
					KernLengths.push_back(K);
				}
			}

			Stream.close();
			return true;
		}


		int OpenGL_BMFont::GetKerningPair(int first, int second)
		{
			if ( KernCount && UseKern ) //Only process if there actually is kerning information
			{
				//kerning is checked for every character processed. This is expensive in terms of processing time.
				for (int j = 0; j < KernCount;  j++ )
				{
					if (KernLengths[j].First == first && KernLengths[j].Second == second)
					{ 
						//LOG_DEBUG("Kerning Pair Found!!!");kerning._Left
						// LOG_DEBUG("FIRST: %d SECOND: %d offset %d",first,second,Kearn[j].Amount);
						return KernLengths[j].Amount;
			  }
				}
			}
			return 0;
		}


		float OpenGL_BMFont::GetStringWidth(const char *string)
		{
			float total=0;
			CharDescriptor  *f;

			for (int i = 0; i != strlen(string); i++)
			{ 
				f=&Chars[string[i]];
				total+=f->XAdvance;
			}

			return total * fscale;
		}

		bool  BMFont::LoadFont(char *fontfile)
		{
			std::ifstream Stream(fontfile);
			if ( !Stream.is_open() )          
			{   
				LOG_DEBUG("Cannot Find Font File %s",fontfile);
				return false;         
			}
			Stream.close();

			//Ok, we have a file. Can we get the Texture as well?
			char* buf=replace_str( fontfile,".fnt", ".png");
			//ReplaceExtension(fontfile,"png");


			// FIXME
			assert(false);
			//ftexid = LoadPNG(buf);


			if (!ftexid)
			{   
				LOG_DEBUG("Cannot find font texture for loading %s",fontfile);
				return false;         
			}

			LOG_DEBUG("Starting to Parse Font %s",fontfile);
			ParseFont(fontfile);
			LOG_DEBUG("Finished Parsing Font %s",fontfile);
			KernCount = (int) KernLengths.size();
			return true;
		}

		void BMFont::Print(Gwen::Point pos, const Gwen::UnicodeString& text)
		{
	

			float CurX = (float) pos.x;
			float CurY = (float) pos.y;
			float DstX = 0.0;
			float DstY = 0.0;
			int Flen;

			float adv = (float) 1.0/Width;                      // Font texture atlas spacing. 
			char	text[512] = "";	                            // Holds Our String
			CharDescriptor  *f;									// Pointer to font.

			

			//Select and enable the font texture. (With mipmapping.)

			// FIXME!
			assert(false);
			//use_texture(&ftexid, 1,0);
			//Set type of blending to use with this font.
			// FIXME!
			assert(false);
			//	SetBlendMode(fblend);
			//Set Text Color, all one color for now. 
			unsigned char *color = (unsigned char*)&fcolor;

			//y= y + LineHeight; //This can be used to flip rendering
			Flen = strlen(text);

			//Todo: Add caching to make this much faster
			txlist.clear();

			for (int i = 0; i != Flen; ++i)
			{

				f=&Chars[text[i]];

				CurX = x + f->XOffset;
				CurY = y + f->YOffset;
				DstX = CurX + f->Width;
				DstY = CurY + f->Height;


				txlist.push_back(txdata(fscale*CurX, fscale*DstY, adv*f->x, 1.0f -(adv*(f->y+f->Height)), \
					color[0],color[1],color[2],color[3] ));

				txlist.push_back(txdata(fscale*CurX, fscale*CurY, adv*f->x,  1.0f -(adv*f->y), \
					color[0],color[1],color[2],color[3] ));

				txlist.push_back(txdata(fscale*DstX, fscale*CurY, adv*(f->x+f->Width),1.0f -(adv*f->y), \
					color[0],color[1],color[2],color[3] ));

				txlist.push_back(txdata(fscale*DstX, fscale*DstY, adv*(f->x+f->Width),1.0f-(adv*(f->y+f->Height)), \
					color[0],color[1],color[2],color[3] ));

				//Only check kerning if there is greater then 1 character and 
				//if the check character is 1 less then the end of the string.
				if (Flen > 1 && i < Flen)
				{ 
					x += GetKerningPair(text[i],text[i+1]);
				}

				x +=  f->XAdvance;
			}
			Render_String(strlen(text));
		}

*/
	}
}