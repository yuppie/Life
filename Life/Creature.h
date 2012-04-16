#include "Space.h"

class Creature
{
public:
	Creature(const CSpace& i_space, const QPoint& i_position);
	~Creature();

private:
	const CSpace& m_space;
	QPoint m_position;
	Creature* m_neighbours[COUNT_NEIGHBOURS];
	bool m_alive;
	unsigned int m_nlives;
};
