#include "Wagon.hpp"
/*
 * Wagon Constructor 
 */

Wagon::Wagon (int posX) 
{
	m_originalNumberOfPassengers=0;
	//Maximum number of passenger for the wagon
	m_maxCapacity=20;
	// Number of passengers actually in the wagon (not ejected ones)
	m_passengersCount=0;
	
	m_areDoorOpened= false;

	SDL_Rect * pos = new SDL_Rect;
	pos->x = posX; pos->y=0;
	SDL_Rect * posr1 = new SDL_Rect;
	posr1->x = posX+10; posr1->y=0;
	SDL_Rect * posr2 = new SDL_Rect;
	posr2->x = posX+50; posr2->y=0;
	SDL_Rect * size = new SDL_Rect;
	size->x = 138; size->y = 97;

	PhysicalObject * wagon = new PhysicalObject(new Sprite("../img/elements/wagon.png",  pos,  size) );
	m_physicalObjects.push_back(wagon);
	size->x = 39; size->y = 36; 
	PhysicalObject * roue1 = new PhysicalObject(new Sprite("../img/elements/roue.png",  posr1,  size) );
	m_physicalObjects.push_back(roue1);
	PhysicalObject * roue2 = new PhysicalObject(new Sprite("../img/elements/roue.png",  posr2,  size) );
	m_physicalObjects.push_back(roue2);


}

Wagon::~Wagon () 
{
	for(int i = 0; i < m_physicalObjects.size(); ++i)
	{
		delete m_physicalObjects[i];
	}
}

b2Body * Wagon::getBody(unsigned int i)
{
	assert(i < m_physicalObjects.size());
	return m_physicalObjects[i]->getBody();
}

/*
 * Réinitialise les valeurs des PhysicalObjects après un smooth pour coller au framerate
 */
void Wagon::clearAllSmoothAngleAndPosition()
{
	for(int i = 0; i < m_physicalObjects.size(); ++i)
	{
		m_physicalObjects[i]->clearSmoothAngleAndPosition();
	}

	for (std::list<Passenger*>::iterator it = m_passengers.begin(); it != m_passengers.end(); it++)
		(*it)->clearSmoothAngleAndPosition();
}

/*
 * draw the wagon on the screen
 */
void Wagon::drawSprite(SDL_Surface * screen, const int & width, const int & height, int posX)
{
	double x, y; 
	b2Vec2 bodyPos;
	double angle,angledegrees;
	bodyPos = m_physicalObjects[0]->getPosition();
	angle = m_physicalObjects[0]->getAngle();
	angledegrees= angle*180/M_PI;
	y = bodyPos.y;
	m_physicalObjects[0]->getSprite()->convertMetersToPixelsY( &y,  width,  height);
	if (angle>=0){
		y = y-30*sin(angle)-35*cos(angle);
	}
	else {
		y = y+30*sin(M_PI-angle)+35*cos(M_PI-angle);
	}
	m_physicalObjects[0]->getSprite()->setPositionY( y);
	m_physicalObjects[0]->getSprite()->setAngle(angledegrees);
	m_physicalObjects[0]->getSprite()->draw(screen, width, height);

	//roues
	for (int i=1; i<3; i++){
		bodyPos = m_physicalObjects[i]->getPosition();
		y = bodyPos.y;
		m_physicalObjects[i]->getSprite()->convertMetersToPixelsY( &y,  width,  height); 
		angle = m_physicalObjects[0]->getAngle();
		if (angle>=0){
			x= posX+10*cos(angle)+(i-1)*40*cos(angle);
			y = y-8*sin(angle)-8*cos(angle);
		}
		else {
			x= posX-10*cos(angle)+(i-1)*40*cos(angle);
			y = y-8*sin(M_PI-angle)+8*cos(M_PI-angle);
		}
		m_physicalObjects[i]->getSprite()->setPosition(x, y);
		angle = m_physicalObjects[i]->getAngle()*180/M_PI;
		m_physicalObjects[i]->getSprite()->setAngle(angle);
		m_physicalObjects[i]->getSprite()->draw(screen, width, height);
	}


}

void Wagon::setMotorSpeed(float speed)
{
	m_spring1->SetMotorSpeed(speed);
	m_spring2->SetMotorSpeed(speed);
}



/*
 * wagon construction
 */
