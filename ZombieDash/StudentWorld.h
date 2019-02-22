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

	bool	checkForCollisions(int dir, double x, double y) const;
	//void	checkOverlap(double x, double y);
	//bool	checkOverlapWithPenelope(double x, double y) const;
	//bool	isSameActor(Actor* actor, Actor* other);
	void	killActor(std::list<Actor*>::iterator& kill);
	void	escape(std::list<Actor*>::iterator& escapee);
	void	overlapWithExit(double x, double y);
	void	overlapWithHazard(double x, double y);
	void	overlapWithGoodie(double x, double y);
	//void	createZombie(double x, double y);

private:
	const int			COLLISION_DISTANCE = sqrt(pow(SPRITE_WIDTH, 2) + pow(SPRITE_HEIGHT, 2));
	const int			OVERLAP_DISTANCE = 100;
	Penelope*			m_penelope;
	std::list<Actor*>	m_actors;
	int					m_alive;
	bool				m_victory;

	void checkDead();
};

#endif // STUDENTWORLD_H_
