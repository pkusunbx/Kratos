/*
//  KRATOS  _____________
//         /  _/ ____/   |
//         / // / __/ /| |
//       _/ // /_/ / ___ |
//      /___/\____/_/  |_| Application
//
//  Main authors:   Thomas Oberbichler
*/

#if !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)
#define  KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/variables.h"

namespace Kratos
{

KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, CROSS_AREA)
KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, PRESTRESS_CAUCHY)

KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, RAYLEIGH_ALPHA)
KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, RAYLEIGH_BETA)

//Load Condition Variables
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, POINT_LOAD)
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, LINE_LOAD)
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, SURFACE_LOAD)

KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, DEAD_LOAD)

//Stress Recovery Variables
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, PK2_STRESS)
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, CAUCHY_STRESS)
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, CAUCHY_STRESS_TOP)
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, CAUCHY_STRESS_BOTTOM)
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, MEMBRANE_FORCE)
KRATOS_DEFINE_SYMMETRIC_2D_TENSOR_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, INTERNAL_MOMENT)

KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, SHEAR_FORCE_1)
KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, SHEAR_FORCE_2)

//Penalty Variables
KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, PENALTY_FACTOR)

// MPM VARIABLES
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(IGA_APPLICATION, MPM_LOCATION)
KRATOS_DEFINE_APPLICATION_VARIABLE(IGA_APPLICATION, double, MPM_WEIGHT)

} // namespace Kratos

#endif // !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)