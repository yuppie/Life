#ifndef CGAME_H
#define CGAME_H

#include <QtGui/QDialog>
#include "ui_cgame.h"
#include "Space.h"

class CGame : public QDialog
{
	Q_OBJECT

public:
	CGame(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CGame();

protected:
	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void drawGrid(QPainter* i_painter, QPoint i_rect, unsigned int scale);

private:
	CSpace* m_space;
	Ui::CGameClass ui;
};

#endif // CGAME_H
