// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: structural_mechanics_application/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//
// System includes
#include <iostream>

// External includes

// Project includes
#include "includes/properties.h"
#include "custom_constitutive/dummy_truss_constitutive_law.h"

namespace Kratos
{

//******************************CONSTRUCTOR*******************************************
//************************************************************************************

DummyTrussConstitutiveLaw::DummyTrussConstitutiveLaw()
    : ConstitutiveLaw()
{
}

//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

DummyTrussConstitutiveLaw::DummyTrussConstitutiveLaw(const DummyTrussConstitutiveLaw& rOther)
    : ConstitutiveLaw(rOther)
{
}

//********************************CLONE***********************************************
//************************************************************************************

ConstitutiveLaw::Pointer DummyTrussConstitutiveLaw::Clone() const
{
    DummyTrussConstitutiveLaw::Pointer p_clone(new DummyTrussConstitutiveLaw(*this));
    return p_clone;
}

//*******************************DESTRUCTOR*******************************************
//************************************************************************************

DummyTrussConstitutiveLaw::~DummyTrussConstitutiveLaw()
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void  DummyTrussConstitutiveLaw::CalculateMaterialResponsePK2 (Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::CalculateMaterialResponseKirchhoff (Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::CalculateMaterialResponsePK1 (Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::CalculateMaterialResponseCauchy (Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::FinalizeMaterialResponsePK1(Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::FinalizeMaterialResponsePK2(Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::FinalizeMaterialResponseCauchy(Parameters& rValues)
{
    // TODO: Add if necessary
}

//************************************************************************************
//************************************************************************************

void DummyTrussConstitutiveLaw::FinalizeMaterialResponseKirchhoff(Parameters& rValues)
{
    // TODO: Add if necessary
}

//*************************CONSTITUTIVE LAW GENERAL FEATURES *************************
//************************************************************************************

void DummyTrussConstitutiveLaw::GetLawFeatures(Features& rFeatures)
{
    //Set the strain size
    rFeatures.mStrainSize = 1;

    //Set the spacedimension
    rFeatures.mSpaceDimension = 3;
}

//************************************************************************************
//************************************************************************************

int DummyTrussConstitutiveLaw::Check(
    const Properties& rMaterialProperties,
    const GeometryType& rElementGeometry,
    const ProcessInfo& rCurrentProcessInfo
)
{
    if(YOUNG_MODULUS.Key() == 0 || rMaterialProperties[YOUNG_MODULUS] <= 0.00)
    {
        KRATOS_ERROR << "YOUNG_MODULUS has Key zero or invalid value " << std::endl;
    }

    const double& nu = rMaterialProperties[POISSON_RATIO];
    const bool check = bool( (nu >0.499 && nu<0.501 ) || (nu < -0.999 && nu > -1.01 ) );

    if(POISSON_RATIO.Key() == 0 || check==true)
    {
        KRATOS_ERROR << "POISSON_RATIO has Key zero invalid value " << std::endl;
    }


    if(DENSITY.Key() == 0 || rMaterialProperties[DENSITY] < 0.00)
    {
        KRATOS_ERROR << "DENSITY has Key zero or invalid value " << std::endl;
    }

    return 0;

}

} // Namespace Kratos
