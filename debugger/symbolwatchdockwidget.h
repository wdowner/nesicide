#ifndef SYMBOLWATCHDOCKWIDGET_H
#define SYMBOLWATCHDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
    class SymbolWatchDockWidget;
}

class SymbolWatchDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SymbolWatchDockWidget(QWidget *parent = 0);
    ~SymbolWatchDockWidget();

private:
    Ui::SymbolWatchDockWidget *ui;
};

#endif // SYMBOLWATCHDOCKWIDGET_H