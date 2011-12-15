#pragma once

#include "ant.h"

class GuardAnt :
	public Ant
{
public:
	GuardAnt(void);
	~GuardAnt(void);
	void chooseMove(State & state);
};

