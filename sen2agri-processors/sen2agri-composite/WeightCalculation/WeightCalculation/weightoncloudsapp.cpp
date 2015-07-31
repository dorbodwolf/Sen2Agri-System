
/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "cloudmaskbinarization.h"
#include "cloudsinterpolation.h"
#include "gaussianfilter.h"
#include "cloudweightcomputation.h"


namespace otb
{

namespace Wrapper
{

class WeightOnCloudsApp : public Application
{

public:
  /** Standard class typedefs. */
  typedef WeightOnCloudsApp                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(WeightOnCloudsApp, otb::Application);

private:
  void DoInit()
  {
    SetName("WeightOnClouds");
    SetDescription("Computes the cloud weight value for the given mask cloud and parameters");

    SetDocName("Weight on Clouds");
    SetDocLongDescription("This application implements the Weight On Clouds algorithm for the Composite Processor");

    SetDocLimitations("None");
    SetDocAuthors("CIU");
    SetDocSeeAlso(" ");
    AddDocTag("Util");

    AddParameter(ParameterType_String,  "incldmsk",   "Input cloud mask image");
    SetParameterDescription("incldmsk", "Image containing the cloud mask.");

    AddParameter(ParameterType_Int, "incldmskres", "Input cloud mask resolution");
    SetParameterDescription("incldmskres", "The resolution of the input cloud mask image.");

    AddParameter(ParameterType_Int, "coarseres", "Coarse resolution");
    SetParameterDescription("coarseres", "The resolution for the undersampling.");
    SetDefaultParameterInt("coarseres", 240);

    AddParameter(ParameterType_Float, "sigmasmallcld", "Small cloud sigma");
    SetParameterDescription("sigmasmallcld", "Sigma value for the small cloud gaussian filter.");

    AddParameter(ParameterType_Float, "sigmalargecld", "Large cloud sigma");
    SetParameterDescription("sigmalargecld", "Sigma value for the large cloud gaussian filter.");

    AddParameter(ParameterType_Int, "outres", "Resolution of the output image");
    SetParameterDescription("coarseres", "The resolution at which the output will be produced.");

    AddParameter(ParameterType_OutputImage, "outcldweight", "Output Cloud Weight Image");
    SetParameterDescription("outcldweight","The output image containg the computed cloud weight for each pixel.");

    // Doc example parameter settings
    SetDocExampleParameterValue("incldmsk", "verySmallFSATSW_r.tif");
    SetDocExampleParameterValue("incldmskres", "20");
    SetDocExampleParameterValue("coarseres", "240");
    SetDocExampleParameterValue("sigmasmallcld", "10.0");
    SetDocExampleParameterValue("sigmalargecld", "50.0");
    SetDocExampleParameterValue("outres", "10");
    SetDocExampleParameterValue("outcldweight", "apAOTWeightOutput.tif");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    // Get the input image list
    std::string inCldFileName = GetParameterString("incldmsk");
    if (inCldFileName.empty())
    {
        itkExceptionMacro("No input Image set...; please set the input image");
    }
    int inputCloudMaskResolution = GetParameterInt("incldmskres");
    int coarseResolution = GetParameterInt("coarseres");
    float sigmaSmallCloud = GetParameterFloat("sigmasmallcld");
    float sigmaLargeCloud = GetParameterFloat("sigmalargecld");
    int outputResolution = GetParameterInt("outres");

    m_cloudMaskBinarization.SetInputFileName(inCldFileName);
    m_cloudMaskBinarization.Update();

    m_underSampler.SetInputImageReader(m_cloudMaskBinarization.GetOutputImageSource());
    m_underSampler.SetInputResolution(inputCloudMaskResolution);
    m_underSampler.SetOutputResolution(coarseResolution);
    m_underSampler.Update();

    // Compute the DistLargeCloud, Low Res
    m_gaussianFilterSmallCloud.SetInputImageReader(m_underSampler.GetOutputImageSource());
    m_gaussianFilterSmallCloud.SetSigma(sigmaSmallCloud);
    m_gaussianFilterSmallCloud.Update();

    m_gaussianFilterLargeCloud.SetInputImageReader(m_underSampler.GetOutputImageSource());
    m_gaussianFilterLargeCloud.SetSigma(sigmaLargeCloud);
    m_gaussianFilterLargeCloud.Update();

    // resample at the current small resolution (10 or 20) the small cloud large resolution image
    m_overSamplerSmallCloud.SetInputImageReader(m_gaussianFilterSmallCloud.GetOutputImageSource());
    m_overSamplerSmallCloud.SetInputResolution(coarseResolution);
    m_overSamplerSmallCloud.SetOutputResolution(outputResolution);
    m_overSamplerSmallCloud.SetInterpolator(Interpolator_Linear);
    m_overSamplerSmallCloud.Update();

    // resample at the current small resolution (10 or 20) the large cloud large resolution image
    m_overSamplerLargeCloud.SetInputImageReader(m_gaussianFilterLargeCloud.GetOutputImageSource());
    m_overSamplerLargeCloud.SetInputResolution(coarseResolution);
    m_overSamplerLargeCloud.SetOutputResolution(outputResolution);
    m_overSamplerLargeCloud.SetInterpolator(Interpolator_Linear);
    m_overSamplerLargeCloud.Update();

    // compute the weight on clouds
    m_cloudWeightComputation.SetInputImageReader1(m_overSamplerSmallCloud.GetOutputImageSource());
    m_cloudWeightComputation.SetInputImageReader2(m_overSamplerLargeCloud.GetOutputImageSource());
    m_cloudWeightComputation.Update();

    // Set the output image
    SetParameterOutputImage("outcldweight", m_cloudWeightComputation.GetOutputImageSource()->GetOutput());
  }

  CloudMaskBinarization m_cloudMaskBinarization;
  CloudsInterpolation m_underSampler;
  GaussianFilter m_gaussianFilterSmallCloud;
  GaussianFilter m_gaussianFilterLargeCloud;
  CloudsInterpolation m_overSamplerSmallCloud;
  CloudsInterpolation m_overSamplerLargeCloud;
  CloudWeightComputation m_cloudWeightComputation;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::WeightOnCloudsApp)
