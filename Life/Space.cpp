#include "Space.h"
#include "Creature.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace
{
	unsigned short rule (unsigned short state, unsigned short neighbours[8])
	{
		unsigned sum(0);
		for(unsigned int i = 0; i < 8; ++i)
		{
			sum +=neighbours[i];
		}
		if (state)
		{
			if (sum == 2 || sum == 3)
			{
				return state;
			}
		}
		else if (sum == 3)
		{
			return 1;
		}
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CSpace::CSpace() : m_width(1),
				   m_height(1),
				   m_scale(1),
				   m_rect(1, 1)
{
	initCont(m_width, m_height);
	m_pivot.setX(0);
	m_pivot.setY(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CSpace::CSpace(QPoint i_size)
	: m_rect(1, 1),
	  m_scale(1),
	  m_brushPoint(Qt::white, Qt::SolidPattern),
	  m_brushSpace(Qt::black, Qt::SolidPattern),
	  m_penBorder(Qt::red)
{
	m_width = i_size.x() > 0 ? i_size.x() : 1;
	m_height = i_size.y() > 0 ? i_size.y() : 1;

	initCont(m_width, m_height);	
	m_pivot.setX(0);
	m_pivot.setY(0);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
CSpace::CSpace(const std::list<QPoint>& i_points) : m_scale(1),
													m_rect(1, 1),
													m_brushPoint(Qt::white, Qt::SolidPattern),
													m_brushSpace(Qt::black, Qt::SolidPattern),
													m_penBorder(Qt::red)
{
	int maxTop(0), maxBottom(0);
	int maxLeft(0), maxRight(0);
	for (std::list<QPoint>::const_iterator i = i_points.begin(); i != i_points.end(); ++i)
	{
		if (i->x() < maxLeft) maxLeft = i->x();
		if (i->x() > maxRight) maxRight = i->x();
		if (i->y() < maxBottom)	maxBottom = i->y();
		if (i->y() > maxTop) maxTop = i->y();
	}
	m_width = maxRight - maxLeft + 3;
	m_height = maxTop - maxBottom + 3;

	// Calc pivotX
	if (maxLeft < 0 && maxRight > 0)
	{
		m_pivot.setX(-maxLeft + 1);
	}
	else
	{
		if (maxRight > 0)
		{
			m_pivot.setX((maxRight - maxLeft)/2 + 1);
		}
		else
		{
			m_pivot.setX(abs(maxLeft - maxRight)/2 + 1);
		}
	}
	//Calc pivotY
	if (maxBottom < 0 && maxTop > 0)
	{
		m_pivot.setY(maxTop + 1);
	}
	else
	{
		if (maxTop > 0)
		{
			m_pivot.setY((maxTop - maxBottom)/2 + 1);
		}
		else
		{
			m_pivot.setY(abs(maxBottom - maxTop)/2 + 1);
		}
	}
	initCont(m_width, m_height);
	//Put init values into Space Container
	for (std::list<QPoint>::const_iterator i = i_points.begin(); i != i_points.end(); ++i)
	{
		m_map[m_pivot.x()+i->x()][m_pivot.y() - i->y()]->SetAlive(true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CSpace::~CSpace(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::Draw(QPainter* i_painter)
{
	i_painter->setBrush(m_brushSpace);
	i_painter->drawRect(0, 0, m_rect.x(), m_rect.y());
	if (m_scale < 2)
	{
		QPen pen;
		for (unsigned int i = 0; i != m_width; ++i)
		{
			for (unsigned int j = 0; j != m_height; ++j)
			{
				if (m_map[i][j]->IsAlive())
				{
					setCellColor(m_map[i][j]->GetLives());
					pen.setColor(m_brushPoint.color());
					i_painter->setPen(pen);
					i_painter->drawPoint(i,j);
				}
			}
		}
	}
	else
	{
		// Draw points.
		for (unsigned int i = 0; i != m_width; ++i)
		{
			for (unsigned int j = 0; j != m_height; ++j)
			{
				if (m_map[i][j]->IsAlive())
				{
					setCellColor(m_map[i][j]->GetLives());
					i_painter->setBrush(m_brushPoint);
					i_painter->drawRect(i * m_scale, j * m_scale, m_scale, m_scale);
				}
			}
		}
		// Draw grid.
		i_painter->setPen(m_penBorder);
		for (unsigned int i = 0; i != m_width; ++i)
		{
			i_painter->drawLine(QPoint(i * m_scale, 0), QPoint(i * m_scale, m_height * m_scale));
		}
		for (unsigned int j = 0; j != m_height; ++j)
		{
			i_painter->drawLine(QPoint(0, j * m_scale), QPoint(m_width * m_scale, j * m_scale));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::Update()
{
	unsigned short leftUp(0), leftCt(0), leftDn(0), DownCt(0);
	unsigned short rightUp(0), rightCt(0), rightDn(0), UpCt(0);

	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			m_map[i][j]->Update();
		}
	}
	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			m_map[i][j]->PostUpdateSwap();
		}
	}
	resizeMap();	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::SetRect(QPoint i_rect)
{
	if (i_rect.x() > 0 && i_rect.y() > 0)
	{
		m_rect = i_rect;
		m_scale = std::min<int>(i_rect.x()/m_width, i_rect.y()/m_height);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::SetPoint(QPoint i_point)
{
	if ((i_point.x() >= 0 && i_point.x() <= m_width - 1)
		&& (i_point.y() >= 0 && i_point.y() <= m_height - 1))
	{
		if (!m_map[i_point.x()][i_point.y()])
		{
			m_map[i_point.x()][i_point.y()].reset(new Creature(*this, i_point));
		}
		m_map[i_point.x()][i_point.y()]->SetAlive(!m_map[i_point.x()][i_point.y()]->IsAlive());
	}
	if (i_point.x() == 0 || i_point.x() == m_width - 1 || i_point.y() == 0 || i_point.y() == m_height - 1)
	{
		resizeMap();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CSpace::GetScale() const
{
	return m_scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CreaturePtr CSpace::GetCreaturesNeighbour (const QPoint& i_creaturePoint, unsigned int i_whatNeighbour) const
{
	CreaturePtr cReturn;
	unsigned int i(i_creaturePoint.x());
	unsigned int j(i_creaturePoint.y());
	if ((i < 0 || i >= m_width)	|| (j < 0 || j >= m_height))
	{
		return cReturn;
	}
	// Correct neighbours
	switch (i_whatNeighbour)
	{
		/* Left Down */ case 0: cReturn = (i > 0 && j < m_height - 1) ? m_map[i-1][j+1] : cReturn; break;
	/* Left Cntr */ case 1: cReturn = (i > 0) ? m_map[i-1][j] : cReturn; break;
	/* Left Up   */ case 2: cReturn = (i > 0 && j > 0) ? m_map[i-1][j-1] : cReturn; break;
	/* Up Center */ case 3: cReturn = (j > 0) ? m_map[i][j-1] : cReturn; break;
	/* Right Up  */ case 4: cReturn = (i < m_width - 1 && j > 0) ? m_map[i+1][j-1] : cReturn; break;
	/* Right Cnt */ case 5: cReturn = (i < m_width - 1) ? m_map[i+1][j] : cReturn; break;
	/* Right Dwn */ case 6: cReturn = (i < m_width - 1 && j < m_height - 1) ? m_map[i+1][j+1] : cReturn; break;
	/* Down Cntr */ case 7: cReturn = (j < m_height - 1) ? m_map[i][j+1] : cReturn; break;
	}
	return cReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::resizeMap()
{
	bool upRow(false), downRow(false);
	bool leftCol(false), rightCol(false);

	//Should add row upwards/downwards
	for (unsigned int i = 0; i < m_width; ++i)
	{
		if (m_map[i][0]->IsAlive())
		{
			upRow = true;
		}
		if (m_map[i][m_height - 1]->IsAlive())
		{
			downRow = true;
		}
	}

	for (unsigned int j = 0; j < m_height; ++j)
	{
		if (m_map[0][j]->IsAlive())
		{
			leftCol = true;
		}
		if (m_map[m_width - 1][j]->IsAlive())
		{
			rightCol = true;
		}
	}

	if (!upRow && !downRow && !leftCol && !rightCol) return;

	unsigned int deltaRow(0);
	unsigned int deltaCol(0);
	if (leftCol) deltaCol = 1;
	if (upRow) deltaRow = 1;

	unsigned int newHeight = downRow ? m_height + deltaRow + 1 : m_height + deltaRow;
	unsigned int newWidth = rightCol ? m_width + deltaCol + 1 : m_width + deltaCol;
	SpaceCont oldMap(m_map);
	initCont(newWidth, newHeight);
	for (unsigned int i = 0; i != m_width; ++i)
	{
		for (unsigned int j = 0; j != m_height; ++j)
		{
			m_map[i + deltaCol][j + deltaRow] = oldMap[i][j];
		}
	}
	m_width = newWidth;
	m_height = newHeight;
	m_scale = std::min<int>(m_rect.x()/m_width, m_rect.y()/m_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::initCont(unsigned int i_width, unsigned int i_height)
{
	m_map.clear();
	SpaceCont map;
	for (unsigned int i = 0; i != i_width; ++i)
	{
		SpaceRow row;
		for (unsigned int j = 0; j != i_height; ++j)
		{
			CreaturePtr newCreature (new Creature(*this, QPoint(i,j)));
			row.push_back(newCreature);
		}
		map.push_back(row);
	}
	m_map.assign(map.begin(), map.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CSpace::setCellColor(unsigned int i_nlives)
{
	if (i_nlives >= COUNT_COLORS)
	{
		i_nlives = COUNT_COLORS -1;
	}
	m_brushPoint.setColor(sc_cellColors[i_nlives]);
}