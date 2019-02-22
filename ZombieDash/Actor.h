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

	virtual void doSomething() = 0;
	virtual bool hasCollision() = 0;

	virtual bool isInfectable()
	{
		return false;
	}

	virtual bool isDamageable()
	{
		return false;
	}

	bool isDead() const
	{
		return m_isDead;
	}

	void setDead()
	{
		m_isDead = true;
	}

	StudentWorld* getWorld() const
	{
		return m_world;
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

	virtual bool isDamageable()
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

	virtual bool isInfectable()
	{
		return true;
	}

	bool isInfected()
	{
		return m_isInfected;
	}

	bool infect()
	{
		if (isInfected()) {
			m_infectionCount++;
			if (m_infectionCount == INFECT_LEVEL) {
				setDead();
				return true;
			}
		}
		return false;
	}

	int infectionCount()
	{
		return m_infectionCount;
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

	int getVaccines() const
	{
		return m_vaccineCount;
	}

	int getGas() const
	{
		return m_gasCount;
	}

	int getLandmines() const
	{
		return m_landmineCount;
	}

	void incVaccines()
	{
		m_vaccineCount++;
	}

	void incGas()
	{
		m_gasCount++;
	}

	void incLandmine()
	{
		m_landmineCount++;
	}

private:
	int	m_vaccineCount;
	int	m_gasCount;
	int m_landmineCount;
};

/*
class Citizen : public InfectablePlayer
{
public:
	Citizen(StudentWorld* world, double startX, double startY) : InfectablePlayer(world, IID_CITIZEN, startX, startY) {}
	virtual ~Citizen();

	virtual void doSomething();

private:
	bool m_escaped;
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

class Exit : public Environment
{
public:
	Exit(StudentWorld* world, double startX, double startY) : Environment(world, IID_EXIT, startX, startY, right, 1) {}
	virtual ~Exit();

	virtual void doSomething();
	virtual bool hasCollision()
	{
		return false;
	}

};


class Hazard : public Environment
{
public:
	Hazard(StudentWorld* world, int imageID, double startX, double startY, Direction dir, int depth) : Environment(world, imageID, startX, startY, dir, depth) {}
	virtual ~Hazard();
	virtual bool hasCollision()
	{
		return false;
	}
};


class Pit : public Hazard
{
public:
	Pit(StudentWorld* world, double startX, double startY) : Hazard(world, IID_PIT ,startX, startY, right, 0) {}
	virtual ~Pit();

	virtual void doSomething();

};
/*
class Landmine : public Hazard
{
	Landmine(StudentWorld* world, double startX, double startY) : Hazard(world, IID_LANDMINE ,startX, startY, right, 1) {}
	virtual ~Landmine();

	virtual bool isDamageable() 
	{
		return true;
	}
	virtual void doSomething();

};
//*/

/**********************************************************************************************************************************************************
																		GOODIE OBJECTS
***********************************************************************************************************************************************************
*/

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 0, 1) {}
	virtual ~Goodie();
	virtual void doSomething();

	virtual bool hasCollision()
	{
		return false;
	}

	virtual bool isDamageable()
	{
		return true;
	}
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_VACCINE_GOODIE ,startX, startY) {}
	virtual ~VaccineGoodie();

	virtual void doSomething();
};


class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_GAS_CAN_GOODIE ,startX, startY) {}
	virtual ~GasCanGoodie();

	virtual void doSomething();

};

class LandmineGoodie : public Goodie
{
public:
	LandmineGoodie(StudentWorld* world, double startX, double startY) : Goodie(world, IID_LANDMINE_GOODIE ,startX, startY) {}
	virtual ~LandmineGoodie();

	virtual void doSomething();

};
/**********************************************************************************************************************************************************
																		PROJECTILE OBJECTS
***********************************************************************************************************************************************************
*/

class Projectile : public Actor
{
public:
	Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir) : Actor(imageID, startX, startY, dir, 0) 
	{
		m_lifespan = 2;
	}
	virtual ~Projectile();
	virtual void doSomething();
	
	void decLife()
	{
		m_lifespan--;
	}

private:
	int m_lifespan;
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

#endif // ACTOR_H_
