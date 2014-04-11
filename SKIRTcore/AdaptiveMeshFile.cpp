/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
//////////////////////////////////////////////////////////////////*/

#include "AdaptiveMeshFile.hpp"

////////////////////////////////////////////////////////////////////

AdaptiveMeshFile::AdaptiveMeshFile()
{
}

//////////////////////////////////////////////////////////////////////

void AdaptiveMeshFile::setFilename(QString value)
{
    _filename = value;
}

//////////////////////////////////////////////////////////////////////

QString AdaptiveMeshFile::filename() const
{
    return _filename;
}

////////////////////////////////////////////////////////////////////
