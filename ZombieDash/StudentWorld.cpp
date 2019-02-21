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
}

StudentWorld::~StudentWorld()
{
	cleanUp();
	cerr << "Destroying the StudentWorld object" << endl;
}

int StudentWorld::init()
{
	Level lev(assetPath());
	int levNum = getLevel();
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
				m_alive++;
			}
			case Level::exit:
			{
				break;
			}
			case Level::wall:
			{
				cerr << "Creating a wall" << endl;
				Wall* getWalled = new Wall(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
				m_actors.push_back(getWalled);
				break;
			}
			case Level::pit:
			{
				break;
			}
			case Level::vaccine_goodie:
			{
				break;
			}
			case Level::gas_can_goodie:
			{
				break;
			}
			case Level::landmine_goodie:
			{
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
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	if (!m_penelope->isDead()) {
		m_penelope->doSomething();
		if (m_penelope->isDead()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end()) {
		(*it)->doSomething();
		if (m_penelope->isDead()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		it++;
	}
	checkDead();
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
		del++;
	}
}

bool StudentWorld::checkForCollisions(int dir, double x, double y)
{
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		if (!((*it)->hasCollision())) {
			it++;
			continue;
		}
		double actorX = (*it)->getX();
		double actorY = (*it)->getY();	
		switch (dir) {
		case 0: //right
			if (x + SPRITE_WIDTH >= actorX && x + SPRITE_WIDTH <= actorX + SPRITE_WIDTH - 1 && y <= actorY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT >= actorY) {
				cerr << "Actor (x, y): (" << actorX << ", " << actorY << ")" << endl;
				cerr << "Actor max (x, y): (" << actorX + SPRITE_WIDTH << ", " << actorY + SPRITE_HEIGHT << ")" << endl;
				if (y + SPRITE_HEIGHT <= actorY || y >= actorY + SPRITE_HEIGHT)
					return false;
				return true;
			}
			break;
		case 90: //up
			if (y + SPRITE_HEIGHT >= actorY && y + SPRITE_HEIGHT <= actorY + SPRITE_HEIGHT - 1 && x <= actorX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH >= actorX) {
				cerr << "Actor (x, y): (" << actorX << ", " << actorY << ")" << endl;
				cerr << "Actor max (x, y): (" << actorX + SPRITE_WIDTH << ", " << actorY + SPRITE_HEIGHT << ")" << endl;
				if (x + SPRITE_WIDTH <= actorX || x >= actorX + SPRITE_WIDTH)
					return false;
				return true;
			}
			break;
		case 180: //left
			if (x <= actorX + SPRITE_WIDTH && x >= actorX && y <= actorY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT >= actorY) {
				cerr << "Actor (x, y): (" << actorX << ", " << actorY << ")" << endl;
				cerr << "Actor max (x, y): (" << actorX + SPRITE_WIDTH << ", " << actorY + SPRITE_HEIGHT << ")" << endl;
				if (y + SPRITE_HEIGHT <= actorY || y >= actorY + SPRITE_HEIGHT)
					return false;
				return true;
			}
			break;
		case 270: //down
			if (y <= actorY + SPRITE_HEIGHT && y >= actorY && x <= actorX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH >= actorX) {
				cerr << "Actor (x, y): (" << actorX << ", " << actorY << ")" << endl;
				cerr << "Actor max (x, y): (" << actorX + SPRITE_WIDTH << ", " << actorY + SPRITE_HEIGHT << ")" << endl;
				if (x + SPRITE_WIDTH <= actorX || x >= actorX + SPRITE_WIDTH)
					return false;
				return true;
			}
			break;
		default:
			break;
		}
		it++;
	}
	return false;
}

bool StudentWorld::checkForOverlap(double x, double y)
{
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		double hDist = ((*it)->getX() - x) * ((*it)->getX() - x);
		double vDist = ((*it)->getY() == y) * ((*it)->getY() == y);
		if ((hDist + vDist) <= COLLISION_DISTANCE)
			return true;
		it++;
	}
	return false;
}

/*
void StudentWorld::createZombie(double x, double y)
{
	Actor* zombie = new Zombie(this, x, y);
	m_actors.push_back(zombie);
}
*/

//checks each actor to see if any actors died during the current tick, if so delete them and erase from the list of current actors
void StudentWorld::checkDead() {
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end()) {
		if ((*it)->isDead()) {
			delete (*it);
			(*it) = nullptr;
			it = m_actors.erase(it);
			continue;
		}
		it++;
	}
}