#pragma once
#include <QPainter>
#include <QPoint>
#include <vector>
#include <list>

class Creature;

typedef std::vector<Creature*> SpaceRow;
typedef std::vector<SpaceRow> SpaceCont;
typedef SpaceRow::const_iterator SpaceRowIt;
typedef SpaceCont::const_iterator SpaceContIt;

#define COUNT_NEIGHBOURS 8
#define COUNT_COLORS 10
static const QColor sc_cellColors[COUNT_COLORS] = {Qt::white, Qt::lightGray, Qt::yellow, Qt::darkYellow,
												   Qt::magenta, Qt::darkMagenta, Qt::cyan, Qt::darkCyan, 
									               Qt::blue, Qt::darkBlue};

class CSpace
{
public:
	CSpace();
	explicit CSpace(QPoint i_size);
	explicit CSpace(const std::list<QPoint>& i_points);
	~CSpace(void);

	void Update();
	void Draw(QPainter* i_painter);

	void SetRect(QPoint i_rect);
	void SetPoint(QPoint i_point);

	unsigned int GetScale() const;
	Creature* GetCreaturesNeighbour (const QPoint& i_creaturePoint, unsigned int i_whatNeighbour) const;

protected:
	void resizeMap();
	void initCont(unsigned int i_width, unsigned int i_height);

	void setCellColor(unsigned int i_nlives);

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_scale;
	QPoint m_pivot;
	QPoint m_rect;
	SpaceCont m_map;


	// Visualization
	QBrush m_brushPoint;
	QBrush m_brushSpace;
	QPen m_penBorder;
};

