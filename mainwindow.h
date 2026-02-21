#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "vk-api.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void tickTimerSlot();

    void on_webEngineView_urlChanged(const QUrl &arg1);

private:
    Ui::MainWindow *ui;

    VkApi* vkApi;

    QTimer tickTimer;

    int wallPosition = 0;

    void fillTableForFrontEnd();
};
#endif // MAINWINDOW_H
