#include "internet_upload.h"
#include <io/file.h>
#include <wireless/redpine_module.h>
#include <application_context_interface.h>

using namespace mono;
using namespace mono::io;
using namespace mono::network;

InternetUpload::InternetUpload() :
    sdfs(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_CLK, SD_SPI_CS, "sd"),
    rpSpi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK),
    spiComm(rpSpi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT)
{
    lastSendTime = 0;
}

void InternetUpload::init()
{
    ssid = String("IHNMobile");
    password = String("10244120");
    url = String("www.ihn.dk");
   
   // ssid = String("WiFI-IHN-1");
   // password = String("*******");
   // url = String("192.168.1.124:7913");	
   // connectWifi();
}

void InternetUpload::connectWifi()
{
    redpine::Module::initialize(&spiComm);

    redpine::Module::setNetworkReadyCallback<InternetUpload>(this, &InternetUpload::wifiConnected);

    redpine::Module::setupWifiOnly(ssid, password);
}


/// MARK: Protected Methods

void InternetUpload::wifiConnected()
{
    if (!redpine::Module::IsNetworkReady())
        return;

    beginDownload();
    wifiStarted.call();
}

void InternetUpload::beginDownload()
{
    if (!redpine::Module::IsNetworkReady())
        return;
    int mcelcius = IApplicationContext::Instance->Temperature->ReadMilliCelcius();
    String s = String::Format("%s?celcius=%i",url(),mcelcius);
    client = HttpClient(s);
    client.setCompletionCallback<InternetUpload>(this, &InternetUpload::httpCompletion);
    lastSendTime = us_ticker_read();
}

void InternetUpload::httpCompletion(network::INetworkRequest::CompletionEvent *evnt)
{

    if (!evnt->Context->HasFailed())
    {
        if (us_ticker_read() - lastSendTime >= 1000000)
        {
            beginDownload();
        }
        else
        {
            int delay = lastSendTime+1000000 - us_ticker_read();
            Timer::callOnce<InternetUpload>(delay>0?delay/1000:0, this, &InternetUpload::beginDownload);
        }

    }
}

void InternetUpload::httpData(network::HttpClient::HttpResponseData const &data)
{
    printf("%s",data.bodyChunk());
}
