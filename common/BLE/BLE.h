#include <memory>

namespace BLE {
    class Device;
    using Device_uPtr = std::unique_ptr<Device>;
    class DeviceScanner;
    class ConnectionManager;
    class ServiceDiscovery;
    class Sender;
}
