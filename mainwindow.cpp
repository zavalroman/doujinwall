#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "vk-objects.h"
#include "doujin-database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DoujinDatabase::init();

    vkApi = new VkApi;
    vkApi->setToken("vk1.a.Pnez3w6OFGDSFqF3yiaGdAj3X_ODTFRkj_XDTB_KbeyM0xZKd44gfW8KKhd7KpObD_qIYw7JqdoXNFC7WjNg8bYBSr1u6MbGSIGzYGUjWJ_ZyXylF4larx2pp5mR674xCpE8LnOS0bXZ2tpr0vZ5eiOhqPo1nDKPJ_Tm-3grLt2a5_fMInQFTUPRLVr8EmZh2kKO-6652wRepPKve7r0eQ");

    ui->webEngineView->load(QUrl("https://oauth.vk.com/authorize?client_id=5534287&scope=audio,docs&redirect_uri=https://oauth.vk.com/blank.html&display=popup&v=5.52&response_type=token"));


    connect(&tickTimer, &QTimer::timeout, this, &MainWindow::tickTimerSlot);
    tickTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tickTimerSlot()
{
    vkApi->wallGet(wallPosition, 100, [this](const QList<WallPost>& wall, bool success) {
        if (success) {
            qDebug() << "INSERT ID FROM" << wall.first().id << "TO" << wall.last().id;
            DoujinDatabase::insertPosts(wall);
        } else {
            qDebug() << "TICK TIMER STOP";
            tickTimer.stop();
        }
    });
    wallPosition += 100;
}

void MainWindow::on_webEngineView_urlChanged(const QUrl &arg1)
{
#define ACCESS_TOKEN_STRING "access_token="
    qDebug() << "URL CHANGED" << arg1;
    QStringList sl = arg1.toString().split(ACCESS_TOKEN_STRING);
    if (sl.size() > 1) {
        QString token = sl.last().split("&").first();
        qDebug() << "NEW TOKEN" << token;
        vkApi->setToken(token);
    }
}

void MainWindow::fillTableForFrontEnd()
{

}

