#ifndef SENSOR_MODEL_H
#define SENSOR_MODEL_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringList>

#include "qobject_list_model.h"
#include "device.h"


class SensorModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(OLM_Device* devices READ devices CONSTANT)

public:
    SensorModel(QObject *parent = nullptr);

    // Modèle des devices
    OLM_Device* devices() { return &m_devices; }
    OLM_Device  m_devices;
};

#endif // SENSOR_MODEL_H
