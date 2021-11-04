
#include <QRandomGenerator>
#include <random>
#include "device.h"

int getRand(int min, int max){
    std::random_device seeder;
    std::mt19937 rng(seeder());
    std::uniform_int_distribution<int> gen(min, max); // uniform, unbiased

    return gen(rng);
}

Device::Device(const QString &name, Device::Type type)
    : m_name(name)
    , m_type(type)
{
    m_state = Device::Nominal;
    m_strength = -1;

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Device::slotSimulation);
    m_timer->setSingleShot(true);
    m_timer->start(getRand(1000, 5000));

}

void Device::slotSimulation()
{
    m_timer->start(getRand(1000, 5000));

    int state = getRand(0, 2);

    if (state == 0)
    {
        m_state = State::Nominal;
    }
    else if (state == 1)
    {
        m_state = State::Warning;
    }
    else
    {
        m_state = State::Error;
    }

    m_strength = getRand(0, 100);

    emit stateChanged();
}
