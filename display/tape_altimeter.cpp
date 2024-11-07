
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
#include "utils/misc.h"

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
    engine->load(QUrl(QStringLiteral("qrc:/display/tape_altimeter.qml")));

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

void TapeAltimeter::update_ticks(GpsData *gps, SensorData *sens)
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
    else
    {
        float baro_alt = 44330 * (1 - pow(sens->baro.pressure/1013.25, 1/5.255));
        if(baro_alt < 11000)
        {
            engine->rootContext()->setContextProperty("altitude", (int) baro_alt);
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

void TapeAltimeter::update_altimeter(pspcommsg msg)
{
    switch(msg.msg_id)
    {
    case 0x8A:
    case 0xE0:
    {
        // Lat and lon for ground alt, alt
        float lat = bytes_to_float(msg.payload+1);
        float lon = bytes_to_float(msg.payload+5);
        float alt = bytes_to_float(msg.payload+9);
        uint8_t fix_valid = msg.payload[31] & 0x1;
        GpsData gps;
        gps.lat = lat;
        gps.lon = lon;
        gps.height_msl = alt;
        gps.fix_valid = fix_valid;

        // Barometer for alternate source
        float pres = bytes_to_float(msg.payload + 26);
        SensorData sens;
        sens.baro.pressure = pres;

        update_ticks(&gps, &sens);

        break;
    }
    case 0xE1:
    {
        // Lat and lon for ground alt, alt
        float lat = bytes_to_float(msg.payload+1);
        float lon = bytes_to_float(msg.payload+5);
        float alt = bytes_to_float(msg.payload+9);
        uint8_t fix_valid = msg.payload[32] & 0x1;
        GpsData gps;
        gps.lat = lat;
        gps.lon = lon;
        gps.height_msl = alt;
        gps.fix_valid = fix_valid;

        // Barometer for alternate source
        float pres = bytes_to_float(msg.payload + 26);
        SensorData sens;
        sens.baro.pressure = pres;

        update_ticks(&gps, &sens);

        break;
    }
    default:
        break;
    }
}

#include "tape_altimeter.moc"
