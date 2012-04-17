#include "Space.h"
#include "boost\weak_ptr.hpp"

class Creature
{
public:
	Creature(const CSpace& i_space, const QPoint& i_position);
	~Creature();

	void SetAlive(bool i_alive);
	bool IsAlive() const;
	void IncreaseLives(int i_delta);
	unsigned int GetLives() const;
	
	void Update();
	void PostUpdateSwap();

protected:
	void updateNeighbours();

private:
	const CSpace& m_space;
	QPoint m_position;
	boost::weak_ptr<Creature> m_neighbours[COUNT_NEIGHBOURS];
	bool m_alive;
	bool m_futureState;
	unsigned int m_nlives;
};
