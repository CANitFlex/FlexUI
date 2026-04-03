#ifndef DEVICECOMMANDEXECUTER_H
#define DEVICECOMMANDEXECUTER_H

#include <QObject>

class DeviceCommandExecuter : public QObject
{
    Q_OBJECT
public:
    explicit DeviceCommandExecuter(QObject *parent = nullptr);

signals:
};

#endif // DEVICECOMMANDEXECUTER_H
