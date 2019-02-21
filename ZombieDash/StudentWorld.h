#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

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

	bool	checkForCollisions(int dir, double x, double y);
	bool	overlapWithExit(double x, double y);
	void	createZombie(double x, double y);

private:
	const int			COLLISION_DISTANCE = 100;
	Penelope*			m_penelope;
	std::list<Actor*>	m_actors;
	int					m_alive;
	bool				m_victory;

	void checkDead();
};

#endif // STUDENTWORLD_H_
