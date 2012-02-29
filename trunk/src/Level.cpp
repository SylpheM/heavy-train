#include "Level.hpp"



/*Level Constructor */

Level::Level () 
{
	//All the blocks of the level
	SDL_Rect * pos = new SDL_Rect;
	pos->x = 0;
	pos->y = 0;
	m_blocks.push_back(new Block(JUNCTION_DOWN, 1000, pos));
		//m_departureTime = ...;
		//m_position = ...;
		m_islandNum =0;
		m_levelNum = 0;
		//m_backgroundImages = ;
		//m_idStations= ...;

}

Level::~Level() 
{
	for(int i = 0; i < m_blocks.size(); ++i)
	{
		delete m_blocks[i];
	}
}


Block * Level::getBlock(int i) 
{
	return m_blocks.at(i);

}

/*
 * Level dessine tous les blocs du vector
 */
void Level::drawBlocks(SDL_Surface * screen, int w, int h)
{
	for (int i=0; i<m_blocks.size(); i++) 
	{
		m_blocks.at(i)->draw( screen, w, h);
	}
}



