#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include "../BLE/BLEGattClient.h"

using namespace BLE;


class AbstractService{

public:
    virtual ~AbstractService(){};
    BleGattClient* bleClient = BleGattClient::instance();
    virtual void execute(const QVariant& var = QVariant()) = 0;
};

#endif // ABSTRACTSERVICE_H
