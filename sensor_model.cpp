#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QQmlContext>

#include "device.h"
#include "sensor_model.h"



SensorModel::SensorModel(QObject *parent)
    : QObject(parent)
{
    m_devices << QSharedPointer<Device>(new Device("Front GPS", Device::GPS));
    m_devices << QSharedPointer<Device>(new Device("Rear GPS", Device::GPS));
    m_devices << QSharedPointer<Device>(new Device("Stackboard GPS", Device::GPS));
    m_devices << QSharedPointer<Device>(new Device("Compass #1", Device::Compass));
    m_devices << QSharedPointer<Device>(new Device("Compass #2", Device::Compass));
    m_devices << QSharedPointer<Device>(new Device("Compass #3", Device::Compass));
    m_devices << QSharedPointer<Device>(new Device("Compass #4", Device::Compass));
    m_devices << QSharedPointer<Device>(new Device("Primary anemometer", Device::Anemometer));
    m_devices << QSharedPointer<Device>(new Device("Secondary anemometer", Device::Anemometer));
    m_devices << QSharedPointer<Device>(new Device("Main Gyro", Device::Gyro));
    m_devices << QSharedPointer<Device>(new Device("Backup Gyro", Device::Gyro));
}
