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
	if (getWorld()->checkForCollisions(x, y))
		return;
	setDirection(dir);
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
	if (isInfected()) {
		if (incInfection() == 500) {
			setDead();
			return;
		}
	}
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
																		ENVIRONMENT MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

Environment::~Environment()
{
	cerr << "Destorying an Environment Object" << endl;
}


/**********************************************************************************************************************************************************
																		WALL MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

Wall::~Wall()
{
	cerr << "Destroying a Wall" << endl;
}

/**********************************************************************************************************************************************************
																		PROJECTILE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/
