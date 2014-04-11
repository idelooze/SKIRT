/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
//////////////////////////////////////////////////////////////////*/

#include <QCoreApplication>
#include "SkirtCommandLineHandler.hpp"
#include "RegisterSimulationItems.hpp"
#include "SignalHandler.hpp"
#include "SkirtMain.hpp"

//////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    // construct application object for argument parsing and such,
    // but don't run the event loop because we don't need it
    QCoreApplication app(argc, argv);
    app.setApplicationName("SKIRT");
    app.setApplicationVersion("v6 (git 000 built on " + QString(__DATE__).simplified() + " at "  __TIME__ ")");

    // install C signal handlers (which throw an exception if all goes well)
    SignalHandler::InstallSignalHandlers();

    // initialize the class registry used for discovering simulation items
    RegisterSimulationItems::registerAll();

    // get and handle the command line arguments
    SkirtCommandLineHandler handler(app.arguments());
    return handler.perform();
}

//////////////////////////////////////////////////////////////////////
