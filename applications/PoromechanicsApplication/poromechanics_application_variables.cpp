
//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Ignasi de Pouplana
//


#include "poromechanics_application_variables.h"

namespace Kratos
{
//Create Variables
KRATOS_CREATE_VARIABLE( double, VELOCITY_COEFFICIENT )
KRATOS_CREATE_VARIABLE( double, DT_PRESSURE_COEFFICIENT )

KRATOS_CREATE_VARIABLE( double, DT_WATER_PRESSURE )
KRATOS_CREATE_VARIABLE( double, NORMAL_FLUID_FLUX )

KRATOS_CREATE_VARIABLE( double, DENSITY_SOLID )
KRATOS_CREATE_VARIABLE( double, BULK_MODULUS_SOLID )
KRATOS_CREATE_VARIABLE( double, BULK_MODULUS_FLUID )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_XX )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_YY )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_ZZ )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_XY )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_YZ )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_ZX )

KRATOS_CREATE_VARIABLE( double, MINIMUM_JOINT_WIDTH )
KRATOS_CREATE_VARIABLE( double, TRANSVERSAL_PERMEABILITY )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( FLUID_FLUX_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_FLUID_FLUX_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_STRESS_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_RELATIVE_DISPLACEMENT_VECTOR )
KRATOS_CREATE_VARIABLE( Matrix, PERMEABILITY_MATRIX )
KRATOS_CREATE_VARIABLE( Matrix, LOCAL_PERMEABILITY_MATRIX )

KRATOS_CREATE_VARIABLE( double, CRITICAL_DISPLACEMENT )

KRATOS_CREATE_VARIABLE(bool, IS_CONVERGED)

KRATOS_CREATE_VARIABLE( Matrix, TOTAL_STRESS_TENSOR )

KRATOS_CREATE_VARIABLE( Matrix, INITIAL_STRESS_TENSOR )

KRATOS_CREATE_VARIABLE( double, STATE_VARIABLE )
KRATOS_CREATE_VARIABLE( double, ARC_LENGTH_LAMBDA )
KRATOS_CREATE_VARIABLE( double, ARC_LENGTH_RADIUS_FACTOR )

KRATOS_CREATE_VARIABLE( double, TIME_UNIT_CONVERTER )

KRATOS_CREATE_VARIABLE( double, LOCAL_EQUIVALENT_STRAIN )
KRATOS_CREATE_VARIABLE( double, NONLOCAL_EQUIVALENT_STRAIN )

KRATOS_CREATE_VARIABLE( double, JOINT_WIDTH )

KRATOS_CREATE_VARIABLE( bool, NODAL_SMOOTHING )
KRATOS_CREATE_VARIABLE( Matrix, NODAL_CAUCHY_STRESS_TENSOR )
KRATOS_CREATE_VARIABLE( Matrix, EFFECTIVE_STRESS_TENSOR )
KRATOS_CREATE_VARIABLE( Matrix, NODAL_EFFECTIVE_STRESS_TENSOR )
KRATOS_CREATE_VARIABLE( double, NODAL_DAMAGE_VARIABLE )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_AREA )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_WIDTH )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_DAMAGE )

KRATOS_CREATE_VARIABLE( double, SHEAR_FRACTURE_ENERGY )

KRATOS_CREATE_VARIABLE( double, BIOT_COEFFICIENT )

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DAMPING_FORCE )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( FLUX_RESIDUAL )
KRATOS_CREATE_VARIABLE( double, THETA_1 )
KRATOS_CREATE_VARIABLE( double, THETA_3 )
KRATOS_CREATE_VARIABLE( double, DELTA )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_DISPLACEMENT_F )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_DISPLACEMENT_LF )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_DISPLACEMENT_P )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_VELOCITY_F )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_VELOCITY_LF )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_VELOCITY_P )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_ACCELERATION_F )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_ACCELERATION_LF )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( DELTA_ACCELERATION_P )
KRATOS_CREATE_VARIABLE( double, DELTA_WATER_PRESSURE_F )
KRATOS_CREATE_VARIABLE( double, DELTA_WATER_PRESSURE_LF )
KRATOS_CREATE_VARIABLE( double, DELTA_WATER_PRESSURE_P )
KRATOS_CREATE_VARIABLE( double, DELTA_DT_WATER_PRESSURE_F )
KRATOS_CREATE_VARIABLE( double, DELTA_DT_WATER_PRESSURE_LF )
KRATOS_CREATE_VARIABLE( double, DELTA_DT_WATER_PRESSURE_P )

}
