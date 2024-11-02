#ifndef BT_MAN_H
#define BT_MAN_H

#include "httplib.h"
#include "nlohmann/json.hpp"
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QLowEnergyController>
#include <QtBluetooth/QLowEnergyService>
#include <QtBluetooth/QLowEnergyCharacteristic>
#include <thread>

class BluetoothManager : public QObject {
    Q_OBJECT
public:
    explicit BluetoothManager(QObject* parent = nullptr);
    ~BluetoothManager();

private:
    std::shared_ptr<httplib::Server> svr;
    std::unique_ptr<std::jthread> serverThread;
    std::string authToken;
    QBluetoothLocalDevice* localDevice;
    QBluetoothSocket* socket;
    std::map<QString, std::string> deviceCallbacks = {
        { "deviceDiscovered", "deviceDiscovered(const QBluetoothDeviceInfo& device)" },
        { "deviceConnected", "deviceConnected()" },
        { "deviceDisconnected", "deviceDisconnected()" },
        { "deviceError", "deviceError(QBluetoothLocalDevice::Error error)" },
        { "deviceStateChanged", "deviceStateChanged(QBluetoothLocalDevice::HostMode mode)" },
        { "devicePairingFinished", "devicePairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing)" },
        { "devicePairingDisplayConfirmation", "devicePairingDisplayConfirmation(const QBluetoothAddress& address, QString pin)" },
        { "devicePairingDisplayPinCode", "devicePairingDisplayPinCode(const QBluetoothAddress& address, QString pin)" },
        { "devicePairingError", "devicePairingError(QBluetoothLocalDevice::Error error)" },
        { "devicePairingFinished", "devicePairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing)" },
        { "devicePairingRequest", "devicePairingRequest(const QBluetoothAddress& address, QString pin)" },
        { "devicePairingSuccess", "devicePairingSuccess(const QBluetoothAddress& address)" },
        { "devicePairingSuccess", "devicePairingSuccess(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing)" }
    };
    void connectToDevice(const QString& address);
private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo& device);
    void deviceConnected();
    void deviceDisconnected();
    void deviceError(QBluetoothLocalDevice::Error error);
    void deviceStateChanged(QBluetoothLocalDevice::HostMode mode);
    void devicePairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);
    void devicePairingDisplayConfirmation(const QBluetoothAddress& address, QString pin);
    void devicePairingDisplayPinCode(const QBluetoothAddress& address, QString pin);
    void devicePairingError(QBluetoothLocalDevice::Error error);
    void devicePairingRequest(const QBluetoothAddress& address, QString pin);
    void devicePairingSuccess(const QBluetoothAddress& address);
    void devicePairingSuccess(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);
signals:
    void messageReceived(const QString& sender, const QString& message);
    void clientConnected(const QString& name);
    void clientDisconnected(const QString& name);
    void error(const QString& message);
public slots:
    void sendMessage(const QString& recipient, const QString& message);
};
#endif // BT_MAN_H