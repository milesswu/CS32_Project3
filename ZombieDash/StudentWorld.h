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

	bool	checkCollisions(int dir, double x, double y, Actor* curr) const;
	bool	checkCollisionWithPenelope(int dir, double x, double y) const;
	bool	checkOverlap(double x, double y, char type);
	bool	checkOverlapWithPenelope(double x, double y) const;
	void	infectPenelope();
	void	killPenelope();
	void	escape(std::list<Actor*>::iterator& escapee);

	bool	findNearestCitizen(double x, double y, Actor* origin);
	double	findNearestZombie(double x, double y);
	bool	targetPenelope(double x, double y, double currDist, Actor* origin);
	double	distanceToPenelope(double x, double y) const;
	void	setClosestDirection(double actorX, double actorY, Actor* origin);

	void	pickupGoodie(char goodie);
	void	shootFlamethrower(int dir);
	void	explode(double x, double y);
	bool	createFlame(double x, double y, int dir);
	bool	createVomit(double x, double y, int dir);
	void	createZombie(double x, double y, int dir);
	bool	createVaccine(double x, double y);
	void	deployMine(double x, double y);

	void completeLevel()
	{
		m_completeLevel = true;
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
	const double		COLLISION_DISTANCE = pow(SPRITE_WIDTH, 2) + pow(SPRITE_HEIGHT, 2);
	const double		OVERLAP_DISTANCE = 100;
	const double		TARGET_RANGE = 6400;
	Penelope*			m_penelope;
	std::list<Actor*>	m_actors;
	int					m_alive;
	bool				m_victory;
	bool				m_completeLevel;

	void checkDead(std::list<Actor*>::iterator& currIt);
};

#endif // STUDENTWORLD_H_
