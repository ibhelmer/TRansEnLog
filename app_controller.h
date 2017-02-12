// Mono Application 
// TRANSportation ENvironment data LOGger (TransEnLog) 
// V1.0 Ib Helmer Nielsen, UCN

#ifndef app_controller_h
#define app_controller_h

// Include the Mono Framework
#include <mono.h>
#include <wireless/module_communication.h>
#include <wireless/redpine_module.h>

// Import the mono and mono::ui namespaces into the context
// to avoid writing long type names, like mono::ui::TextLabel
using namespace mono;
using namespace mono::ui;

// The App main controller object.
// This template app will show a "hello" text in the screen
class AppController : public mono::IApplication {

    // This is the text label object that will displayed
    TextLabelView helloLabel;
	// The SPI port to the Redpine module
	mbed::SPI spi;
	// The spi based communication interface to the module
	redpine::ModuleSPICommunication spiComm;
public:

    // The default constructor
    AppController();

    // Called automaticlly by Mono on device reset
    void monoWakeFromReset();

    // Called automatically by Mono just before it enters sleep mode
    void monoWillGotoSleep();

    // Called automatically by Mono right after after it wakes from sleep
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
