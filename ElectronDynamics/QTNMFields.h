/*
  QTNMFields.h

  Derived QTNM field classes

  Transcription of T. Goffrey's original Python code 
*/

#ifndef QTNM_FIELDS_H
#define QTNM_FIELDS_H

#include "ElectronDynamics/BaseField.h"
#include "BasicFunctions/Constants.h"

#include "TVector3.h"

namespace rad
{
  /// Class describing the magnetic field of an infinitesimally thin current coil
  /// in the x-y plane
  class CoilField : public BaseField {
  private:
    double coilRadius;
    double coilCurrent;
    double coilZ;
    double coilMu;

    double central_field();

    double on_axis_field(const double z);
    
  public:
    /// \param radius The radius of the coil (in metres)
    /// \param current The current passing through the coil (in Amps)
    /// \param z The z position of the coil (in metres)
    /// \param Magnetic permeability
    CoilField(const double radius=0.005, const double current=40, const double z=0.0, const double mu=MU0);

    /// \param vec Position vector of charge
    /// \returns The magnetic field vector at the point
    TVector3 evaluate_field_at_point(const TVector3 vec);
  };

  /// Class describing the bathtub field
  /// Made up of two superposed current loops and a background magnetic field
  /// The coils must be located along the z axis 
  class BathtubField : public BaseField {
  private:
    CoilField coil1;
    CoilField coil2;
    TVector3 btBkg;
    
  public:
    /// \param radius The radius of both coils
    /// \param The current (in amps) flowing through each trap coil
    /// \param Z1 The z position of the centre of the first trap coil
    /// \param Z2 The z position of the centre of the second trap coil
    /// \param background A vector describing the background field in Tesla
    BathtubField(const double radius, const double current, const double Z1, const double Z2, TVector3 background);

    /// \param vec Position vector of charge
    /// \returns The magnetic field vector at the point
    TVector3 evaluate_field_at_point(const TVector3 vec);
  };

  /// Class describing the field of a finite solenoid
  /// Axis of the solenoid coincides with z axis with origin at z = 0
  class SolenoidField : public BaseField {
  private:
    double r;
    double l;
    double i;
    double n;
    double mu;

    /// Gives the on axis field for the solenoid
    /// \param z The position along the z axis
    /// \Returns The magnetic field in Tesla
    double on_axis_field(const double z);
    
  public:
    /// \param radius The internal radius of the solenoid (in metres)
    /// \param length The length of the bore of the solenoid (in metres)
    /// \param current The current (in amps) passing through each filament
    /// \param turnsPerMetre Number of coil terms per metre
    SolenoidField(const double radius, const double length, const double current, const double turnsPerMetre, const double perm=MU0) : r(radius), l(length), i(current), n(turnsPerMetre), mu(perm) {}

    /// Gives the field at a positon vector 
    /// \param vec Position vector of charge
    /// \Returns The magnetic field vector at the point (in Tesla)
    TVector3 evaluate_field_at_point(const TVector3 vec);
  };

  /// Class describing something like a non-ideal background field, i.e. from a finite solenoid
  /// The field variation is expressed as a quadratic function of the axial distance
  /// The axis of the field coincides with the z axis
  /// Inhomogeneity only varies as a function of z
  class InhomogeneousBackgroundField : public BaseField {
  private:
    double maxB;
    double inhom;
    double inhomZ;

  public:
    /// Parametrised constructor for the inhomogeneous background field
    /// \param maximumField The maximum central field
    /// \param fractionalInhom The maximum inhomogeneity as a fraction of the maximum field
    /// \param inhomZPos Z position at which the inhomogeneity reaches fractionalInhom
    InhomogeneousBackgroundField(const double maximumField, const double fractionalInhom, const double inhomZPos) : maxB(maximumField), inhom(fractionalInhom), inhomZ(inhomZPos) {}

    /// Gives the field at a positon vector 
    /// \param vec Position vector of charge
    /// \Returns The magnetic field vector at the point (in Tesla)
    TVector3 evaluate_field_at_point(const TVector3 vec);
  };
}

#endif
