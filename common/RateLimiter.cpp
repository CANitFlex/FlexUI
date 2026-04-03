#include "RateLimiter.h"
#include <QDebug>

RateLimiter* RateLimiter::instance(QObject *parent) {
    static RateLimiter* inst = nullptr;
    if (!inst) {
        inst = new RateLimiter(parent);
    }
    return inst;
}

RateLimiter::RateLimiter(QObject *parent)
    : QObject(parent)
    , m_intervalMs(1000 / qBound(1, fps, 60))
{
    m_timer.setInterval(m_intervalMs);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &RateLimiter::flush);

    qDebug() << "RateLimiter created:" << fps << "FPS (" << m_intervalMs << "ms interval)";
}


void RateLimiter::start() {
    if (!m_timer.isActive()) {
        m_timer.start();
        qDebug() << "✅ RateLimiter started";
    }
}

void RateLimiter::stop() {
    m_timer.stop();
    QMutexLocker locker(&m_mutex);
    m_pendingData.clear();
    qDebug() << "⏹️ RateLimiter stopped";
}

void RateLimiter::queue(Enums::Services service, const QVariant& value) {
    QMutexLocker locker(&m_mutex);
    m_pendingData[service] = value;

    qDebug() << "🔵 Queued:" << static_cast<int>(service) << "=" << value;

    if (!m_timer.isActive()) {
        m_timer.start();
        qDebug() << "🔵 Auto-started timer";
    }
}

void RateLimiter::flush() {
    QMutexLocker locker(&m_mutex);

    if (m_pendingData.isEmpty()) {
        return;
    }

    qDebug() << "Flushing" << m_pendingData.size() << "commands";

    for (auto it = m_pendingData.constBegin(); it != m_pendingData.constEnd(); ++it) {
        qDebug() << "   → dataReady(" << static_cast<int>(it.key()) << "," << it.value() << ")";
        emit dataReady(it.key(), it.value());
    }

    m_pendingData.clear();
}
