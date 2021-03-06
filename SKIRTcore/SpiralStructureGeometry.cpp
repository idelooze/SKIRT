/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
///////////////////////////////////////////////////////////////// */

#include "FatalError.hpp"
#include "NR.hpp"
#include "Random.hpp"
#include "SpecialFunctions.hpp"
#include "SpiralStructureGeometry.hpp"

////////////////////////////////////////////////////////////////////

SpiralStructureGeometry::SpiralStructureGeometry()
    : GenGeometry(),
      _geometry(0),
      _m(0), _p(0), _R0(0), _phi0(0), _w(0), _N(0), _tanp(0), _CN(0),
      _Nphi(0), _dphi(0), _Ngamma(0), _dgamma(0), _Xvv()
{
}

//////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setupSelfBefore()
{
    GenGeometry::setupSelfBefore();

    // verify property values
    if (_m <= 0) throw FATALERROR("The number of spiral arms should be positive");
    if (_p <= 0 || _p >= M_PI/2.) throw FATALERROR("The pitch angle should be between 0 and 90 degrees");
    if (_R0 <= 0) throw FATALERROR("The radius zero-point should be positive");
    if (_phi0 < 0 || _phi0 > 2.0*M_PI) throw FATALERROR("The phase zero-point should be between 0 and 360 degrees");
    if (_w <= 0 || _w > 1.) throw FATALERROR("The weight of the spiral perturbation should be between 0 and 1");
    if (_N < 0 || _N > 10) throw FATALERROR("The arm-interarm size ratio index should be between 0 and 10");

    // cache frequently used values
    _tanp = tan(_p);
    _CN = sqrt(M_PI) * SpecialFunctions::gamma(_N+1.0) / SpecialFunctions::gamma(_N+0.5);

    // setup the vector of phi values for the cumulative distribution
    _Nphi = 360;
    _dphi = 2.0*M_PI/_Nphi;
    _Ngamma = 720;
    _dgamma = 2.0*M_PI/_Ngamma;
    _Xvv.resize(_Ngamma+1, _Nphi+1);
    for (int k=0; k<=_Ngamma; k++)
    {
        double gamma = k*_dgamma;
        for (int i=0; i<=_Nphi; i++)
        {
            double phi = i*_dphi;
            _Xvv(k,i) = 1.0/(2.0*M_PI)
                * ( (1.0-_w)*phi + 2.0*_w*_CN/_m *
                    (integratesin2n(0.5*_m*gamma,_N) - integratesin2n(0.5*_m*(gamma-phi),_N)));
        }
    }
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setGeometry(AxGeometry* value)
{
    if (_geometry) delete _geometry;
    _geometry = value;
    if (_geometry) _geometry->setParent(this);
}

////////////////////////////////////////////////////////////////////

AxGeometry*
SpiralStructureGeometry::geometry()
const
{
    return _geometry;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setArms(int value)
{
    _m = value;
}

////////////////////////////////////////////////////////////////////

int
SpiralStructureGeometry::arms()
const
{
    return _m;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setPitch(double value)
{
    _p = value;
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::pitch()
const
{
    return _p;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setRadius(double value)
{
    _R0 = value;
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::radius()
const
{
    return _R0;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setPhase(double value)
{
    _phi0 = value;
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::phase()
const
{
    return _phi0;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setPerturbWeight(double value)
{
    _w = value;
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::perturbWeight()
const
{
    return _w;
}

////////////////////////////////////////////////////////////////////

void
SpiralStructureGeometry::setIndex(int value)
{
    _N = value;
}

////////////////////////////////////////////////////////////////////

int
SpiralStructureGeometry::index()
const
{
    return _N;
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::density(Position bfr)
const
{
    double R, phi, z;
    bfr.cylindrical(R,phi,z);
    double gamma = log(R/_R0)/_tanp + _phi0 + 0.5*M_PI/_m;
    double perturbation = (1.0-_w) + _w*_CN*pow(sin(0.5*_m*(gamma-phi)),2*_N);
    return _geometry->density(R,z) * perturbation;
}

////////////////////////////////////////////////////////////////////

Position
SpiralStructureGeometry::generatePosition()
const
{
    Position bfr = _geometry->generatePosition();
    double R, dummyphi, z;
    bfr.cylindrical(R,dummyphi,z);
    double gamma = log(R/_R0)/_tanp + _phi0 + 0.5*M_PI/_m;
    gamma -= floor(gamma/(2.0*M_PI))*2.0*M_PI; // ensure that gamma is between 0 and 2*pi
    int k = static_cast<int>(gamma/_dgamma);
    const Array& Xv = _Xvv[k];
    double X = _random->uniform();
    int i = NR::locate_clip(Xv,X);
    double p = (X-Xv[i])/(Xv[i+1]-Xv[i]);
    double phi = (i+p)*_dphi;
    return Position(R,phi,z,Position::CYLINDRICAL);
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::SigmaX()
const
{
    return _geometry->SigmaX();
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::SigmaY()
const
{
    return _geometry->SigmaY();
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::SigmaZ()
const
{
    return _geometry->SigmaZ();
}

////////////////////////////////////////////////////////////////////

double
SpiralStructureGeometry::integratesin2n(double x, int n)
const
{
    double ans = x;
    for (int j=1; j<=n; j++)
        ans = (ans*(2.0*j-1.0)-cos(x)*pow(sin(x),2.0*j-1))/(2.0*j);
    return ans;
}

////////////////////////////////////////////////////////////////////
