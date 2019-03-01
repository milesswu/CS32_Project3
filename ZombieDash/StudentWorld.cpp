#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	m_penelope = nullptr;
	m_alive = 0;
	m_completeLevel = false;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
	cerr << "Destroying the StudentWorld object" << endl;
}

int StudentWorld::init()
{
	Level lev(assetPath());
	int levNum = getLevel(); //TODO
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << levNum << ".txt";
	Level::LoadResult result = lev.loadLevel(oss.str());
	if (result == Level::load_fail_file_not_found || getLevel() > 99) {
		cerr << "Cannot find " << oss.str() << " data file" << endl;
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
	}
	m_alive = 0;
	for (int i = 0; i < LEVEL_HEIGHT; i++) {
		for (int j = 0; j < LEVEL_WIDTH; j++) {
			Level::MazeEntry entry = lev.getContentsOf(i, j);
			//cerr << "Testing: " << entry << endl;
			switch (entry) {
			case Level::empty:
			{
				break;
			}
			case Level::smart_zombie:
			{
				cerr << "Creating a Smart Zombie" << endl;
				Actor* smart = new SmartZombie(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(smart);
				break;
			}
			case Level::dumb_zombie:
			{
				cerr << "Creating a Dumb Zombie" << endl;
				Actor* dumb = new DumbZombie(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(dumb);
				break;
			}
			case Level::player:
			{
				cerr << "Creating Penelope" << endl;
				m_penelope = new Penelope(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				break;
			}
			case Level::citizen:
			{
				m_alive++;
				cerr << "Creating a Citizen" << endl;
				Actor* citizen = new Citizen(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(citizen);
				break;
			}
			case Level::exit:
			{
				cerr << "Creating an Exit" << endl;
				Actor* exit = new Exit(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(exit);
				break;
			}
			case Level::wall:
			{
				cerr << "Creating a Wall" << endl;
				Actor* wall = new Wall(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(wall);
				break;
			}
			case Level::pit:
			{
				cerr << "Creating a Pit" << endl;
				Actor* pit = new Pit(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(pit);
				break;
			}
			case Level::vaccine_goodie:
			{
				Actor* vGoodie = new VaccineGoodie(this, i * SPRITE_WIDTH, j* SPRITE_HEIGHT);
				m_actors.push_back(vGoodie);
				break;
			}
			case Level::gas_can_goodie:
			{
				Actor* gGoodie = new GasCanGoodie(this, i * SPRITE_WIDTH, j* SPRITE_HEIGHT);
				m_actors.push_back(gGoodie);
				break;
			}
			case Level::landmine_goodie:
			{
				Actor* lGoodie = new LandmineGoodie(this, i * SPRITE_WIDTH, j* SPRITE_HEIGHT);
				m_actors.push_back(lGoodie);
				break;
			}
			default:
				break;
			}
		}
	}
	cerr << "Citizens to save: " << m_alive << endl;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	ostringstream oss;

	if (getScore() < 0)
		oss.fill(' ');
	else
		oss.fill('0');
	oss << "Score: " << setw(6) << getScore();
	oss.fill(' ');
	oss << "  Level: " << getLevel() 
		<< "  Lives: " << getLives()
		<< "  Vacc: "<< m_penelope->getVaccines()
		<< "  Flames: " << m_penelope->getGas()
		<< "  Mines: " << m_penelope->getLandmines()
		<< "  Infected: " << m_penelope->infectionCount()
		<< endl;
	setGameStatText(oss.str());
	if (!m_penelope->isDead()) {
		m_penelope->doSomething();
	}
	if (m_penelope->isDead()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end()) {
		(*it)->doSomething();
		if (m_penelope->isDead()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (m_completeLevel == true) {
			m_completeLevel = false;
			return GWSTATUS_FINISHED_LEVEL;
		}
		checkDead(it);
		if (it != m_actors.end()) {
			it++;
		}
	}
	//checkDead();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_penelope;
	list<Actor*>::iterator del;
	del = m_actors.begin();
	while (del != m_actors.end())
	{
		delete (*del);
		(*del) = nullptr;
		del = m_actors.erase(del);
	}
}

void StudentWorld::killPenelope()
{
	m_penelope->kill();
}

void StudentWorld::infectPenelope()
{
	m_penelope->infectInfectable();
}

bool StudentWorld::checkCollisions(double x, double y, Actor* curr) const
{
	list<Actor*>::const_iterator it = m_actors.begin();
	
	while (it != m_actors.end())
	{
		if (((*it)->hasCollision() == false) || ((*it) == curr)){
			//cerr << "doesnt" << endl;
			it++;
			continue;
		}
		double upperBound = (*it)->getY() + SPRITE_HEIGHT;
		double lowerBound = (*it)->getY() - SPRITE_HEIGHT;
		double leftBound = (*it)->getX() - SPRITE_WIDTH;
		double rightBound = (*it)->getX() + SPRITE_WIDTH;
		if (x > leftBound && x < rightBound && y < upperBound && y > lowerBound)
			return true;
		it++;
	}
	return false;
}

bool StudentWorld::checkCollisionWithPenelope(double x, double y) const
{
	double upperBound = m_penelope->getY() + SPRITE_HEIGHT;
	double lowerBound = m_penelope->getY() - SPRITE_HEIGHT;
	double leftBound = m_penelope->getX() - SPRITE_WIDTH;
	double rightBound = m_penelope->getX() + SPRITE_WIDTH;
	if (x > leftBound && x < rightBound && y < upperBound && y > lowerBound)
		return true;
	return false;
}

bool StudentWorld::checkOverlap(double x, double y, char type) {
	list<Actor*>::iterator it = m_actors.begin();
	double hDist = 0;
	double vDist = 0;
	while (it != m_actors.end())
	{
		hDist = ((*it)->getX() - x) * ((*it)->getX() - x);
		vDist = ((*it)->getY() - y) * ((*it)->getY() - y);
		if (vDist + hDist <= OVERLAP_DISTANCE) {
			switch (type) {
			case 'e':
				if ((*it)->isInfectable()) {
					escape(it);
				}
				break;
			case 'h':
				if ((*it)->isDamageable()) {
					(*it)->kill();
				}
				break;
			case 'l':
				if ((*it)->doesMove())
				return true;
				break;
			case 'f':
				if ((*it)->blockFlames())
					return true;
				break;
			case 'v':
				if ((*it)->blockVomit())
					return true;
				break;
			case 'i':
				if ((*it)->isInfectable())
					(*it)->infectInfectable();
				break;
			case 'z':
				if ((*it)->isInfectable()) {
					return (randInt(1, 3) == 1);
				}
				break;
			default:
				return true;
			}
		}
		it++;
	}
	return false;
}

bool StudentWorld::checkOverlapWithPenelope(double x, double y) const
{
	double pDist = distanceToPenelope(x, y);
	if (pDist <= OVERLAP_DISTANCE) {
		return true;
	}
	return false;
}

//function called whenever a citizen escapes (overlaps with exit)
void StudentWorld::escape(list<Actor*>::iterator& escapee)
{
	increaseScore(500);
	decAlive();
	playSound(SOUND_CITIZEN_SAVED); 
	(*escapee)->setDead();
}

bool StudentWorld::findNearestInfectable(double x, double y, Actor* origin) 
{
	double mDist = 6400;
	double actorX = 0;
	double actorY = 0;
	double hDist = 0;
	double vDist = 0;
	Actor* targetActor = nullptr;
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end())
	{
		actorX = (*it)->getX();
		actorY = (*it)->getY();
		hDist = ((actorX - x) * (actorX - x));
		vDist = ((actorY - y) * (actorY - y));
		if ((*it)->isInfectable()) {
			if (hDist + vDist <= TARGET_RANGE && hDist + vDist <= mDist) {
				mDist = hDist + vDist;
				targetActor = (*it);
			}
		}
		it++;
	}
	if (targetPenelope(x, y, mDist, origin)) {
		return true;
	}
	if (targetActor == nullptr) {
		return false;
	}
	setClosestDirection(targetActor->getX(), targetActor->getY(), origin);
	return true;
}

double StudentWorld::findNearestZombie(double x, double y) 
{
	double mDist = 6401;
	double actorX = 0;
	double actorY = 0;
	double hDist = 0;
	double vDist = 0;
	Actor* targetActor = nullptr;
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end())
	{
		actorX = (*it)->getX();
		actorY = (*it)->getY();
		hDist = ((actorX - x) * (actorX - x));
		vDist = ((actorY - y) * (actorY - y));
		if ((*it)->doesMove() && (!(*it)->isInfectable())) {
			if (hDist + vDist <= TARGET_RANGE && hDist + vDist <= mDist) {
				mDist = hDist + vDist;
				targetActor = (*it);
			}
		}
		it++;
	}
	return mDist;
}
bool StudentWorld::targetPenelope(double x, double y, double currDist, Actor* origin) 
{
	double pDist = distanceToPenelope(x, y);
	if (pDist <= TARGET_RANGE && pDist <= currDist) {
			setClosestDirection(m_penelope->getX(), m_penelope->getY(), origin);
		return true;
	}
	return false;
}

void StudentWorld::setClosestDirection(double actorX, double actorY, Actor* origin)
{
	//cerr << "setting direction" << endl;
	double originX = origin->getX();
	double originY = origin->getY();
	if (originX == actorX) {
		if (originY <= actorY)
			origin->setDirection(90);
		else
			origin->setDirection(270);
		return;
	}
	if (originY == actorY) {
		if (originX <= actorX)
			origin->setDirection(0);
		else
			origin->setDirection(180);
		return;
	}
	int hor_ver = randInt(1, 2);
	switch (hor_ver) {
	case 1: //horizontal
		if (originX <= actorX)
			origin->setDirection(0);
		else
			origin->setDirection(180);
		break;
	case 2: //vertical
		if (originY <= actorY)
			origin->setDirection(90);
		else
			origin->setDirection(270);
		break;
	}
	return;
}

double StudentWorld::distanceToPenelope(double x, double y) const
{
	double pxDist = (m_penelope->getX() - x) * (m_penelope->getX() - x);
	double pyDist = (m_penelope->getY() - y) * (m_penelope->getY() - y);
	return pxDist + pyDist;
}

//function called whenever penelope overlaps with a goodie
void StudentWorld::pickupGoodie(char goodie)
{
	switch (goodie) {
	case 'v':
		m_penelope->incVaccines();
		break;
	case 'g':
		m_penelope->incGas();
		break;
	case 'l':
		m_penelope->incLandmine();
		break;
	default:
		return;
	}
	playSound(SOUND_GOT_GOODIE);
}

void StudentWorld::explode(double x, double y)
{
	cerr << "explosion" << endl;
	Actor* landminePit = new Pit(this, x, y);
	m_actors.push_back(landminePit);
	createFlame(x + SPRITE_WIDTH, y, 90);
	createFlame(x - SPRITE_WIDTH, y, 90);
	createFlame(x, y + SPRITE_HEIGHT, 90);
	createFlame(x, y - SPRITE_HEIGHT, 90);
	createFlame(x + SPRITE_WIDTH, y + SPRITE_HEIGHT, 90);
	createFlame(x + SPRITE_WIDTH, y - SPRITE_HEIGHT, 90);
	createFlame(x - SPRITE_WIDTH, y + SPRITE_HEIGHT, 90);
	createFlame(x - SPRITE_WIDTH, y - SPRITE_HEIGHT, 90);
}
//

//returns true if successfully created a vaccine at x and y
bool StudentWorld::createVaccine(double x, double y)
{
	if (checkOverlap(x, y, 'a') || checkOverlapWithPenelope(x, y))
		return false;
	cerr << "Creating a Vaccine" << endl;
	Actor* vacc = new VaccineGoodie(this, x, y);
	m_actors.push_back(vacc);
	return true;
}

//returns true if created a flame at specified x and y
bool StudentWorld::createFlame(double x, double y, int dir) 
{
	if (checkOverlap(x, y, 'f'))
		return false;
	cerr << "Creating a Flame" << endl;
	Actor* flame = new Flame(this, x, y, dir);
	m_actors.push_back(flame);
	return true;
}

bool StudentWorld::createVomit(double x, double y, int dir)
{
	if ((randInt(1, 3) != 1))
		return false;
	if (checkOverlap(x, y, 'v'))
		return false;
	cerr << "Creating Vomit" << endl;
	Actor* vomit = new Vomit(this, x, y, dir);
	m_actors.push_back(vomit);
	return true;
}

void StudentWorld::createZombie(double x, double y, int dir)
{
	cerr << "Zombie Born" << endl;
	Actor* zombie = nullptr;
	int random = randInt(1, 10);
	if (random > 3 && random <= 10)
		zombie = new DumbZombie(this, x, y, dir);
	else
		zombie = new SmartZombie(this, x, y, dir);
	m_actors.push_back(zombie);
	playSound(SOUND_ZOMBIE_BORN);
}

void StudentWorld::deployMine(double x, double y)
{
	Actor* landmine = new Landmine(this, x, y);
	m_actors.push_back(landmine);
}

void StudentWorld::shootFlamethrower(int dir)
{
	double playerX = m_penelope->getX();
	double playerY = m_penelope->getY();
	double flameX = playerX;
	double flameY = playerY;
	int i = 1;
	while (i < 4) {
		switch (dir) {
		case 90:
			flameY = playerY + (i * SPRITE_HEIGHT);
			break;
		case 270:
			flameY = playerY - (i * SPRITE_HEIGHT);
			break;
		case 180:
			flameX = playerX - (i * SPRITE_WIDTH);
			break;
		case 0:
			flameX = playerX + (i * SPRITE_WIDTH);
			break;
		default:
			return;
		}
		if (!createFlame(flameX, flameY, dir))
			break;
		i++;
	}
}

//checks each actor to see if any actors died during the current tick, if so delete them and erase from the list of current actors
void StudentWorld::checkDead(list<Actor*>::iterator& currIt) {
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end()) {
		if ((*it)->isDead()) {
			delete (*it);
			if (it == currIt)
				it = currIt = m_actors.erase(it);
			else
				it = m_actors.erase(it);
		}
		else {
			it++;
		}
	}
}