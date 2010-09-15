#include "statuswidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    label = new QLabel(tr("Initialising"));
    label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(label);
    progress = new QProgressBar();
    progress->setMinimum(0);
    progress->setMaximum(0);
    layout->addWidget(progress);
}
