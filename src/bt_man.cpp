#include "bt_man.h"

BluetoothManager::BluetoothManager(QObject* parent){
    this->svr = std::make_shared<httplib::Server>();
    this->serverThread = std::make_unique<std::jthread>([&]{
        svr->Get("/hello", [&](const httplib::Request& req, httplib::Response& res){
            std::string authHeader = req.get_header_value("Authorization");
            if(authHeader != "Bearer " + this->authToken){
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }
            res.set_content("Hello World!", "text/plain");
        });
        svr->listen("localhost", 8080);
    });
    this->localDevice = new QBluetoothLocalDevice();
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    this->localDevice->powerOn();
}

BluetoothManager::~BluetoothManager(){

}

void BluetoothManager::connectToDevice(const QString& address){

}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo& device){

}

void BluetoothManager::deviceConnected(){

}

void BluetoothManager::deviceDisconnected(){

}

void BluetoothManager::deviceError(QBluetoothLocalDevice::Error error){

}

void BluetoothManager::deviceStateChanged(QBluetoothLocalDevice::HostMode mode){

}

void BluetoothManager::devicePairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing){

}

void BluetoothManager::devicePairingDisplayConfirmation(const QBluetoothAddress& address, QString pin){

}

void BluetoothManager::devicePairingDisplayPinCode(const QBluetoothAddress& address, QString pin){

}

void BluetoothManager::devicePairingError(QBluetoothLocalDevice::Error error){

}

void BluetoothManager::devicePairingRequest(const QBluetoothAddress& address, QString pin){

}

void BluetoothManager::devicePairingSuccess(const QBluetoothAddress& address){

}

void BluetoothManager::devicePairingSuccess(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing){

}

void BluetoothManager::sendMessage(const QString& recipient, const QString& message){

}
