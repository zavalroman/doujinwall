#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_webEngineView_urlChanged(const QUrl &arg1);

private:
    Ui::MainWindow *ui;

    VkApi* vkApi;
};
#endif // MAINWINDOW_H