void Wagon::build(b2World * world, double x, float high)
{	
	//Création des wagons
	float32 hz = 4.0f;
	float32 zeta = 0.7f;
	float32 speed = 2.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x, high+9.0f);

	m_physicalObjects[0]->setBody(world->CreateBody(&bd));

	int typeWagon = 1;
	if( typeWagon == 0 )
	{
		//Le code qui donne un rectangle tout simple
		b2PolygonShape chassis;
		b2Vec2 vertices[4];
		vertices[0].Set(-1.5f, -0.8f);
		vertices[1].Set(1.5f, -0.8f);
		vertices[2].Set(1.5f, 0.8f);
		vertices[3].Set(-1.5f, 0.8f);
		chassis.Set(vertices, 4);

		m_physicalObjects[0]->getBody()->CreateFixture(&chassis, 5.0f);
	}else if( typeWagon == 1 )
	{
		//avec des polygones
		b2PolygonShape frameLeft;
		b2Vec2 vertices[4];
		vertices[0].Set(-1.6f, -0.8f);
		vertices[1].Set(-1.5f, -0.8f);
		vertices[2].Set(-1.5f, 0.8f);
		vertices[3].Set(-1.6f, 0.8f);
		frameLeft.Set(vertices, 4);
		b2PolygonShape frameBot;
		vertices[0].Set(-1.6f, -0.9f);
		vertices[1].Set(1.6f, -0.9f);
		vertices[2].Set(1.6f, -0.8f);
		vertices[3].Set(-1.6f, -0.8f);
		frameBot.Set(vertices, 4);

		b2PolygonShape frameRight;
		vertices[0].Set(1.5f, -0.9f);
		vertices[1].Set(1.6f, -0.9f);
		vertices[2].Set(1.6f, 0.9f);
		vertices[3].Set(1.5f, 0.9f);
		frameRight.Set(vertices, 4);
		//*
		b2PolygonShape frameRoof;
		vertices[0].Set(-1.6f, 0.8f);
		vertices[1].Set(1.6f, 0.8f);
		vertices[2].Set(1.6f, 0.9f);
		vertices[3].Set(-1.6f, 0.9f);
		frameRoof.Set(vertices, 4);
		b2FixtureDef fixtureRoof;
		fixtureRoof.shape = &frameRoof;
		fixtureRoof.density = 5.0f;
		fixtureRoof.filter.groupIndex = -1; // So it doesn't collide with passengers
		//*/
	
		m_physicalObjects[0]->getBody()->CreateFixture(&frameBot, 5.0f);
		m_physicalObjects[0]->getBody()->CreateFixture(&frameLeft, 5.0f);
		m_physicalObjects[0]->getBody()->CreateFixture(&frameRight, 5.0f);
		m_physicalObjects[0]->getBody()->CreateFixture(&fixtureRoof);
		//m_physicalObjects[0]->getBody()->CreateFixture(&chassisToit, 1.0f);
	}
	b2CircleShape circle;
	circle.m_radius = 0.5f;

	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 3.0f;
	fd.friction = 0.9f;

	bd.position.Set(x-1.0f, high+7.9f);
	m_physicalObjects[1]->setBody(world->CreateBody(&bd));
	m_physicalObjects[1]->getBody()->CreateFixture(&fd);// roue1

	bd.position.Set(x+1.0f, high+7.9f);
	m_physicalObjects[2]->setBody(world->CreateBody(&bd));
	m_physicalObjects[2]->getBody()->CreateFixture(&fd);// roue2

	b2WheelJointDef jd;
	b2Vec2 axis(0.0f, 1.0f);

	jd.Initialize(m_physicalObjects[0]->getBody(), m_physicalObjects[1]->getBody(), m_physicalObjects[1]->getBody()->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = true;
	jd.frequencyHz = hz;
	jd.dampingRatio = zeta;
	m_spring1 = (b2WheelJoint*)world->CreateJoint(&jd);// joint pour la roue1

	jd.Initialize(m_physicalObjects[0]->getBody(), m_physicalObjects[2]->getBody(), m_physicalObjects[2]->getBody()->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.frequencyHz = hz;
	jd.dampingRatio = zeta;
	m_spring2 = (b2WheelJoint*)world->CreateJoint(&jd);// joint pour la roue2


	//TEST Création d'un passager A RETIRER PLUS TARD
	for(int i = 0; i< 20; i++ ){
		Passenger *p = new Passenger(0.0f, 0.0f);
		addPassenger(p);

		//ejectPassenger(p);
	}//*/
}

// Add the Passenger also creates the passenger joint. Call checkCapacity at the end.
void Wagon::addPassenger(Passenger* p)
{
	m_passengers.push_back(p);
	++ m_passengersCount;
	p->switchDynamic();

	// On le déplace dans le wagon
	// On le place selon le nombre de passagers déja présents
	float shift = ((float)m_passengersCount / 5) - m_passengersCount / 5;
	std::clog<<m_passengersCount<<"shift : "<<shift<<std::endl;
	b2Vec2 pos(m_physicalObjects[0]->getBody()->GetPosition().x - 1.1f + (float)shift * 2.7f, m_physicalObjects[0]->getBody()->GetPosition().y +0.1f );
	p->getBody()->SetTransform(pos, 0.0);

	//Création du joint pour lier le passager au wagon
	b2RevoluteJointDef jointDef;
	b2Vec2 worldAnchorPassenger(p->getBody()->GetPosition().x, p->getBody()->GetPosition().y - p->m_height/2.0f + p->m_width/4.0f );
	jointDef.Initialize(p->getBody(), m_physicalObjects[0]->getBody(), worldAnchorPassenger);

	//On limite l'ampleur de l'angle
	jointDef.lowerAngle = -0.15f * b2_pi; // -45 degrees
	jointDef.upperAngle = 0.15f * b2_pi; // 45 degrees

	jointDef.enableLimit = true;

	p->setJoint( PhysicalObject::m_world->CreateJoint(&jointDef) );
	//checkCapacity();
}

// Eject the passenger by removing its joint, set the passenger’s “isEjected” to true. After a timer of 5 sec, we call deletePassenger().
void Wagon::ejectPassenger(Passenger* p)
{
	PhysicalObject::m_world->DestroyJoint(p->getJoint());
	p->setJoint(NULL);
	p->setIsEjected(true);
	m_passengersCount --;

	//On lui met une force vers le haut pour qu'il soit ejecté
	p->getBody()->ApplyForce( b2Vec2(-10, 40), p->getBody()->GetPosition() );

	//timer de 5 sec
	
	//deletePassenger(p);
}

void Wagon::deletePassenger(Passenger* p)
{
	// TODO On vérifie que le passager est hors de l'écran
	m_passengers.remove(p);
	delete p;
	p = NULL;
}