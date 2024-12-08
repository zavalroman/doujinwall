#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "vk-objects.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vkApi = new VkApi;
    vkApi->setToken("vk1.a.2oesWxQnjamPePjGns0ONhig1-lYG9ROxvLLL06pP2mov1WsGBhddgmEFL4vjF1XdMAyC4jnciKUSWnw_taEQb7kzatfH1S8gejaKIEy_TqdOOfJNRxm_xcqO7ayRGp56FMbr9tgm3e_ZdA8uGAfUmEekA4Ahd0Zi93kTTFGZC6C03gKDN2WSv67kt6afmBURfMALLsmhwmKu5vlnd75IA");

    vkApi->wallGet(0, 10, [this](const QList<WallPost>& wall) {
        for (auto& p : wall) {
            qDebug() << "POST" << p.text;
            for (auto& i : p.images) {
                qDebug() << "IMAGE ID" << i.id;
                for (auto& s : i.sizes) {
                    qDebug() << "SIZE" << s.width << s.height << s.url;
                }
            }
            for(auto& f : p.files) {
                qDebug() << "FILE ID" << f.id << f.url;
            }
            qDebug() << "";
        }
    });

    //ui->webEngineView->load(QUrl("https://oauth.vk.com/authorize?client_id=5534287&scope=audio,docs&redirect_uri=https://oauth.vk.com/blank.html&display=popup&v=5.52&response_type=token"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_webEngineView_urlChanged(const QUrl &arg1)
{
    qDebug() << "URL CHANGED" << arg1;
}

