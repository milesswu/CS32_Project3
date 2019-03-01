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
bool Player::move(Direction dir, double x, double y)
{
	setDirection(dir);
	if (getWorld()->checkCollisions(x, y, this) || getWorld()->checkCollisionWithPenelope(x, y))
		return false;
	moveTo(x, y);
	return true;
}

//InfectablePlayer Destructor
InfectablePlayer::~InfectablePlayer()
{
	cerr << "Destroying Infectable Player Object" << endl;
}

void InfectablePlayer::doSomething()
{
	if (infectionCount() == INFECT_LEVEL) {
		getWorld()->createZombie(getX(), getY(), getDirection());
		kill();
		return;
	}
	if (isInfected())
		incInfection();
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
	InfectablePlayer::doSomething();
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


bool Penelope::move(Direction dir, double x, double y)
{
	setDirection(dir);
	if (getWorld()->checkCollisions(x, y, this))
		return false;
	moveTo(x, y);
	return true;
}


void Penelope::shootFlamethrower(Direction dir)
{
	if (getGas() <= 0)
		return;
	getWorld()->shootFlamethrower(dir);
	getWorld()->playSound(SOUND_PLAYER_FIRE);
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
	getWorld()->playSound(SOUND_PLAYER_DIE);
	setDead();
}

/**********************************************************************************************************************************************************
																		CITIZEN MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

Citizen::~Citizen()
{
	cerr << "Destroying a Citizen" << endl;
}

void Citizen::doSomething()
{
	if (isDead())
		return;
	InfectablePlayer::doSomething();
	if (isParalyzed()) {
		changeState();
		return;
	}
	changeState();
	double chooseUp, chooseDown, chooseLeft, chooseRight;
	chooseUp = chooseDown = chooseLeft = chooseRight = 0;
	double maxDist = 0;
	Direction newDir = right;

	//Check collisions with actors in all directions, if no collision detected, compute which direction would bring citizen farthest from closest zombie
	if (!getWorld()->checkCollisions(getX(), getY() + 2, this) && !getWorld()->checkCollisionWithPenelope(getX(), getY() +21)) {
		chooseUp = getWorld()->findNearestZombie(getX(), getY() + 2);
		if (chooseUp > maxDist) {
			maxDist = chooseUp;
			newDir = up;
		}
		else if (chooseUp == maxDist)
		{
			newDir = -1;
		}
	}
	if (!getWorld()->checkCollisions(getX(), getY() - 2, this) && !getWorld()->checkCollisionWithPenelope(getX(), getY() - 2)) {
		chooseDown = getWorld()->findNearestZombie(getX(), getY() - 2);
		if (chooseDown > maxDist) {
			maxDist = chooseDown;
			newDir = down;
		}
		else if (chooseDown == maxDist)
		{
			newDir = -1;
		}
	}
	if (!getWorld()->checkCollisions(getX() - 2, getY(), this) && !getWorld()->checkCollisionWithPenelope(getX() - 2, getY())) {
		chooseLeft = getWorld()->findNearestZombie(getX() - 2, getY());
		if (chooseLeft > maxDist) {
			maxDist = chooseLeft;
			newDir = left;
		}
		else if (chooseLeft == maxDist)
		{
			newDir = -1;
		}
	}
	if (!getWorld()->checkCollisions(getX() + 2, getY(), this) && !getWorld()->checkCollisionWithPenelope(getX() + 2, getY())) {
		chooseRight = getWorld()->findNearestZombie(getX() + 2, getY());
		if (chooseRight > maxDist) {
			maxDist = chooseRight;
			newDir = right;
		}
		else if (chooseRight == maxDist)
		{
			newDir = -1;
		}
	}

	//target penelope if she is closer than any zombie
	double pDist = getWorld()->distanceToPenelope(getX(), getY());
	if (getWorld()->targetPenelope(getX(), getY(), maxDist, this)) {
		newDir = getDirection();
	}
	else if (maxDist > 6400)
		return;
	switch (newDir) {
	case up:
		if (move(up, getX(), getY() + 2))
		break;
	case down:
		if (move(down, getX(), getY() - 2))
		break;
	case right:
		if (move(right, getX() + 2, getY()))
		break;
	case left:
		if (move(left, getX() - 2, getY()))
		break;
	default:
		break;
	}
}

void Citizen::infectInfectable()
{
	if (isInfected() == false)
		getWorld()->playSound(SOUND_CITIZEN_INFECTED);
	InfectablePlayer::infectInfectable();
}

void Citizen::kill()
{
	getWorld()->increaseScore(-1000);
	getWorld()->decAlive();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	setDead();
}


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
	if (isDead())
		return;
	if (isParalyzed()) {
		changeState();
		return;
	}
	changeState();
	if (spitVomit()) {
		getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
		return;
	}
	if (moves() == 0) {
		setMovePlan();
		changeDirection();
	}
	int dir = getDirection();
	double dest_x = getX();
	double dest_y = getY();
	switch (dir) {
	case up:
		dest_y = getY() + 1;
		break;
	case down:
		dest_y = getY() - 1;
		break;
	case right:
		dest_x = getX() + 1;
		break;
	case left:
		dest_x = getX() - 1;
		break;
	default:
		break;
	}
	if (move(dir, dest_x, dest_y)) {
		m_movePlan--;
	}
	else
		m_movePlan = 0;
}

bool Zombie::spitVomit()
{
	double vomX = getX();
	double vomY = getY();
	switch (getDirection()) {
	case up:
		vomY = getY() + SPRITE_HEIGHT;
		break;
	case down:
		vomY = getY() - SPRITE_HEIGHT;
		break;
	case left:
		vomX = getX() - SPRITE_WIDTH;
		break;
	case right:
		vomX = getX() + SPRITE_WIDTH;
		break;
	default:
		break;
	}
	if (getWorld()->checkOverlap(vomX, vomY, 'z') || getWorld()->checkOverlapWithPenelope(vomX, vomY)) {
		return getWorld()->createVomit(vomX, vomY, getDirection());
	}
	return false;
}

void Zombie::changeDirection()
{
	int rand = randInt(1, 4);
	switch (rand) {
	case 1:
		setDirection(up);
		break;
	case 2:
		setDirection(down);
		break;
	case 3:
		setDirection(right);
		break;
	case 4:
		setDirection(left);
		break;
	default:
		break;
	}
}

void Zombie::kill()
{
	getWorld()->increaseScore(getScore());
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	setDead();
}
/**********************************************************************************************************************************************************
																		DUMBZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

DumbZombie::~DumbZombie()
{
	cerr << "Destorying a Dumb Zombie" << endl;
}

void DumbZombie::kill()
{
	if (m_hasVaccine) {
		int dir = randInt(1, 4);
		double destx = getX();
		double desty = getY();
		switch (dir) {
		case 1: //right
			destx = getX() + SPRITE_WIDTH;
			break;
		case 2: //left
			destx = getX() - SPRITE_WIDTH;
			break;
		case 3: //up
			desty = getY() + SPRITE_HEIGHT;
			break;
		case 4: //down
			desty = getY() - SPRITE_HEIGHT;
			break;
		default:
			break;
		}
		cerr << "try to make a vaccine" << endl;
		getWorld()->createVaccine(destx, desty);
	}
	Zombie::kill();
}

/**********************************************************************************************************************************************************
																		SMARTZOMBIE MEMBER FUNCTIONS
***********************************************************************************************************************************************************
*/

SmartZombie::~SmartZombie()
{
	cerr << "Destroying a Smart Zombie" << endl;
}

void SmartZombie::changeDirection()
{
	if (getWorld()->findNearestInfectable(getX(), getY(), this)) {
		return;
	}
	Zombie::changeDirection();
}

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
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
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
	getWorld()->checkOverlap(getX(), getY(), 'i');
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
