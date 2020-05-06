//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya (https://github.com/sunethwarna)
//

// System includes

// Project includes
#include "includes/cfd_variables.h"
#include "includes/checks.h"
#include "includes/variables.h"

// Application includes
#include "custom_elements/element_data/evm_k_epsilon/evm_k_epsilon_element_data_utilities.h"
#include "custom_utilities/rans_calculation_utilities.h"
#include "evm_k_omega_sst_element_data_utilities.h"
#include "rans_application_variables.h"

// Include base h
#include "evm_k_omega_sst_k_element_data.h"

namespace Kratos
{
namespace EvmKOmegaSSTElementDataUtilities
{
template <unsigned int TDim>
const Variable<double>& KElementData<TDim>::GetScalarVariable()
{
    return TURBULENT_KINETIC_ENERGY;
}

template <unsigned int TDim>
const Variable<double>& KElementData<TDim>::GetScalarRateVariable()
{
    return TURBULENT_KINETIC_ENERGY_RATE;
}

template <unsigned int TDim>
const Variable<double>& KElementData<TDim>::GetScalarRelaxedRateVariable()
{
    return RANS_AUXILIARY_VARIABLE_1;
}

template <unsigned int TDim>
void KElementData<TDim>::Check(const GeometryType& rGeometry, const ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY
    const int number_of_nodes = rGeometry.PointsNumber();

    for (int i_node = 0; i_node < number_of_nodes; ++i_node)
    {
        const NodeType& r_node = rGeometry[i_node];
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(VELOCITY, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(KINEMATIC_VISCOSITY, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(TURBULENT_VISCOSITY, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(TURBULENT_KINETIC_ENERGY, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(TURBULENT_KINETIC_ENERGY_RATE, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(
            TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(RANS_AUXILIARY_VARIABLE_1, r_node);

        KRATOS_CHECK_DOF_IN_NODE(TURBULENT_KINETIC_ENERGY, r_node);
    }

    KRATOS_CATCH("");
}
template <unsigned int TDim>
GeometryData::IntegrationMethod KElementData<TDim>::GetIntegrationMethod()
{
    return GeometryData::GI_GAUSS_2;
}

template <unsigned int TDim>
void KElementData<TDim>::CalculateConstants(const ProcessInfo& rCurrentProcessInfo)
{
    mA1 = rCurrentProcessInfo[TURBULENCE_RANS_A1];
    mSigmaK1 = rCurrentProcessInfo[TURBULENT_KINETIC_ENERGY_SIGMA_1];
    mSigmaK2 = rCurrentProcessInfo[TURBULENT_KINETIC_ENERGY_SIGMA_2];
    mSigmaOmega2 = rCurrentProcessInfo[TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE_SIGMA_2];
    mBetaStar = rCurrentProcessInfo[TURBULENCE_RANS_C_MU];
}

template <unsigned int TDim>
void KElementData<TDim>::CalculateGaussPointData(const Vector& rShapeFunctions,
                                                 const Matrix& rShapeFunctionDerivatives,
                                                 const int Step)
{
    KRATOS_TRY

    using namespace RansCalculationUtilities;

    const GeometryType& r_geometry = this->GetGeometry();

    mTurbulentKineticEnergy = std::max(
        EvaluateInPoint(r_geometry, TURBULENT_KINETIC_ENERGY, rShapeFunctions), 1e-12);
    mTurbulentSpecificEnergyDissipationRate = std::max(
        EvaluateInPoint(r_geometry, TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE, rShapeFunctions),
        1e-12);
    mKinematicViscosity = EvaluateInPoint(r_geometry, KINEMATIC_VISCOSITY, rShapeFunctions);
    mWallDistance = EvaluateInPoint(r_geometry, DISTANCE, rShapeFunctions);

    CalculateGradient(mTurbulentKineticEnergyGradient, r_geometry,
                      TURBULENT_KINETIC_ENERGY, rShapeFunctionDerivatives, Step);

    CalculateGradient(mTurbulentSpecificEnergyDissipationRateGradient,
                      r_geometry, TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE,
                      rShapeFunctionDerivatives, Step);

    mCrossDiffusion = EvmKOmegaSSTElementDataUtilities::CalculateCrossDiffusionTerm(
        mSigmaOmega2, mTurbulentSpecificEnergyDissipationRate,
        mTurbulentKineticEnergyGradient, mTurbulentSpecificEnergyDissipationRateGradient);

    const double f_1 = EvmKOmegaSSTElementDataUtilities::CalculateF1(
        mTurbulentKineticEnergy, mTurbulentSpecificEnergyDissipationRate,
        mKinematicViscosity, mWallDistance, mBetaStar, mCrossDiffusion, mSigmaOmega2);

    const double f_2 = EvmKOmegaSSTElementDataUtilities::CalculateF2(
        mTurbulentKineticEnergy, mTurbulentSpecificEnergyDissipationRate,
        mKinematicViscosity, mWallDistance, mBetaStar);

    mBlendedSimgaK =
        EvmKOmegaSSTElementDataUtilities::CalculateBlendedPhi(mSigmaK1, mSigmaK2, f_1);

    mVelocityDivergence = GetDivergence(r_geometry, VELOCITY, rShapeFunctionDerivatives);

    CalculateGradient<TDim>(mVelocityGradient, r_geometry, VELOCITY,
                            rShapeFunctionDerivatives, Step);

    const array_1d<double, 3>& r_vorticity =
        EvmKOmegaSSTElementDataUtilities::CalculateVorticity<TDim>(mVelocityGradient);
    const double vorticity_norm = norm_2(r_vorticity);

    mTurbulentKinematicViscosity =
        EvmKOmegaSSTElementDataUtilities::CalculateTurbulentKinematicViscosity(
            mTurbulentKineticEnergy, mTurbulentSpecificEnergyDissipationRate,
            vorticity_norm, f_2, mA1);

    KRATOS_CATCH("");
}

template <unsigned int TDim>
array_1d<double, 3> KElementData<TDim>::CalculateEffectiveVelocity(
    const Vector& rShapeFunctions, const Matrix& rShapeFunctionDerivatives) const
{
    return RansCalculationUtilities::EvaluateInPoint(this->GetGeometry(),
                                                     VELOCITY, rShapeFunctions);
}

template <unsigned int TDim>
double KElementData<TDim>::CalculateEffectiveKinematicViscosity(
    const Vector& rShapeFunctions, const Matrix& rShapeFunctionDerivatives) const
{
    return mKinematicViscosity + mBlendedSimgaK * mTurbulentKinematicViscosity;
}

template <unsigned int TDim>
double KElementData<TDim>::CalculateReactionTerm(const Vector& rShapeFunctions,
                                                 const Matrix& rShapeFunctionDerivatives) const
{
    return std::max(mBetaStar * mTurbulentKineticEnergy / mTurbulentKinematicViscosity +
                        (2.0 / 3.0) * mVelocityDivergence,
                    0.0);
}

template <unsigned int TDim>
double KElementData<TDim>::CalculateSourceTerm(const Vector& rShapeFunctions,
                                               const Matrix& rShapeFunctionDerivatives) const
{
    return EvmKEpsilonElementDataUtilities::CalculateSourceTerm<TDim>(
        mVelocityGradient, mTurbulentKinematicViscosity);
}

// template instantiations
template class KElementData<2>;
template class KElementData<3>;

} // namespace EvmKOmegaSSTElementDataUtilities

} // namespace Kratos