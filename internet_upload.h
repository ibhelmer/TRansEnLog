

#ifndef internet_upload_h
#define internet_upload_h

#include <mbed.h>
#include <SDFileSystem.h>
#include <wireless/module_communication.h>
#include <mn_string.h>
#include <http_client.h>

using namespace mono;

class InternetUpload {
 
    bool fsExist;
    String ssid;
    String password;
    String url;
    timestamp_t lastSendTime;

    SDFileSystem sdfs;
    mbed::SPI rpSpi;
    redpine::ModuleSPICommunication spiComm;
    network::HttpClient client;

protected:

    void wifiConnected();

public:

    mbed::FunctionPointer wifiStarted;

    InternetUpload();

    void init();

    void connectWifi();

    void beginDownload();

    void httpCompletion(network::INetworkRequest::CompletionEvent *evnt);

    void httpData(network::HttpClient::HttpResponseData const &data);
};

#endif /* internet_upload_h */
