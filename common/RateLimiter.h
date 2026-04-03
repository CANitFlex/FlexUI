#ifndef RATELIMITER_H
#define RATELIMITER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QVariant>
#include "Enums.h"

class RateLimiter : public QObject {
    Q_OBJECT
public:

    static RateLimiter* instance(QObject *parent = nullptr);

    void queue(Enums::Services service, const QVariant& value);
    void start();
    void stop();

signals:
    void dataReady(Enums::Services service, const QVariant& value);

private slots:
    void flush();

private:
    explicit RateLimiter(QObject *parent = nullptr);

    int fps = 20;
    QTimer m_timer;
    int m_intervalMs;
    QMutex m_mutex;
    QMap<Enums::Services, QVariant> m_pendingData; 
    bool m_hasPending = false;
};
#endif // RATELIMITER_H
