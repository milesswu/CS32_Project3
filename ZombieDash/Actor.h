#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, int depth = 0) : GraphObject(imageID, startX, startY, dir, depth) 
	{
		m_world = world;
		m_isDead = false;
	}
	virtual ~Actor();

	virtual void	doSomething() = 0;
	virtual bool	hasCollision() = 0;

	StudentWorld* getWorld() const
	{
		return m_world;
	}

	bool isDead() const
	{
		return m_isDead;
	}

	void setDead()
	{
		m_isDead = true;
	}

private:
	StudentWorld*	m_world;
	bool			m_isDead;

};

/**********************************************************************************************************************************************************
																		MOVING PLAYER OBJECTS
***********************************************************************************************************************************************************
*/

class Player : public Actor
{
public:
	Player(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY) {}
	virtual ~Player();

	virtual bool hasCollision()
	{
		return true;
	}

	void	move(Direction dir, double x, double y);
private:

};

class InfectablePlayer : public Player
{
public:
	InfectablePlayer(StudentWorld* world, int imageID, double startX, double startY) : Player(world, imageID, startX, startY) 
	{
		m_isInfected = false;
		m_infectionCount = 0;
	}
	virtual ~InfectablePlayer();

	bool isInfected()
	{
		return m_isInfected;
	}

	bool getInfected()
	{
		return m_infectionCount == INFECT_LEVEL;
	}

	int incInfection()
	{
		m_infectionCount++;
		return m_infectionCount;
	}

	void cure()
	{
		m_infectionCount = 0;
	}

private:
	const int	INFECT_LEVEL = 500;
	bool		m_isInfected;
	int			m_infectionCount;

};

class Penelope : public InfectablePlayer
{
public:
	Penelope(StudentWorld* world, double startX, double startY) : InfectablePlayer(world, IID_PLAYER, startX, startY) 
	{
		m_vaccineCount = 0;
		m_gasCount = 0;
		m_landmineCount = 0;
	}
	virtual ~Penelope();

	virtual void	doSomething();

private:
	int	m_vaccineCount;
	int	m_gasCount;
	int m_landmineCount;
};

/*
class Citizen : public InfectablePlayer
{
	Citizen(StudentWorld* world, double startX, double startY) : InfectablePlayer(world, IID_CITIZEN, startX, startY) {}
	virtual ~Citizen();

	virtual void doSomething();

};

class Zombie : public Player
{
public:
	Zombie(StudentWorld* world, double startX, double startY) : Player(world, IID_ZOMBIE, startX, startY) {}
	virtual ~Zombie();

	virtual void doSomething();

private:
	int	movePlan;
};

class DumbZombie : public Zombie
{
	DumbZombie(StudentWorld* world, double startX, double startY) : Zombie(world, startX, startY) {}
	virtual ~DumbZombie();

	virtual void doSomething();

};

class SmartZombie : public Zombie
{
	SmartZombie(StudentWorld* world, double startX, double startY) : Zombie(world, startX, startY) {}
	virtual ~SmartZombie();

	virtual void doSomething();

};
//*/

/**********************************************************************************************************************************************************
																		ENVIRONMENT OBJECTS
***********************************************************************************************************************************************************
*/

class Environment : public Actor
{
public:
	Environment(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, int depth = 0) : Actor(world, imageID, startX, startY, dir, depth) {}
	virtual ~Environment();
};

class Wall : public Environment
{
public:
	Wall(StudentWorld* world, double startX, double startY) : Environment(world, IID_WALL, startX, startY) {}
	virtual ~Wall();

	virtual void doSomething() { return; }
	virtual bool hasCollision()
	{
		return true;
	}
};

/*
class Exit : public Environment
{
public:
	Exit(StudentWorld* world, double startX, double startY) : Environment(world, IID_EXIT, startX, startY, right, 0) {}
	virtual ~Exit();

	virtual void doSomething();

};

class Hazard : public Environment
{
public:
	Hazard(StudentWorld* world, int imageID, double startX, double startY) : Environment(world, imageID, startX, startY, right, 1) {}
	virtual ~Hazard();

	virtual void doSomething();

};

class Pit : public Hazard
{
public:
	Pit(StudentWorld* world, double startX, double startY) : Hazard(world, IID_PIT ,startX, startY) {}
	virtual ~Pit();

	virtual void doSomething();

};

class Landmine : public Hazard
{
	Landmine(StudentWorld* world, double startX, double startY) : Hazard(world, IID_LANDMINE ,startX, startY) {}
	virtual ~Landmine();

	virtual void doSomething();

};
//*/

/**********************************************************************************************************************************************************
																		GOODIE OBJECTS
***********************************************************************************************************************************************************
*/
/*
class Goodie : public Actor
{
public:
	Goodie(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 0, 1) {}
	virtual ~Goodie();

	virtual void doSomething();

};

class VaccineGoodie : public Goodie
{
	VaccineGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_VACCINE_GOODIE ,startX, startY) {}
	virtual ~VaccineGoodie();

	virtual void doSomething();

};

class GasCanGoodie : public Goodie
{
	GasCanGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_GAS_CAN_GOODIE ,startX, startY) {}
	virtual ~GasCanGoodie();

	virtual void doSomething();

};

class LandmineGoodie : public Goodie
{
	LandmineGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_LANDMINE_GOODIE ,startX, startY) {}
	virtual ~LandmineGoodie();

	virtual void doSomething();

};
//*/
/**********************************************************************************************************************************************************
																		PROJECTILE OBJECTS
***********************************************************************************************************************************************************
*/
/*
class Projectile : public Actor
{
public:
	Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir);
	virtual ~Projectile();

	virtual void doSomething() = 0;

};

class Vomit : public Projectile	
{
	Vomit(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(world, IID_VOMIT ,startX, startY, dir) {}
	virtual ~Vomit();

	virtual void doSomething();

};

class Flame : public Projectile
{
	Flame(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(world, IID_FLAME, startX, startY, dir) {}
	virtual ~Flame();

	virtual void doSomething();

};
//*/
#endif // ACTOR_H_
