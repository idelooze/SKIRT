/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
//////////////////////////////////////////////////////////////////*/

#ifndef READFITSGEOMETRY_HPP
#define READFITSGEOMETRY_HPP

#include <valarray>
#include <vector>
#include "GenGeometry.hpp"

////////////////////////////////////////////////////////////////////

/** The ReadFitsGeometry class is a subclass of the GenGeometry class, and describes
    a geometry characterized by observations. A 2D observed image fits file is read into SKIRT,
    and deprojected assuming a certain position angle, inclination and azimuth. The density is
    assumed to follow a exponential profile in the vertical directions, \f[ \rho(z) =
    \rho_0\, \exp\left(-\frac{|z|}{h_z}\right). \f] By running a SKIRT simulation with inclination
    of 0 degrees and position angle of the simulated galaxy, the SKIRT model images can be
    compared with the observations. The model geometry is set by nine parameters: the input filename,
    the pixel scale \f$pix\f$, the position angle \f$pa\f$, the inclination \f$incl\f$,
    the number of pixels in x and y direction \f$n_x\f$ and \f$n_y\f$,
    the center of galaxy in (x,y) image coordinates \f$x_c\f$ and \f$y_c\f$
    and the vertical scale height \f$h_z\f$. */
class ReadFitsGeometry : public GenGeometry
{

    Q_OBJECT
    Q_CLASSINFO("Title", "a read from image geometry")

    Q_CLASSINFO("Property", "filename")
    Q_CLASSINFO("Title", "the name of the file with the image parameters")

    Q_CLASSINFO("Property", "pixelScale")
    Q_CLASSINFO("Title", "the physical pixel scale")
    Q_CLASSINFO("Quantity", "length")
    Q_CLASSINFO("MinValue", "0")

    Q_CLASSINFO("Property", "positionAngle")
    Q_CLASSINFO("Title", "the position angle")
    Q_CLASSINFO("Quantity", "angle")
    Q_CLASSINFO("MinValue", "-360")
    Q_CLASSINFO("MinValue", "360")

    Q_CLASSINFO("Property", "inclination")
    Q_CLASSINFO("Title", "the inclination of the system")
    Q_CLASSINFO("Quantity", "angle")
    Q_CLASSINFO("MinValue", "0")
    Q_CLASSINFO("MaxValue", "180")

    Q_CLASSINFO("Property", "xelements")
    Q_CLASSINFO("Title", "number of elements in X")
    Q_CLASSINFO("MinValue", "0")

    Q_CLASSINFO("Property", "yelements")
    Q_CLASSINFO("Title", "number of elements in Y")
    Q_CLASSINFO("MinValue", "0")

    Q_CLASSINFO("Property", "xcenter")
    Q_CLASSINFO("Title", "center in X coordinates")
    Q_CLASSINFO("MinValue", "0")

    Q_CLASSINFO("Property", "ycenter")
    Q_CLASSINFO("Title", "center in Y coordinates")
    Q_CLASSINFO("MinValue", "0")

    Q_CLASSINFO("Property", "axialScale")
    Q_CLASSINFO("Title", "the axial scale height")
    Q_CLASSINFO("Quantity", "length")
    Q_CLASSINFO("MinValue", "0")

    //============= Construction - Setup - Destruction =============

public:
    /** The default constructor. */
    Q_INVOKABLE ReadFitsGeometry();

protected:
    /** This function verifies the validity of the pixel scale, the inclination angle, the number of
        pixels in the x and y direction, the center of the image in x and y coordinates
        and the vertical scale height \f$h_z\f$. A vector of normalized cumulative pixel luminosities
        is computed, satisfying the condition that the total mass equals 1.*/
    void setupSelfBefore();

    //======== Setters & Getters for Discoverable Attributes =======

public:
    /** Sets the name of the file with the image parameters. */
    Q_INVOKABLE void setFilename(QString value);

    /** Returns the name of the file with the image parameters. */
    Q_INVOKABLE QString filename() const;

    /** Sets the pixel scale. */
    Q_INVOKABLE void setPixelScale(double value);

    /** Returns the pixel scale. */
    Q_INVOKABLE double pixelScale() const;

    /** Sets the position angle. */
    Q_INVOKABLE void setPositionAngle(double value);

    /** Returns the position angle. */
    Q_INVOKABLE double positionAngle() const;

    /** Sets the inclination. */
    Q_INVOKABLE void setInclination(double value);

    /** Returns the inclination. */
    Q_INVOKABLE double inclination() const;

    /** Sets the number of elements in X. */
    Q_INVOKABLE void setXelements(int value);

    /** Returns the number of elements in X. */
    Q_INVOKABLE int xelements() const;

    /** Sets the number of elements in Y. */
    Q_INVOKABLE void setYelements(int value);

    /** Returns the number of elements in Y. */
    Q_INVOKABLE int yelements() const;

    /** Sets the center in X coordinates. */
    Q_INVOKABLE void setXcenter(double value);

    /** Returns the center in X coordinates. */
    Q_INVOKABLE double xcenter() const;

    /** Sets the center in Y coordinates. */
    Q_INVOKABLE void setYcenter(double value);

    /** Returns the center in Y coordinates. */
    Q_INVOKABLE double ycenter() const;

    /** Sets the axial scale height. */
    Q_INVOKABLE void setAxialScale(double value);

    /** Returns the axial scale height. */
    Q_INVOKABLE double axialScale() const;

    //======================== Other Functions =======================

    /** This function returns the density \f$\rho(x,y,z)\f$ at the position (x,y,z). */
    double density(Position bfr) const;

    /** This function generates a random position (x,y,z) from the geometry, by drawing a random
        point from the appropriate probability density distribution function. The (x,y) coordinates
        are derived from the normalized cumulative luminosity vector of the observed 2D projection.
        The z coordinate is derived from the vertical exponential probability distribution function. */
    Position generatePosition() const;

    double SigmaX() const;

    double SigmaY() const;

    double SigmaZ() const;

    //======================== Data Members ========================

private:
    QString _filename;
    double _pix;
    double _pa;
    double _incl;
    int _nx;
    int _ny;
    double _xc;
    double _yc;
    double _hz;

    // data members initialized during setup
    double _xpmax, _ypmax, _xpmin, _ypmin;
    std::vector<double> _Lv;
    std::vector<double> _Xv;
};

////////////////////////////////////////////////////////////////////

#endif // READFITSGEOMETRY_HPP
