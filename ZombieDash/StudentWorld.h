#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
#include <cmath>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	//functions called with every class to test for overlap and collision
	bool	checkCollisions(double x, double y, Actor* curr) const;
	bool	checkOverlap(double x, double y, char type);
	bool	checkCollisionWithPenelope(double x, double y) const;
	bool	checkOverlapWithPenelope(double x, double y) const;

	//functions to handle specific behaviors with penelope and citizens
	void	infectPenelope();
	void	killPenelope();
	void	escape(std::list<Actor*>::iterator& escapee);
	
	//functions used for 'AI,' targeting nearest actors (used for citizen and smart zombies)
	bool	findNearestInfectable(double x, double y, Actor* origin);
	double	findNearestZombie(double x, double y);
	bool	targetPenelope(double x, double y, double currDist, Actor* origin);
	void	setClosestDirection(double actorX, double actorY, Actor* origin);

	//functions used by penelope specifically
	void	pickupGoodie(char goodie);
	void	shootFlamethrower(int dir);

	//functions that handle manipulation of student world's objects
	bool	createFlame(double x, double y, int dir);
	bool	createVomit(double x, double y, int dir);
	void	createZombie(double x, double y, int dir);
	bool	createVaccine(double x, double y);
	void	deployMine(double x, double y);
	void	explode(double x, double y);

	double	distanceToPenelope(double x, double y) const;


	void completeLevel()
	{
		m_completeLevel = true;
		playSound(SOUND_LEVEL_FINISHED);
	}

	int leftAlive()
	{
		return m_alive;
	}

	void decAlive()
	{
		m_alive--;
	}

private:
	const double		OVERLAP_DISTANCE = 100;
	const double		TARGET_RANGE = 6400;
	Penelope*			m_penelope;
	std::list<Actor*>	m_actors;
	int					m_alive;
	bool				m_completeLevel;

	void checkDead(std::list<Actor*>::iterator& currIt);
};

#endif // STUDENTWORLD_H_
