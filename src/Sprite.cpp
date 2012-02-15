#include "Sprite.hpp"

/**
 * Sprite Constructor
 */
Sprite::Sprite(const char* imageFileName, SDL_Rect * pos, SDL_Rect * size)
: m_actualFrame(0)
, m_nbFrames(0)
, m_position(pos)
, m_angle(0.0)
, m_size(size)
, m_timeActualFrame(0)
, m_timeFrame(10)
{
	SDL_Surface * img = IMG_Load(imageFileName);
	if(img)
	{
		m_frames.push_back(img);
		m_nbFrames++;
	}
	//If pos is not in percentage
	assert( m_position->x >= 0 && m_position->y >= 0 && m_position->x <= 100 && m_position->y <= 100 );
}

/**
 * Sprite Destructor
 */
Sprite::~Sprite()
{
	for(unsigned int i = 0; i < m_frames.size(); ++i)
	{
		SDL_FreeSurface(m_frames[i]);
	}

	delete m_position;
	delete m_size;
}

/**
 * Add an new image in the sprite
 */
void Sprite::addImage(const char* imageFileName)
{
	SDL_Surface * img = IMG_Load(imageFileName);
	if(img)
	{
		m_frames.push_back(img);
		m_nbFrames++;
	}
}

/**
 * Animate the sprite, alternating its images every "m_nbFrames" seconds
 */
void Sprite::animate()
{
	if(m_nbFrames > 1)
	{
		//increments m_timeActualFrame (TODO : en secondes et pas en frame)
		m_actualFrame++;
		if(m_timeActualFrame >= m_timeFrame)
		{
			m_timeActualFrame = 0;
			m_actualFrame++;
			if(m_actualFrame >= m_nbFrames)
			{
				m_actualFrame = 0;
			}
		}
	}
}

/**
 * Draw the sprite in the screen
 */
void Sprite::draw(SDL_Surface * screen, const int & width, const int & height)
{
	if(m_actualFrame < m_nbFrames)
	{
		//Rotation and zoom
		float zoomX = (float)width /m_size->x;
		float zoomY = (float)height/m_size->y ;
		SDL_Surface * image = rotozoomSurfaceXY(m_frames[m_actualFrame], m_angle, zoomX, zoomY, 1);

		//image = scaleSurface(m_frames[m_actualFrame], width, height);

		//Drawing to a position
		SDL_Rect * realPos = new SDL_Rect;
		realPos->x = width * m_position->x * 0.01;
		realPos->y = height * m_position->y * 0.01;
		SDL_BlitSurface(image, NULL, screen, realPos);
		
		delete realPos;
		SDL_FreeSurface(image);
	}
}

/**
 * Change the sprite's image actually displayed
 */
void Sprite::changeImageManually(const int & imageNum)
{
	if(imageNum < m_nbFrames)
	{
		m_actualFrame = imageNum;
	}
}

/*
*
*/
/*
SDL_Surface* Sprite::scaleSurface(SDL_Surface *surface, Uint16 Width, Uint16 Height)
{
    if(!surface || !Width || !Height)
        return 0;
     
    SDL_Surface *_ret = SDL_CreateRGBSurface(surface->flags, Width, Height, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
 
    double    _stretch_factor_x = (static_cast<double>(Width)  / static_cast<double>(surface->w)),
        _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(surface->h));
 
    for(Sint32 y = 0; y < surface->h; y++)
        for(Sint32 x = 0; x < surface->w; x++)
            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    putpixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x, 
                        static_cast<Sint32>(_stretch_factor_y * y) + o_y, getpixel(surface, x, y));
 
    return _ret;
}

Uint32 Sprite::getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       // shouldn't happen, but avoids warnings
    }
}

void Sprite::putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}*/