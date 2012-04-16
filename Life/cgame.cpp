#include "cgame.h"
#include <QPainter>
#include <QKeyEvent>
#include <QEvent>
#include <list>

//////////////////////////////////////////////////////////////////////////////////////////////////
CGame::CGame(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	/*QPoint p1(1, 0);
	QPoint p2(1, 1);
	QPoint p3(1, 2);
	QPoint p4(3, 1);
	QPoint p5(3, 2);
	QPoint p6(3, 3);
	QPoint p7(4, 2);
	QPoint p8(-1, -1);
	QPoint p9(-1, -2);
	QPoint p10(-3, -2);
	std::list<QPoint> list;
	list.push_back(p1);
	list.push_back(p2);
	list.push_back(p3);
	list.push_back(p4);
	list.push_back(p5);
	list.push_back(p6);
	list.push_back(p7);
	list.push_back(p8);
	list.push_back(p9);
	list.push_back(p10);*/
	m_space = new CSpace(QPoint(10,10));

	QPoint rect(700, 700);
	m_space->SetRect(rect);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	m_space->Draw(&painter); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
	case Qt::Key_Space:
		m_space->Update();
		repaint();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		unsigned int scale = m_space->GetScale();
		QPoint point(event->pos());
		point.setX(point.x()/scale);
		point.setY(point.y()/scale);
		m_space->SetPoint(point);
		repaint();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::drawGrid(QPainter* i_painter, QPoint i_rect, unsigned int scale)
{
}

