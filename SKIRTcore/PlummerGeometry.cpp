/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
//////////////////////////////////////////////////////////////////*/

#include <cmath>
#include "FatalError.hpp"
#include "PlummerGeometry.hpp"
#include "Random.hpp"

using namespace std;

//////////////////////////////////////////////////////////////////////

PlummerGeometry::PlummerGeometry()
    : _c(0), _rho0(0)
{
}

//////////////////////////////////////////////////////////////////////

void PlummerGeometry::setupSelfBefore()
{
    SpheGeometry::setupSelfBefore();

    // verify property values
    if (_c <= 0) throw FATALERROR("the scale length c should be positive");

    // calculate cached values
    _rho0 = 0.75/pow(_c,3)/M_PI;
}

////////////////////////////////////////////////////////////////////

void
PlummerGeometry::setScale(double value)
{
    _c = value;
}

////////////////////////////////////////////////////////////////////

double
PlummerGeometry::scale()
const
{
    return _c;
}

//////////////////////////////////////////////////////////////////////

double
PlummerGeometry::density(double r)
const
{
    double s = r/_c;
    return _rho0 * pow(1.0+s*s,-2.5);
}

//////////////////////////////////////////////////////////////////////

double
PlummerGeometry::randomradius()
const
{
    double t = pow(_random->uniform(),1.0/3.0);
    return _c * t/sqrt((1.0-t)*(1.0+t));
}

//////////////////////////////////////////////////////////////////////

double
PlummerGeometry::Sigmar()
const
{
    return 0.5/(M_PI*_c*_c);
}

//////////////////////////////////////////////////////////////////////
