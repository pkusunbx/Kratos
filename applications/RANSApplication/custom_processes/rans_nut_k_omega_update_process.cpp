//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya
//

// System includes
#include <cmath>
#include <limits>

// External includes

// Project includes
#include "includes/cfd_variables.h"
#include "includes/define.h"
#include "utilities/parallel_utilities.h"

// Application includes
#include "custom_utilities/fluid_calculation_utilities.h"
#include "custom_utilities/rans_calculation_utilities.h"
#include "rans_application_variables.h"

// Include base h
#include "rans_nut_k_omega_update_process.h"

namespace Kratos
{
RansNutKOmegaUpdateProcess::RansNutKOmegaUpdateProcess(
    Model& rModel,
    Parameters rParameters)
: mrModel(rModel)
{
    KRATOS_TRY

    rParameters.ValidateAndAssignDefaults(GetDefaultParameters());

    mEchoLevel = rParameters["echo_level"].GetInt();
    mModelPartName = rParameters["model_part_name"].GetString();
    mMinValue = rParameters["min_value"].GetDouble();

    KRATOS_CATCH("");
}

RansNutKOmegaUpdateProcess::RansNutKOmegaUpdateProcess(
    Model& rModel,
    const std::string& rModelPartName,
    const double MinValue,
    const int EchoLevel)
: mrModel(rModel),
  mModelPartName(rModelPartName),
  mMinValue(MinValue),
  mEchoLevel(EchoLevel)
{
}

int RansNutKOmegaUpdateProcess::Check()
{
    KRATOS_TRY

    const auto& r_model_part = mrModel.GetModelPart(mModelPartName);

    KRATOS_ERROR_IF(!r_model_part.HasNodalSolutionStepVariable(TURBULENT_KINETIC_ENERGY))
        << "TURBULENT_KINETIC_ENERGY is not found in nodal solution step variables list of "
        << mModelPartName << ".";

    KRATOS_ERROR_IF(!r_model_part.HasNodalSolutionStepVariable(TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE))
        << "TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE is not found in nodal solution step variables list of "
        << mModelPartName << ".";

    return 0;

    KRATOS_CATCH("");
}

void RansNutKOmegaUpdateProcess::ExecuteInitializeSolutionStep()
{
    KRATOS_TRY

    if (!mIsInitialized) {
        this->ExecuteAfterCouplingSolveStep();
        mIsInitialized = true;
    }

    KRATOS_CATCH("");
}

void RansNutKOmegaUpdateProcess::ExecuteAfterCouplingSolveStep()
{
    KRATOS_TRY

    using IndexType = std::size_t;

    using NodeType = Node<3>;

    using GeometryType = Geometry<NodeType>;

    using ShapeFunctionDerivativesArrayType = GeometryType::ShapeFunctionsGradientsType;

    using ElementType = ModelPart::ElementType;

    using tls_type = std::tuple<Vector, Matrix, ShapeFunctionDerivativesArrayType>;

    auto& r_model_part = mrModel.GetModelPart(mModelPartName);

    // setting element nu_t values
    block_for_each(r_model_part.Elements(), tls_type(), [&](ElementType& rElement, tls_type& rTLS) {
        auto& Ws = std::get<0>(rTLS);
        auto& Ns = std::get<1>(rTLS);
        auto& dNdXs = std::get<2>(rTLS);

        const auto& r_integration_method = rElement.GetIntegrationMethod();
        double nu_t = 0.0;

        RansCalculationUtilities::CalculateGeometryData(
            rElement.GetGeometry(), r_integration_method, Ws, Ns, dNdXs);

        for (IndexType g = 0; g < Ws.size(); ++g) {
            const Vector& N = row(Ns, g);

            double tke, omega;
            FluidCalculationUtilities::EvaluateInPoint(rElement.GetGeometry(), N,
                std::tie(tke, TURBULENT_KINETIC_ENERGY),
                std::tie(omega, TURBULENT_SPECIFIC_ENERGY_DISSIPATION_RATE));

            if (tke > 0.0 && omega > 0.0) {
                nu_t += tke / omega;
            }
        }

        nu_t = std::max(nu_t / Ws.size(), mMinValue);
        rElement.SetValue(TURBULENT_VISCOSITY, nu_t);
    });

    // setting wall nu_t values
    RansCalculationUtilities::CalculateWallTurbulentViscosity(r_model_part, mMinValue);

    KRATOS_INFO_IF(this->Info(), mEchoLevel > 1)
        << "Calculated nu_t for nodes in " << mModelPartName << ".\n";

    KRATOS_CATCH("");
}

std::string RansNutKOmegaUpdateProcess::Info() const
{
    return std::string("RansNutKOmegaUpdateProcess");
}

void RansNutKOmegaUpdateProcess::PrintInfo(std::ostream& rOStream) const
{
    rOStream << this->Info();
}

void RansNutKOmegaUpdateProcess::PrintData(std::ostream& rOStream) const
{
}

const Parameters RansNutKOmegaUpdateProcess::GetDefaultParameters() const
{
    const auto default_parameters = Parameters(R"(
        {
            "model_part_name" : "PLEASE_SPECIFY_MODEL_PART_NAME",
            "echo_level"      : 0,
            "min_value"       : 1e-15
        })");
    return default_parameters;
}

} // namespace Kratos.
