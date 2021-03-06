/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
///////////////////////////////////////////////////////////////// */

#include <fstream>
#include <iomanip>
#include "DistantInstrument.hpp"
#include "FatalError.hpp"
#include "FilePaths.hpp"
#include "Log.hpp"
#include "PhotonPackage.hpp"
#include "Units.hpp"
#include "WavelengthGrid.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////

DistantInstrument::DistantInstrument()
    : _distance(0), _azimuth(0), _inclination(0), _positionangle(0)
{
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::setupSelfBefore()
{
    Instrument::setupSelfBefore();

    // verify attribute values
    if (_distance <= 0) throw FATALERROR("Distance was not set");

    // calculate derived values
    _bfkobs = Direction(_inclination,_azimuth);
    _costheta = cos(_inclination);
    _sintheta = sin(_inclination);
    _cosphi = cos(_azimuth);
    _sinphi = sin(_azimuth);
    _cospa = cos(_positionangle);
    _sinpa = sin(_positionangle);
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::setDistance(double value)
{
    _distance = value;
}

////////////////////////////////////////////////////////////////////

double DistantInstrument::distance() const
{
    return _distance;
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::setInclination(double value)
{
    _inclination = value;
}

////////////////////////////////////////////////////////////////////

double DistantInstrument::inclination() const
{
    return _inclination;
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::setAzimuth(double value)
{
    _azimuth = value;
}

////////////////////////////////////////////////////////////////////

double DistantInstrument::azimuth() const
{
    return _azimuth;
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::setPositionAngle(double value)
{
    _positionangle = value;
}

////////////////////////////////////////////////////////////////////

double DistantInstrument::positionAngle() const
{
    return _positionangle;
}

////////////////////////////////////////////////////////////////////

Direction DistantInstrument::bfkobs(const Position& /*bfr*/) const
{
    return _bfkobs;
}

////////////////////////////////////////////////////////////////////

void DistantInstrument::calibrateAndWriteSEDs(QList< Array* > Farrays, QStringList Fnames)
{
    WavelengthGrid* lambdagrid = find<WavelengthGrid>();
    int Nlambda = find<WavelengthGrid>()->Nlambda();

    // calibration step 1: conversion from bolometric luminosities (units W) to monochromatic luminosities (units W/m)

    for (int ell=0; ell<Nlambda; ell++)
    {
        double dlambda = lambdagrid->dlambda(ell);
        foreach (Array* Farr, Farrays)
        {
            (*Farr)[ell] /= dlambda;
        }
    }

    // calibration step 2: conversion of the integrated flux from monochromatic luminosity units (W/m) to
    //                     flux density units (W/m3) by taking into account the distance

    double fourpid2 = 4.0*M_PI*_distance*_distance;
    foreach (Array* Farr, Farrays)
    {
        (*Farr) /= fourpid2;
    }

    // write a text file for easy SED plotting

    Units* units = find<Units>();
    QString sedfilename = find<FilePaths>()->output(_instrumentname + "_sed.dat");
    find<Log>()->info("Writing SED to " + sedfilename + "...");
    ofstream sedfile(sedfilename.toLocal8Bit().constData());
    sedfile << "# column 1: lambda (" << units->uwavelength().toStdString() << ")\n";
    for (int q = 0; q < Farrays.size(); q++)
    {
        sedfile << "# column " << (q+2) << ": "
                << Fnames[q].toStdString() << "; "
                << units->sfluxdensity().toStdString() << " "
                << "(" << units->ufluxdensity().toStdString() << ")\n";
    }
    sedfile << scientific << setprecision(8);
    for (int ell=0; ell<Nlambda; ell++)
    {
        double lambda = lambdagrid->lambda(ell);
        sedfile << units->owavelength(lambda);
        for (int q = 0; q < Farrays.size(); q++)
        {
            sedfile << '\t' << units->ofluxdensity(lambda, (*(Farrays[q]))[ell]);
        }
        sedfile << endl;
    }
}

////////////////////////////////////////////////////////////////////
