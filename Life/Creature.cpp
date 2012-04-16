#pragma once
#include "Creature.h"

Creature::Creature(const CSpace& i_space, const QPoint& i_position) :
	m_space(i_space),
	m_position(i_position),
	m_alive(false),
	m_nlives(0)
{
	

}


Creature::~Creature(void)
{
}
