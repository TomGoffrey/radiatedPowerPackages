/*
  Signal.h
  Class for a signal detected on an antenna or antennas
*/

#ifndef SIGNAL_H
#define SIGNAL_H

#include "SignalProcessing/InducedVoltage.h"
#include "SignalProcessing/LocalOscillator.h"
#include "SignalProcessing/NoiseFunc.h"
#include "FieldClasses/FieldClasses.h"
#include "BasicFunctions/BasicFunctions.h"

#include <vector>

#include "TGraph.h"
#include "TH2.h"

namespace rad
{
  /// Signal base class
  /// Class members are:
  /// grVITime In phase processed voltage component
  /// grVQTime Quadrature processed voltage component
  /// sampleRate Hypothetical sample rate of a digitiser
  
  class Signal
  {
  private:
    TGraph* grVITime; // In phase component
    TGraph* grVQTime; // Quadrature component
    double sampleRate;
    
    TGraph* DownmixInPhase(TGraph* grInput, LocalOscillator lo);
    TGraph* DownmixQuadrature(TGraph* grInput, LocalOscillator lo);

    /// Function for performing downsampling on a time domain wave using the class sample rate
    /// \param grInput The input time domain waveform
    /// Returns the downsampled waveform    
    TGraph* SampleWaveform(TGraph* grInput);

    /// Function for performing downsampling on a time domain wave
    /// \param grInput The input time domain waveform
    /// \param sRate The designated sample rate
    /// Returns the downsampled waveform
    TGraph* SampleWaveform(TGraph* grInput, const double sRate);
    
    void AddGaussianNoise(TGraph* grInput, std::vector<GaussianNoise> noiseTerms);
    
  public:
    ~Signal();
    
    /// \param fp The field points where the antenna is located
    /// \param lo The local oscillator used to do the downmixing
    /// \param srate The sample rate of a hypothetical ADC
    /// \param noiseTerms A vector of noise terms to be added to the signal
    /// \param kUseRetardedTime Boolean on whether or not to use the retarded time
    Signal(std::vector<FieldPoint> fp, LocalOscillator lo, double srate, std::vector<GaussianNoise> noiseTerms={}, const bool kUseRetardedTime=false);

    /// Constructor for a single field point
    /// \param fp The field point where the antenna is located
    /// \param lo The local oscillator used to do the downmixing
    /// \param srate The sample rate of a hypothetical ADC
    /// \param noiseTerms A vector of noise terms to be added to the signal
    /// \param kUseRetardedTime Boolean on whether or not to use the retarded time
    Signal(FieldPoint fp, LocalOscillator lo, double srate, std::vector<GaussianNoise> noiseTerms={}, const bool kUseRetardedTime=false);

    /// Constructor for a single induced voltage
    /// \param iv The induced voltage at a particular field point
    /// \param lo The local oscillator used to do the downmixing
    /// \param srate The sample rate of a hypothetical ADC
    /// \param noiseTerms A vector of noise terms to be added to the signal
    Signal(InducedVoltage iv, LocalOscillator lo, double srate,
	   std::vector<GaussianNoise> noiseTerms={});
    
    /// Copy constructor
    Signal(const Signal &s1);

    /// Returns the summed in-phase voltage component after all signal processing
    TGraph* GetVITimeDomain(int firstPoint=-1, int lastPoint=-1);
    /// Returns the summed quadrature voltage component after all signal processing
    TGraph* GetVQTimeDomain(int firstPoint=-1, int lastPoint=-1);

    /// Returns the power spectrum of the in-phase voltage component after all signal processing
    /// \param loadResistance The load resistance used for the power calculation
    /// \param firstPoint The first point to return
    /// \param lastPoint The last point to return
    TGraph* GetVIPowerNorm(const double loadResistance, int firstPoint=-1, int lastPoint=-1);

    /// Returns the power spectrum of the quadrature voltage component after all signal processing
    /// \param loadResistance The load resistance used for the power calculation
    TGraph* GetVQPowerNorm(const double loadResistance, int firstPoint=-1, int lastPoint=-1);

    /// Returns the 2D spectrogram made using the in-phase voltage component
    /// \param loadResistance The load resistance used for the power calculation
    /// \param NSamplesPerTimeBin The number of time samples used to make each time bin
    TH2D* GetVISpectrogram(const double loadResistance, const int NSamplesPerTimeBin);

    /// Returns the 2D spectrogram made using the quadrature voltage component
    /// \param loadResistance The load resistance used for the power calculation
    /// \param NSamplesPerTimeBin The number of time samples used to make each time bin
    TH2D* GetVQSpectrogram(const double loadResistance, const int NSamplesPerTimeBin);
  };
}

#endif
