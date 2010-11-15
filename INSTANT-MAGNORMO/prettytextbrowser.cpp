#include "prettytextbrowser.h"
#include <QPainter>

PrettyTextBrowser::PrettyTextBrowser(QWidget *parent)
	: QTextBrowser(parent)
{
}

PrettyTextBrowser::~PrettyTextBrowser()
{

}

void PrettyTextBrowser::setContactPhoto(QPixmap photo)
{
	contactPhoto = photo;
}

void PrettyTextBrowser::paintEvent(QPaintEvent *e)
{
	{
		QPainter painter(viewport());
		QBrush brush(contactPhoto);
		int w = contactPhoto.width(), h = contactPhoto.height();
		int x = width() - w - 4, y = 1;
		painter.setOpacity(0.5);
		painter.setBrush(brush);
		painter.translate(QPoint(x, y));
		painter.drawRoundedRect(0, 0, w, h, 3.0, 3.0);
	}
	QTextBrowser::paintEvent(e);
}