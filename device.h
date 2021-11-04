#ifndef DEVICE_H
#define DEVICE_H

#include <QJsonDocument>
#include <QObject>
#include <QList>
#include <QQmlEngine>
#include <qobject_list_model.h>
#include <QTimer>

struct Device : QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT )
    Q_PROPERTY(Type type READ type CONSTANT )
    Q_PROPERTY(State state READ state NOTIFY stateChanged )
    Q_PROPERTY(int strength READ strength NOTIFY stateChanged )

public:
    enum Type {
        Anemometer,
        GPS,
        Compass,
        Gyro
    };
    Q_ENUM(Type)

    enum State {
        Nominal,
        Warning,
        Error
    };
    Q_ENUM(State)

    explicit Device(const QString &name, Type type);

    QString		name		() const { return  m_name; }
    Type		type		() const { return  m_type; }
    State		state		() const { return  m_state; }
    int		strength		() const { return  m_strength; }

signals:
    void stateChanged();

private:
    QTimer *m_timer;
    QString m_name;
    Type m_type;
    State m_state;
    int m_strength;

private slots:
    void slotSimulation();
};

DECLARE_Q_OBJECT_LIST_MODEL( Device )


#endif // DEVICE_H
