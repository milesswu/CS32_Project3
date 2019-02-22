#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	m_penelope = nullptr;
	m_alive = 0;
	m_victory = false;
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
	int levNum = getLevel() + 1;
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << levNum << ".txt";
	Level::LoadResult result = lev.loadLevel(oss.str());
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find " << oss.str() << " data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
	}
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
				break;
			}
			case Level::dumb_zombie:
			{
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
				//m_alive++;
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
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score:  " << setw(6) << getScore();
	oss.fill(' ');
	oss << "  Level:" << setw(3) << getLevel() << "  Lives:"
		<< setw(3) << getLives()
		<< "  Vacc:" << setw(3) << m_penelope->getVaccines()
		<< "  Flames:" << setw(3) << m_penelope->getGas()
		<< "  Mines:" << setw(3) << m_penelope->getLandmines()
		<< "  Infected:" << setw(3) << m_penelope->infectionCount()
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
		it++;
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

void StudentWorld::killActor(Actor* kill)
{
	kill->setDead();
}

void StudentWorld::killPenelope()
{
	m_penelope->setDead();
}

bool StudentWorld::checkForCollisions(int dir, double x, double y) const
{
	list<Actor*>::const_iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		if (!((*it)->hasCollision())) {
			it++;
			continue;
		}
		double actorX = (*it)->getX();
		double actorY = (*it)->getY();	
		double distance = (pow(actorX - x, 2) + pow(actorY - y, 2));
		switch (dir) {
		///*
		case 0: //right
			if (x + SPRITE_WIDTH >= actorX && x + SPRITE_WIDTH <= actorX + SPRITE_WIDTH - 1 && y <= actorY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT >= actorY) {
				if (y + SPRITE_HEIGHT <= actorY || y >= actorY + SPRITE_HEIGHT)
					return false;
				return true;
			}
			break;
		case 90: //up
			if (y + SPRITE_HEIGHT >= actorY && y + SPRITE_HEIGHT <= actorY + SPRITE_HEIGHT - 1 && x <= actorX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH >= actorX) {
				if (x + SPRITE_WIDTH <= actorX || x >= actorX + SPRITE_WIDTH)
					return false;
				return true;
			}
			break;
		case 180: //left
			if (x <= actorX + SPRITE_WIDTH && x >= actorX && y <= actorY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT >= actorY) {
				if (y + SPRITE_HEIGHT <= actorY || y >= actorY + SPRITE_HEIGHT)
					return false;
				return true;
			}
			break;
		case 270: //down
			if (y <= actorY + SPRITE_HEIGHT && y >= actorY && x <= actorX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH >= actorX) {
				//cerr << "Actor (x, y): (" << actorX << ", " << actorY << ")" << endl;
				//cerr << "Actor max (x, y): (" << actorX + SPRITE_WIDTH << ", " << actorY + SPRITE_HEIGHT << ")" << endl;
				if (x + SPRITE_WIDTH <= actorX || x >= actorX + SPRITE_WIDTH)
					return false;
				return true;
			}
			break;
		default:
			break;
		//*/
		}
		it++;
	}
	return false;
}

/*
void StudentWorld::checkOverlap(double x, double y) const
{
	list<Actor*>::const_iterator it;
	it = m_actors.begin();
	double hDist = 0;
	double vDist = 0;
	while (it != m_actors.end())
	{
		hDist = ((*it)->getX() - x) * ((*it)->getX() - x);
		vDist = ((*it)->getY() - y) * ((*it)->getY() - y);
		if (vDist + hDist <= OVERLAP_DISTANCE) {
			return (*it);
		}
		it++;
	}
	return nullptr;
}
*/

bool StudentWorld::checkOverlapWithPenelope(double x, double y) const
{
	double hDist = (m_penelope->getX() - x) * (m_penelope->getX() - x);
	double vDist = (m_penelope->getY() - y) * (m_penelope->getY() - y);
	if (vDist + hDist <= OVERLAP_DISTANCE) {
		return true;
	}
	return false;
}

//tests if any citizens overlap with a specified exit, if so, has them escape
void StudentWorld::overlapWithExit(double x, double y)
{
	list<Actor*>::iterator it;
	it = m_actors.begin();
	double hDist = 0;
	double vDist = 0;
	while (it != m_actors.end())
	{
		if ((*it)->isInfectable()) {
			hDist = ((*it)->getX() - x) * ((*it)->getX() - x);
			vDist = ((*it)->getY() - y) * ((*it)->getY() - y);
			if (vDist + hDist <= OVERLAP_DISTANCE) {
				escape(it);
				continue;
			}
		}
		it++;
	}
}

//checks if any actors overlap with a specified hazard object
void StudentWorld::overlapWithHazard(double x, double y)
{
	list<Actor*>::iterator it = m_actors.begin();
	double hDist = 0;
	double vDist = 0;
	while (it != m_actors.end())
	{
		if ((*it)->isDamageable()) {
			hDist = ((*it)->getX() - x) * ((*it)->getX() - x);
			vDist = ((*it)->getY() - y) * ((*it)->getY() - y);
			if (vDist + hDist <= OVERLAP_DISTANCE) {
				killActor(*it);
			}
		}
		it++;
	}
}

//function called whenever a citizen escapes (overlaps with exit)
void StudentWorld::escape(list<Actor*>::iterator& escapee)
{
	increaseScore(500);
	m_alive--;
	(*escapee)->setDead();
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
		break;
	}
}

/*
void StudentWorld::createZombie(double x, double y)
{
	Actor* zombie = new Zombie(this, x, y);
	m_actors.push_back(zombie);
}
*/

//checks each actor to see if any actors died during the current tick, if so delete them and erase from the list of current actors
void StudentWorld::checkDead(list<Actor*>::iterator& currIt) {
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end()) {
		if ((*it)->isDead()) {
			delete (*it);
			(*it) = nullptr;
			list<Actor*>::iterator temp = m_actors.erase(it);
			if (it == currIt)
				currIt = temp;
			it = temp;
			continue;
		}
		it++;
	}
}