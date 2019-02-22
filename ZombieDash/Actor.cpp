#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/**********************************************************************************************************************************************************
																		ACTOR MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/
//Actor Destructor
Actor::~Actor()
{
	cerr << "Destroying Actor Object" << endl;
}


/**********************************************************************************************************************************************************
																		PLAYER MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

//Player Destructor
Player::~Player()
{
	cerr << "Destroying Player Object" << endl;
}

//Helper function to set new direction and location when a Player object moves
void Player::move(Direction dir, double x, double y)
{
	setDirection(dir);
	//cerr << "Penelope (x, y): (" << getX() << ", " << getY() << ")" << endl;
	//cerr << "Penelope max(?) (x, y): (" << getX() + SPRITE_WIDTH - 1 << ", " << getY() + SPRITE_HEIGHT - 1 << ")" << endl;
	if (getWorld()->checkForCollisions(dir, getX(), getY()))
		return;
	moveTo(x, y);
}

//InfectablePlayer Destructor
InfectablePlayer::~InfectablePlayer()
{
	cerr << "Destroying Infectable Player Object" << endl;
}


/**********************************************************************************************************************************************************
																		PENELOPE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

//Penelope Destructor
Penelope::~Penelope()
{
	cerr << "Deleting Penelope" << endl;
}

//Penelope doSomething()
void Penelope::doSomething()
{
	if (isDead())
		return;
	if (infect())
		return;
	int keyPress;
	if (getWorld()->getKey(keyPress)) {
		switch (keyPress) {
		case KEY_PRESS_LEFT:
			move(left, getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			move(right, getX() + 4, getY());
			break;
		case KEY_PRESS_DOWN:
			move(down, getX(), getY() - 4);
			break;
		case KEY_PRESS_UP:
			move(up, getX(), getY() + 4);
			break;
		}
	}
}

/**********************************************************************************************************************************************************
																		CITIZEN MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/



/**********************************************************************************************************************************************************
																		ZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/




/**********************************************************************************************************************************************************
																		DUMBZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/




/**********************************************************************************************************************************************************
																		SMARTZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/




/**********************************************************************************************************************************************************
																	ENVIRONMENT OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/

Environment::~Environment()
{
	cerr << "Destorying an Environment Object" << endl;
}

Wall::~Wall()
{
	cerr << "Destroying a Wall" << endl;
}

Exit::~Exit()
{
	cerr << "Destroying an Exit" << endl;
}

void Exit::doSomething()
{
	getWorld()->overlapWithExit(getX(), getY());
	return;
}

/**********************************************************************************************************************************************************
																	HAZARD OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/

Hazard::~Hazard()
{
	cerr << "Destroying a Hazard Object" << endl;
}

Pit::~Pit()
{
	cerr << "Destoying a Pit" << endl;
}

void Pit::doSomething()
{
	getWorld()->overlapWithHazard(getX(), getY());
}

/**********************************************************************************************************************************************************
																GOODIE OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/

Goodie::~Goodie()
{
	cerr << "Destroying a Goodie Object" << endl;
}

VaccineGoodie::~VaccineGoodie()
{
	cerr << "Destorying a Vaccine" << endl;
}

void VaccineGoodie::doSomething()
{
	if (getWorld()->pickupGoodie(getX(), getY(), 'v')) {
		getWorld()->increaseScore(50);
		getWorld()->killActor(this);
	}
}

GasCanGoodie::~GasCanGoodie()
{
	cerr << "Destroying a Gas Can" << endl;
}

void GasCanGoodie::doSomething()
{
	if (getWorld()->pickupGoodie(getX(), getY(), 'g')) {
		getWorld()->increaseScore(50);
		getWorld()->killActor(this);
	}
}

LandmineGoodie::~LandmineGoodie()
{
	cerr << "Destroying a Landmine Goodie" << endl;
}

void LandmineGoodie::doSomething()
{
	if (getWorld()->pickupGoodie(getX(), getY(), 'l')) {
		getWorld()->increaseScore(50);
		getWorld()->killActor(this);
	}
}

/**********************************************************************************************************************************************************
																PROJECTILE OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/
