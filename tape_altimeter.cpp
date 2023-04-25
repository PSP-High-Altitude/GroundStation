
#include "tape_altimeter.h"
#include <QVBoxLayout>
#include <QQmlContext>
#include <QRect>
#include <QLabel>
#include <QMetaType>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <math.h>
#include "keys.h"

struct AltimeterData
{
    Q_GADGET
    Q_PROPERTY(QRect rect MEMBER rect)
    Q_PROPERTY(QString text MEMBER text)

public:
    QRect rect;
    QString text;
    AltimeterData(const QRect& rect = QRect(), const QString& text = "") {
        this->rect = rect;
        this->text = text;
    }
    AltimeterData(const AltimeterData& copy) {
        this->rect = copy.rect;
        this->text = copy.text;
    }
    ~AltimeterData() {}
};

Q_DECLARE_METATYPE(AltimeterData)

TapeAltimeter::TapeAltimeter(QWidget* window, QQmlApplicationEngine *engine, QNetworkAccessManager *net_mgr)
{
    for (int i = 0; i < ALT_HEIGHT/ALT_TICK_SPACE; i++) {
        tick_list << QVariant::fromValue( AltimeterData{ QRect{0, i*ALT_TICK_SPACE, 120, 2}, "text1"} );
    }

    this->engine = engine;
    this->net_mgr = net_mgr;

    alt_lat = 0;
    alt_lon = 0;

    engine->rootContext()->setContextProperty("altitude", 190);
    engine->rootContext()->setContextProperty("ground_alt", 190);
    engine->rootContext()->setContextProperty("timer_period", ANIMATION_TIMER_PERIOD);
    engine->rootContext()->setContextProperty("tick_spacing", ALT_TICK_SPACE);
    engine->rootContext()->setContextProperty("altimeter_height", ALT_HEIGHT);
    engine->rootContext()->setContextProperty("tick_list", tick_list);
    engine->load(QUrl(QStringLiteral("qrc:/tape_altimeter.qml")));

    QQuickWindow *qmlWindow = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    this->view = qmlWindow;
    qmlWindow->hide();
    QWidget *placeholder = window->findChild<QWidget*>("widget_altitude");
    QWidget *container = QWidget::createWindowContainer(qmlWindow, window);
    container->setMaximumSize(placeholder->maximumSize());
    container->setMinimumSize(placeholder->minimumSize());
    QVBoxLayout *layout = window->findChild<QVBoxLayout*>("layout_altitude");
    layout->replaceWidget(placeholder, container);
    container->setObjectName("widget_altitude");

    QObject::connect(net_mgr, &QNetworkAccessManager::finished,
                     this, [=](QNetworkReply *reply)
    {
        if (reply->error()) {
            return;
        }

        QString resp = reply->readAll();
        QJsonObject resp_json = QJsonDocument::fromJson(resp.toUtf8()).object();
        QJsonArray array = resp_json["results"].toArray();
        float elevation = array.at(0)["elevation"].toDouble();
        engine->rootContext()->setContextProperty("ground_alt", (int) elevation);
    });
}

TapeAltimeter::~TapeAltimeter() {}

void TapeAltimeter::update_ticks(GpsData *gps)
{
    if(gps->fix_valid)
    {
        engine->rootContext()->setContextProperty("altitude", (int) gps->height_msl);
        if(abs(gps->lat - alt_lat) > 0.1 || abs(gps->lon - alt_lon) > 0.1)
        {
            update_ground_level(gps->lat, gps->lon);
            alt_lat = gps->lat;
            alt_lon = gps->lon;
        }
    }
}

void TapeAltimeter::update_ground_level(float lat, float lon)
{
    char url[256];
    sprintf_s(url, "https://maps.googleapis.com/maps/api/elevation/json?locations=%.7f%%2C%.7f&key=%s", lat, lon, GOOGLE_API_KEY);
    request.setUrl(QUrl(url));
    net_mgr->get(request);
}

#include "tape_altimeter.moc"
