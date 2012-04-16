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
	SpaceRow row(m_width, 0);
	SpaceCont map(m_height, row);
	m_map.assign(map.begin(), map.end());
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
	SpaceRow row(m_width, 0);
	SpaceCont map(m_height, row);
	m_map.assign(map.begin(), map.end());
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
	SpaceRow row(m_height, 0);
	SpaceCont map(m_width, row);
	m_map.assign(map.begin(), map.end());
	//Put init values into Space Container
	for (std::list<QPoint>::const_iterator i = i_points.begin(); i != i_points.end(); ++i)
	{
		m_map[m_pivot.x()+i->x()][m_pivot.y() - i->y()] = 1;
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
		QPen pen(m_brushPoint.color());
		for (unsigned int i = 0; i != m_width; ++i)
		{
			for (unsigned int j = 0; j != m_height; ++j)
			{
				if (m_map[i][j] == 1)
				{					
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
				if (m_map[i][j] == 1)
				{
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
	SpaceCont tempMap(m_map);
	unsigned short leftUp(0), leftCt(0), leftDn(0), DownCt(0);
	unsigned short rightUp(0), rightCt(0), rightDn(0), UpCt(0);

	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			// Correct neighbours
			leftDn = (i > 0 && j < m_height - 1) ? tempMap[i-1][j+1] : 0;
			leftCt = (i > 0) ? tempMap[i-1][j] : 0;
			leftUp = (i > 0 && j > 0) ? tempMap[i-1][j-1] : 0;
			UpCt = (j > 0) ? tempMap[i][j-1] : 0;
			rightUp = (i < m_width - 1 && j > 0) ? tempMap[i+1][j-1] : 0;
			rightCt = (i < m_width - 1) ? tempMap[i+1][j] : 0;
			rightDn = (i < m_width - 1 && j < m_height - 1) ? tempMap[i+1][j+1] : 0;
			DownCt = (j < m_height - 1) ? tempMap[i][j+1] : 0;
			unsigned short neighbours[8] = {leftDn, leftCt, leftUp, UpCt, rightUp, rightCt, rightDn, DownCt};

			m_map[i][j] = rule(tempMap[i][j], neighbours);
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
		m_map[i_point.x()][i_point.y()] = (m_map[i_point.x()][i_point.y()] == 0) ? 1 : 0;
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

void CSpace::resizeMap()
{
	bool upRow(false), downRow(false);
	bool leftCol(false), rightCol(false);

	//Should add row upwards/downwards
	for (unsigned int i = 0; i < m_width - 1; ++i)
	{
		if (m_map[i][0] == 1)
		{
			upRow = true;
		}
		if (m_map[i][m_height - 1] == 1)
		{
			downRow = true;
		}
	}

	for (unsigned int j = 1; j < m_height - 1; ++j)
	{
		if (m_map[0][j] == 1)
		{
			leftCol = true;
		}
		if (m_map[m_width - 1][j] == 1)
		{
			rightCol = true;
		}
	}

	if (!upRow && !downRow && !leftCol && !rightCol) return;

	unsigned int deltaRow(0);
	unsigned int deltaCol(0);
	if (leftCol) deltaCol = 1;
	if (upRow) deltaRow = 1;

	SpaceRow newRow(downRow ? m_height + deltaRow + 1 : m_height + deltaRow, 0);
	SpaceCont newMap(rightCol ? m_width + deltaCol + 1 : m_width + deltaCol, newRow);
	for (unsigned int i = 0; i != m_width; ++i)
	{
		for (unsigned int j = 0; j != m_height; ++j)
		{
			newMap[i + deltaCol][j + deltaRow] = m_map[i][j];
		}
	}
	m_height = newRow.size();
	m_width = newMap.size();
	m_scale = std::min<int>(m_rect.x()/m_width, m_rect.y()/m_height);

	m_map.clear();
	m_map.assign(newMap.begin(), newMap.end());
}