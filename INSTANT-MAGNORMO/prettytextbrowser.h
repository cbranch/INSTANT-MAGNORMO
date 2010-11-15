#ifndef PRETTYTEXTBROWSER_H
#define PRETTYTEXTBROWSER_H

#include <QTextBrowser>
#include <QPixmap>

class PrettyTextBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	PrettyTextBrowser(QWidget *parent);
	~PrettyTextBrowser();

public slots:
	void setContactPhoto(QPixmap pixmap);

protected:
	virtual void paintEvent(QPaintEvent *e);

private:
	QPixmap contactPhoto;
	
};

#endif // PRETTYTEXTBROWSER_H
