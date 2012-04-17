#pragma once
#include "Creature.h"

Creature::Creature(const CSpace& i_space, const QPoint& i_position) :
	m_space(i_space),
	m_position(i_position),
	m_alive(false),
	m_futureState(false),
	m_nlives(0)
{
	// Init array!
	for (unsigned int i = 0; i < 8; ++i)
	{
		m_neighbours[i].reset();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Creature::~Creature(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Creature::SetAlive(bool i_alive)
{
	m_alive = i_alive;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Creature::IsAlive() const
{
	return m_alive;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Creature::IncreaseLives(int i_delta)
{
	m_nlives = (m_nlives + i_delta >= 0) ? m_nlives + i_delta : 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Creature::GetLives() const
{
	return m_nlives;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Creature::Update()
{
	updateNeighbours();
	unsigned sum(0);
	for(unsigned int i = 0; i < 8; ++i)
	{
		CreaturePtr neighbour(m_neighbours[i].lock());
		if (neighbour.get() && neighbour->IsAlive())
		{
			sum++;
		}
	}
	m_futureState = m_alive;
	if (m_alive)
	{
		if (sum != 2 && sum != 3)
		{
			m_futureState = false;
		}
	}
	else
	{
		if (sum == 3)
		{
			m_futureState = true;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Creature::PostUpdateSwap()
{
	if (m_alive && m_alive == m_futureState)
	{
		m_nlives++;
	}
	else
	{
		m_nlives = 0;
	}
	m_alive = m_futureState;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Creature::updateNeighbours()
{
	for (unsigned int i = 0; i < 8; ++i)
	{
		CreaturePtr neighbour(m_neighbours[i].lock());
		if (!neighbour.get())
		{
			m_neighbours[i] = m_space.GetCreaturesNeighbour(m_position, i);
		}
	}
}