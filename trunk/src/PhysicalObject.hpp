#ifndef __PHYSICAL_OBJECT_HPP__
#define __PHYSICAL_OBJECT_HPP__

#ifdef _WIN32
#include "Box2D/Box2D.h"
#include "SDL/SDL_mixer.h"
#else
#include <Box2D/Box2D.h>
#include <SDL/SDL_mixer.h>
#endif

#include "Sprite.hpp"

class PhysicalObject
{
	public :
		PhysicalObject();
		PhysicalObject(Sprite * sprite);
		virtual ~PhysicalObject();

		//Retourne m_body
		b2Body * getBody();
		//Attribue un body
		void setBody(b2Body * body);
		//Retourne le m_sprite
		Sprite * getSprite();

		//Pointeur vers le m_world de ActualGame
		static b2World * m_world;
		//Ratio calcul� en fonction du framerate
		static double fixedTimestepAccumulatorRatio;

		//Retourne la position smooth�e en fonction de framerate de m_body
		b2Vec2 getPositionSmoothed();
		//Retourne l'angle smooth� en fonction de framerate de m_body
		double getAngleSmoothed();
		//R�initialise les valeurs apr�s un smooth
		void clearSmoothAngleAndPosition();
	protected :
		//Body of the physical object
		b2Body * m_body;
		//Sprite of the physical Object
		Sprite * m_sprite;

		//Position de m_body au dernier pas de simulation
		b2Vec2 m_previousPosition;
		//Angle de m_body au dernier pas de simulation
		float m_previousAngle;
};
#endif