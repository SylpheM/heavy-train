#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include "Bonus.hpp"
#include "Sprite.hpp"

#ifdef _WIN32
#include "Box2D/Box2D.h"
#else
#include <Box2D/Box2D.h>
#endif

enum BlockType
{
	GROUND = 0,
	PRECIPICE,
	STATION,
	TUNNEL,
	JUNCTION_UP,
	JUNCTION_DOWN
};

class Block
{
	public :
		Block(BlockType type, int sizeX, SDL_Rect * pos);
		virtual ~Block();
		void build(b2World * world);
		void scroll(int);
		b2Body * getBody();
		void setBody(b2Body * body);
		void draw(SDL_Surface * screen, const int & width, const int & height);
	private :
		Sprite * m_sprite;
		b2Body * m_body;
		int m_sizeX;

		BlockType m_type;

		//If the block is GROUND, all the points of the spline are stored here
		std::vector<SDL_Rect*> m_points;
		//If the block is GROUND, all the points of the body are stored here
		std::vector<b2Vec2> m_groundPoints;
		
		int m_maxSpeed;
		int m_id;
		int m_sizeXMin;
		int m_sizeXMax;
		int m_y;
		int m_yMax;

		const float m_hillSegmentWidth;
};
#endif
