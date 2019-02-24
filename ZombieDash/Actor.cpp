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
	if (getWorld()->checkWallCollisions(dir, getX(), getY()))
		return;
	moveTo(x, y);
}

//InfectablePlayer Destructor
InfectablePlayer::~InfectablePlayer()
{
	cerr << "Destroying Infectable Player Object" << endl;
}

void InfectablePlayer::doSomething()
{
	if (incInfection() == 500) {
		getWorld()->createZombie(getX(), getY(), getDirection());
		kill();
	}
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
		case KEY_PRESS_ENTER:
			useVaccine();
			break;
		case KEY_PRESS_SPACE:
			shootFlamethrower(getDirection());
			break;
		case KEY_PRESS_TAB:
			deployMine();
			break;
		}
	}
}

void Penelope::shootFlamethrower(Direction dir)
{
	if (getGas() <= 0)
		return;
	getWorld()->shootFlamethrower(dir);
	m_gasCount--;

}

void Penelope::deployMine()
{
	if (getLandmines() <= 0)
		return;
	getWorld()->deployMine(getX(), getY());
	m_landmineCount--;
}

void Penelope::kill()
{
	setDead();
}

/**********************************************************************************************************************************************************
																		CITIZEN MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/



/**********************************************************************************************************************************************************
																		ZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

Zombie::~Zombie()
{
	cerr << "Destroying a Zombie Object" << endl;
}

void Zombie::doSomething()
{
	return;
}

void Zombie::kill()
{
	getWorld()->increaseScore(1000);
	setDead();
}
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
	getWorld()->checkOverlap(getX(), getY(), 'e');
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		if (getWorld()->leftAlive() == 0)
			getWorld()->completeLevel();
	}
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
	getWorld()->checkOverlap(getX(), getY(), 'h');
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->killPenelope();
	}
}

Landmine::~Landmine()
{
	cerr << "Destroying a Landmine" << endl;
}

void Landmine::doSomething()
{
	if (isActive() == false) {
		decSafety();
		if (getSafety() == 0)
			activate();
	}
	else {
		if (getWorld()->checkOverlapWithPenelope(getX(), getY()) || getWorld()->checkOverlap(getX(), getY(), 'l')) {
			kill();
		}
	}
}

void Landmine::kill()
{
	getWorld()->explode(getX(), getY());
	setDead();
}

/**********************************************************************************************************************************************************
																GOODIE OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/

Goodie::~Goodie()
{
	cerr << "Destroying a Goodie Object" << endl;
}

void Goodie::doSomething()
{
	getWorld()->increaseScore(50);
	kill();
}

void Goodie::kill()
{
	setDead();
}

VaccineGoodie::~VaccineGoodie()
{
	cerr << "Destorying a Vaccine" << endl;
}

void VaccineGoodie::doSomething()
{
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->pickupGoodie('v');
		Goodie::doSomething();
	}

	if (get_iTicks() != 0)
		dec_iTicks();
}

GasCanGoodie::~GasCanGoodie()
{
	cerr << "Destroying a Gas Can" << endl;
}

void GasCanGoodie::doSomething()
{
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->pickupGoodie('g');
		Goodie::doSomething();
	}

	if (get_iTicks() != 0)
		dec_iTicks();
}

LandmineGoodie::~LandmineGoodie()
{
	cerr << "Destroying a Landmine Goodie" << endl;
}

void LandmineGoodie::doSomething()
{
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->pickupGoodie('l');
		Goodie::doSomething();
	}

	if (get_iTicks() != 0)
		dec_iTicks();
}

/**********************************************************************************************************************************************************
																PROJECTILE OBJECT IMPLEMENTATIONS
***********************************************************************************************************************************************************
*/

Projectile::~Projectile()
{
	cerr << "Destroying a Projectile Object" << endl;
}

void Projectile::doSomething()
{
	if (getLife() == 0) {
		kill();
		return;
	}
	decLife();
}

void Projectile::kill()
{
	setDead();
}

Vomit::~Vomit()
{
	cerr << "Destroying Vomit" << endl;
}

void Vomit::doSomething()
{
	getWorld()->checkOverlap(getX(), getY(), 'v');
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->infectPenelope();
	}
	Projectile::doSomething();
}

Flame::~Flame()
{
	cerr << "Destroying a Flame" << endl;
}

void Flame::doSomething()
{
	getWorld()->checkOverlap(getX(), getY(), 'h');
	if (getWorld()->checkOverlapWithPenelope(getX(), getY())) {
		getWorld()->killPenelope();
	}
	Projectile::doSomething();
}
