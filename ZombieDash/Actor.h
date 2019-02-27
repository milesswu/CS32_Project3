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
	virtual void kill() = 0;
	virtual void infectInfectable() = 0;

	virtual bool hasCollision()
	{
		return false;
	}

	virtual bool isInfectable()
	{
		return false;
	}

	virtual bool isDamageable()
	{
		return false;
	}

	virtual bool doesMove()
	{
		return false;
	}

	virtual bool blockFlames()
	{
		return false;
	}
	
	virtual bool blockVomit()
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
	Player(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right) : Actor(world, imageID, startX, startY, dir) {}
	virtual ~Player();
	virtual bool move(Direction dir, double x, double y);

	virtual bool hasCollision()
	{
		return true;
	}

	virtual bool isDamageable()
	{
		return true;
	}

	virtual bool doesMove()
	{
		return true;
	}

	virtual void infectInfectable()
	{
		return;
	}

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
	virtual void doSomething();
	virtual void infect();

	virtual void infectInfectable()
	{
		m_isInfected = true;
	}

	virtual bool isInfectable()
	{
		return true;
	}

	bool isInfected()
	{
		return m_isInfected;
	}

	int infectionCount()
	{
		return m_infectionCount;
	}

	void incInfection()
	{
		m_infectionCount++;
	}

	void cure()
	{
		m_infectionCount = 0;
		m_isInfected = false;
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
	virtual void doSomething();
	virtual bool move(Direction dir, double x, double y);
	virtual void kill();

	void shootFlamethrower(Direction dir);
	void deployMine();

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
		m_gasCount += 5;
	}

	void incLandmine()
	{
		m_landmineCount += 2;
	}

	void useVaccine()
	{
		if (m_vaccineCount > 0) {
			cure();
			m_vaccineCount--;
		}
	}



private:
	int	m_vaccineCount;
	int	m_gasCount;
	int m_landmineCount;
};

class Citizen : public InfectablePlayer
{
public:
	Citizen(StudentWorld* world, double startX, double startY) : InfectablePlayer(world, IID_CITIZEN, startX, startY) 
	{
		m_isParalyzed = false;		
	}
	virtual ~Citizen();
	virtual void kill();

	virtual void doSomething();

	bool isParalyzed()
	{
		return m_isParalyzed;
	}

	void changeState()
	{
		m_isParalyzed = !m_isParalyzed;
	}

private:
	bool m_isParalyzed;
};

//*/

class Zombie : public Player
{
public:
	Zombie(StudentWorld* world, double startX, double startY, Direction dir = right) : Player(world, IID_ZOMBIE, startX, startY, dir) 
	{
		m_movePlan = 0;
		m_isParalyzed = false;
		setScore(1000);
	}
	virtual ~Zombie();
	virtual void kill();
	virtual void doSomething();
	virtual void changeDirection();

	bool spitVomit();

	bool isParalyzed()
	{
		return m_isParalyzed;
	}

	void changeState()
	{
		m_isParalyzed = !m_isParalyzed;
	}

	void setScore(int score)
	{
		m_score = score;
	}

	int getScore()
	{
		return m_score;
	}

	int moves()
	{
		return m_movePlan;
	}

	void setMovePlan()
	{
		m_movePlan = randInt(3, 10);
	}

	virtual void infectInfectable()
	{
		return;
	}

private:
	int		m_movePlan;
	bool	m_isParalyzed;
	int		m_score;
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* world, double startX, double startY, Direction dir = right) : Zombie(world, startX, startY, dir) 
	{
		if (randInt(1, 10) == 1)
			m_hasVaccine = true;
		else
			m_hasVaccine = false;
	}
	virtual ~DumbZombie();
	virtual void kill();

private:
	bool m_hasVaccine;
};


class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* world, double startX, double startY, Direction dir = right) : Zombie(world, startX, startY, dir) 
	{
		setScore(2000);
	}
	virtual ~SmartZombie();

	virtual void doSomething();
	virtual void changeDirection();
};

/**********************************************************************************************************************************************************
																		ENVIRONMENT OBJECTS
***********************************************************************************************************************************************************
*/

class Environment : public Actor
{
public:
	Environment(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, int depth = 0) : Actor(world, imageID, startX, startY, dir, depth) {}
	virtual ~Environment();
	virtual void kill() { return; }

	virtual bool blockFlames()
	{
		return true;
	}

	virtual void infectInfectable()
	{
		return;
	}
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

	virtual bool blockVomit()
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
};


class Hazard : public Environment
{
public:
	Hazard(StudentWorld* world, int imageID, double startX, double startY, Direction dir, int depth) : Environment(world, imageID, startX, startY, dir, depth) {}
	virtual ~Hazard();

	virtual bool blockFlames()
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

class Landmine : public Hazard
{
public:
	Landmine(StudentWorld* world, double startX, double startY) : Hazard(world, IID_LANDMINE ,startX, startY, right, 1) 
	{
		m_isActive = false;
		m_safetyTicks = 30;
	}
	virtual ~Landmine();
	virtual void doSomething();
	virtual void kill();

	virtual bool isDamageable()
	{
		return true;
	}

	void decSafety()
	{
		m_safetyTicks--;
	}

	void activate()
	{
		m_isActive = true;
	}

	bool isActive()
	{
		return m_isActive;
	}

	int getSafety()
	{
		return m_safetyTicks;
	}

private:
	bool	m_isActive;
	int		m_safetyTicks;
};


/**********************************************************************************************************************************************************
																		GOODIE OBJECTS
***********************************************************************************************************************************************************
*/

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 0, 1) 
	{
		m_iTicks = 2;
	}
	virtual ~Goodie();
	virtual void doSomething();
	virtual void kill();

	virtual bool isDamageable()
	{
		return m_iTicks == 0;
	}

	int get_iTicks()
	{
		return m_iTicks;
	}

	void dec_iTicks()
	{
		m_iTicks--;
	}

	virtual void infectInfectable()
	{
		return;
	}

private:
	int m_iTicks;
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
	Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir) : Actor(world, imageID, startX, startY, dir, 0) 
	{
		m_lifespan = 2;
	}
	virtual ~Projectile();
	virtual void doSomething();

	virtual bool isDamgeable()
	{
		return false;
	}

	virtual void kill();
	int getLife()
	{
		return m_lifespan;
	}

	void decLife()
	{
		m_lifespan--;
	}

	virtual void infectInfectable()
	{
		return;
	}

private:
	int m_lifespan;
};

class Vomit : public Projectile	
{
public:
	Vomit(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(world, IID_VOMIT ,startX, startY, dir) {}
	virtual ~Vomit();

	virtual void doSomething();
};

class Flame : public Projectile
{
public:
	Flame(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(world, IID_FLAME, startX, startY, dir) {}
	virtual ~Flame();

	virtual void doSomething();

};

#endif // ACTOR_H_
