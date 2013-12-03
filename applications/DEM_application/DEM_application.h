//
//   Project Name:        Kratos
//   Last Modified by:    $Author:  $
//   Date:                $Date:  $
//   Revision:            $Revision: 1.2 $
//
//


#if !defined(KRATOS_DEM_APPLICATION_H_INCLUDED )
#define  KRATOS_DEM_APPLICATION_H_INCLUDED


// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"


#include "includes/variables.h"
#include "custom_elements/cylinder_particle.h"
#include "custom_elements/cylinder_continuum_particle.h"
#include "custom_elements/spheric_particle.h"
#include "custom_elements/spheric_continuum_particle.h"
#include "custom_elements/spheric_swimming_particle.h"
#include "custom_elements/DEM_FEM_Particle.h"
#include "custom_elements/Particle_Contact_Element.h"
#include "custom_conditions/RigidFace.h"
#include "custom_conditions/RigidEdge.h"


const long double pi = 3.141592653589793238462643383279;  //this one is used in the forward_euler_scheme... does it have to be here?

namespace Kratos
{

  KRATOS_DEFINE_VARIABLE(WeakPointerVector< Element >, CONTINUUM_INI_NEIGHBOUR_ELEMENTS)
  KRATOS_DEFINE_VARIABLE(WeakPointerVector< Element >, NODE_TO_NEIGH_ELEMENT_POINTER)
  
  // OPTIONS AND FLAGS
  
  KRATOS_DEFINE_VARIABLE(int, BOUNDING_BOX_OPTION)
  KRATOS_DEFINE_VARIABLE(int, ROTATION_OPTION)
  KRATOS_DEFINE_VARIABLE(int, CRITICAL_TIME_OPTION)
  KRATOS_DEFINE_VARIABLE(int, VIRTUAL_MASS_OPTION)
  KRATOS_DEFINE_VARIABLE(int, UNIFORM_MATERIAL_OPTION)
  KRATOS_DEFINE_VARIABLE(int, ACTIVATE_SEARCH)
  KRATOS_DEFINE_VARIABLE(vector<int>, ACTIVATE_SEARCH_VECTOR)
  KRATOS_DEFINE_VARIABLE(int, LIMIT_SURFACE_OPTION)
  KRATOS_DEFINE_VARIABLE(int, LIMIT_CYLINDER_OPTION)
  KRATOS_DEFINE_VARIABLE(int, CLEAN_INDENT_OPTION)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_VARIABLES_OPTION)
  KRATOS_DEFINE_VARIABLE(int, TRIHEDRON_OPTION)
  KRATOS_DEFINE_VARIABLE(int, FORCE_CALCULATION_TYPE)
  KRATOS_DEFINE_VARIABLE(int, DAMP_TYPE)
  KRATOS_DEFINE_VARIABLE(int, ROTA_DAMP_TYPE)
  KRATOS_DEFINE_VARIABLE(int, NEIGH_INITIALIZED)
  KRATOS_DEFINE_VARIABLE(int, GROUP_ID)
  KRATOS_DEFINE_VARIABLE(int, TRIAXIAL_TEST_OPTION)
  KRATOS_DEFINE_VARIABLE(int, FIX_VELOCITIES_FLAG)
  KRATOS_DEFINE_VARIABLE(int, STRESS_STRAIN_OPTION)
  KRATOS_DEFINE_VARIABLE(double,DUMMY_DEBUG_DOUBLE)
  KRATOS_DEFINE_VARIABLE(int, DUMMY_DEBUG_INT)
  KRATOS_DEFINE_VARIABLE(int, PARTICLE_ID)
  
    

  // *************** Continuum only BEGIN *************
  KRATOS_DEFINE_VARIABLE(bool, AREA_CALCULATED_FLAG)
  KRATOS_DEFINE_VARIABLE(Vector, DEM_AREA_VECTOR)
  KRATOS_DEFINE_VARIABLE(bool, DELTA_OPTION)
  KRATOS_DEFINE_VARIABLE(int, CASE_OPTION)
  KRATOS_DEFINE_VARIABLE(int, DEMPACK_OPTION)
  KRATOS_DEFINE_VARIABLE(double, AREA_FACTOR)
  KRATOS_DEFINE_VARIABLE(int, SKIN_SPHERE)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_COHESION)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_TENSION)
  
  KRATOS_DEFINE_VARIABLE(int, ROTATION_SPRING_OPTION)
  KRATOS_DEFINE_VARIABLE(int, CONTACT_MESH_OPTION)
  KRATOS_DEFINE_VARIABLE(int, FAILURE_CRITERION_OPTION)
  KRATOS_DEFINE_VARIABLE(int, CONCRETE_TEST_OPTION)
  KRATOS_DEFINE_VARIABLE(int, PARTICLE_CONTINUUM)
  KRATOS_DEFINE_VARIABLE(int, plot_OPTIONS)
  KRATOS_DEFINE_VARIABLE(double, DEMPACK_DAMPING)
  KRATOS_DEFINE_VARIABLE(double, DEMPACK_GLOBAL_DAMPING)
  KRATOS_DEFINE_VARIABLE(double, SURFACE_FRICTION_1)
  KRATOS_DEFINE_VARIABLE(double, SURFACE_FRICTION_2)
  KRATOS_DEFINE_VARIABLE(double, SURFACE_FRICTION_3)
  KRATOS_DEFINE_VARIABLE(double, SURFACE_FRICTION_4)
  KRATOS_DEFINE_VARIABLE(double, SURFACE_FRICTION_5)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_FRICTION_1)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_FRICTION_2)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_FRICTION_3)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_FRICTION_4)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_FRICTION_5)
  KRATOS_DEFINE_VARIABLE(int, IF_BOUNDARY_ELEMENT)
  KRATOS_DEFINE_VARIABLE(Vector, IF_BOUNDARY_FACE)
  KRATOS_DEFINE_VARIABLE(vector<int>, PARTICLE_CONTACT_FAILURE_ID)
  //KRATOS_DEFINE_VARIABLE(int, PARTICLE_FAILURE_ID)
  
  KRATOS_DEFINE_VARIABLE(double, PREDEFINED_SKIN)
  // *************** Continuum only END ***************

  // *************** Swimming only BEGIN *************
  KRATOS_DEFINE_VARIABLE(int, NON_NEWTONIAN_OPTION)
  KRATOS_DEFINE_VARIABLE(int, MANUALLY_IMPOSED_DRAG_LAW_OPTION)
  KRATOS_DEFINE_VARIABLE(int, DRAG_MODIFIER_TYPE)
  KRATOS_DEFINE_VARIABLE(int, BUOYANCY_FORCE_TYPE)
  KRATOS_DEFINE_VARIABLE(int, DRAG_FORCE_TYPE)
  KRATOS_DEFINE_VARIABLE(int, VIRTUAL_MASS_FORCE_TYPE)
  KRATOS_DEFINE_VARIABLE(int, LIFT_FORCE_TYPE)
  // *************** Swimming only END ***************

  // COUNTING

  KRATOS_DEFINE_VARIABLE(int, NUM_OF_NEIGH)
  KRATOS_DEFINE_VARIABLE(int, TOTAL_CONTACTS)
  KRATOS_DEFINE_VARIABLE(int, NUM_PARTICLES_INITIALIZED)
  KRATOS_DEFINE_VARIABLE(std::size_t, POTENTIAL_NEIGHBOURS)

  // TOLERANCES

  KRATOS_DEFINE_VARIABLE(double, DISTANCE_TOLERANCE)
  KRATOS_DEFINE_VARIABLE(double, SEARCH_TOLERANCE)
  

  // *************** Swimming only BEGIN *************
  KRATOS_DEFINE_VARIABLE(double, POWER_LAW_TOLERANCE)
  // *************** Swimming only END ***************

  // TIME RELATED

  KRATOS_DEFINE_VARIABLE(double, TIME_INCREASING_RATIO)
  KRATOS_DEFINE_VARIABLE(double, FINAL_SIMULATION_TIME)

  // MATERIAL PARAMETERS

  KRATOS_DEFINE_VARIABLE(double, NODAL_MASS_COEFF)
  KRATOS_DEFINE_VARIABLE(double, DEM_MAGIC_FACTOR)
  KRATOS_DEFINE_VARIABLE(double, DEM_MAGIC_FACTOR_POISSON)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_MOMENT_OF_INERTIA)
  KRATOS_DEFINE_VARIABLE(double, ROLLING_FRICTION)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_DENSITY)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_YOUNG_MODULUS)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_POISSON_RATIO)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_COHESION)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_ROLLING_FRICTION)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_STATIC_FRICTION)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_DYNAMIC_FRICTION)
  KRATOS_DEFINE_VARIABLE(double, GENERAL_RESTITUTION_COEFF)
  KRATOS_DEFINE_VARIABLE(double, HISTORICAL_MIN_K)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_KN)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_KT)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_INERTIA)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_DENSITY)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_FRICTION)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_STATIC_FRICTION_COEF)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_DYNAMIC_FRICTION_COEF)
  KRATOS_DEFINE_VARIABLE(double, RESTITUTION_COEFF)
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_ROTATION_DAMP_RATIO)
  KRATOS_DEFINE_VARIABLE(double, SQRT_OF_MASS)
  KRATOS_DEFINE_VARIABLE(double, LN_OF_RESTITUTION_COEFF)

  // *************** Continuum only BEGIN *************
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_KR)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_RN)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_RT)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_RR)
  KRATOS_DEFINE_VARIABLE(double, GLOBAL_FRI_ANG)

  KRATOS_DEFINE_VARIABLE(double, NON_ELASTIC_STAGE)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_FRACTION_N1)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_FRACTION_N2)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_FRACTION_N3)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_LIMIT_COEFF_C1)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_LIMIT_COEFF_C2)
  KRATOS_DEFINE_VARIABLE(double, SLOPE_LIMIT_COEFF_C3)
  KRATOS_DEFINE_VARIABLE(double, YOUNG_MODULUS_PLASTIC)
  KRATOS_DEFINE_VARIABLE(double, PLASTIC_YIELD_STRESS)
  KRATOS_DEFINE_VARIABLE(double, DAMAGE_FACTOR)
  KRATOS_DEFINE_VARIABLE(double, DONZE_G1)
  KRATOS_DEFINE_VARIABLE(double, DONZE_G2)
  KRATOS_DEFINE_VARIABLE(double, DONZE_G3)
  KRATOS_DEFINE_VARIABLE(double, DONZE_MAX_DEF)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_FAILURE)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_SIGMA)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_TAU)
  KRATOS_DEFINE_VARIABLE(double, FAILURE_CRITERION_STATE)
  KRATOS_DEFINE_VARIABLE(double, UNIDIMENSIONAL_DAMAGE)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_SIGMA_MAX)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_SIGMA_MIN)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_TAU_ZERO)
  KRATOS_DEFINE_VARIABLE(double, CONTACT_INTERNAL_FRICC)
  KRATOS_DEFINE_VARIABLE(double, LOW_POISSON_FORCE)
  KRATOS_DEFINE_VARIABLE(double, HIGH_POISSON_FORCE)

  // *************** Continuum only END *************

  // *************** Swimming only BEGIN *************
  KRATOS_DEFINE_VARIABLE(double, GEL_STRENGTH)
  KRATOS_DEFINE_VARIABLE(double, POWER_LAW_N)
  KRATOS_DEFINE_VARIABLE(double, POWER_LAW_K)
  KRATOS_DEFINE_VARIABLE(double, INIT_DRAG_FORCE)
  KRATOS_DEFINE_VARIABLE(double, DRAG_LAW_SLOPE)
  // *************** Swimming only END ***************

  // GEOMETRIC PARAMETERS

  // *************** Swimming only BEGIN *************
  KRATOS_DEFINE_VARIABLE(double, PARTICLE_SPHERICITY)
  // *************** Swimming only END ***************

  KRATOS_DEFINE_VARIABLE(double, MAX_INDENTATION)

  // *************** Continuum only BEGIN *************
  KRATOS_DEFINE_VARIABLE(double, LOCAL_CONTACT_AREA_HIGH)
  KRATOS_DEFINE_VARIABLE(double, LOCAL_CONTACT_AREA_LOW)
  KRATOS_DEFINE_VARIABLE(double, MEAN_CONTACT_AREA)
  KRATOS_DEFINE_VARIABLE(double, REPRESENTATIVE_VOLUME)
  KRATOS_DEFINE_VARIABLE(Vector, PARTICLE_INITIAL_DELTA)
  KRATOS_DEFINE_VARIABLE(Vector, PARTICLE_CONTACT_DELTA)

  KRATOS_DEFINE_VARIABLE(VectorArray3Double, ARROW_POINT)
  // *************** Continuum only END ***************
  
  
  // INLET PARAMETERS
    
  KRATOS_DEFINE_VARIABLE(double,INLET_START_TIME)    
  KRATOS_DEFINE_VARIABLE(double,INLET_STOP_TIME)
  KRATOS_DEFINE_VARIABLE(double,INLET_NUMBER_OF_PARTICLES)
    

  // KINEMATICS

  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_ROTATION_ANGLE)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(EULER_ANGLES)
  KRATOS_DEFINE_VARIABLE(double, ORIENTATION_REAL)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ORIENTATION_IMAG)
  KRATOS_DEFINE_VARIABLE(Vector, INITIAL_AXES_TRACKING)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_5)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_5)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_5)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_5)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_RADIUS_1)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_RADIUS_2)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_RADIUS_3)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_RADIUS_4)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_RADIUS_5)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_VELOCITY_1)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_VELOCITY_2)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_VELOCITY_3)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_VELOCITY_4)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_VELOCITY_5)
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_1)  
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_2) 
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_3) 
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_4) 
  KRATOS_DEFINE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_5) 
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DELTA_DISPLACEMENT)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTA_DISPLACEMENT)

  // FORCE AND MOMENTUM

  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(TOTAL_FORCES)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_5)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_1)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_2)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_3)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_4)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_5)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_MOMENT)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(MAX_ROTA_MOMENT)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_FORCES)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)

  // ENERGY

   KRATOS_DEFINE_VARIABLE(double, KINETIC_ENERGY)
   KRATOS_DEFINE_VARIABLE(double, ELASTIC_ENERGY_OF_CONTACTS)

  // *************** Continuum only BEGIN *************
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
  KRATOS_DEFINE_VARIABLE(Vector, PARTICLE_BLOCK_CONTACT_FORCE)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE)
  KRATOS_DEFINE_VARIABLE(VectorArray3Double, PARTICLE_CONTACT_FORCES )

  // CONCRETE TEST

  KRATOS_DEFINE_VARIABLE(double, FIXED_VEL_TOP)
  KRATOS_DEFINE_VARIABLE(double, FIXED_VEL_BOT)
  KRATOS_DEFINE_VARIABLE(int, STEP_FIX_VELOCITIES)
  KRATOS_DEFINE_VARIABLE(int, FIX_HORIZONTAL_VEL)
  KRATOS_DEFINE_VARIABLE(double, RADIAL_DISPLACEMENT)
  KRATOS_DEFINE_VARIABLE(double, AREA_VERTICAL_TAPA)
  KRATOS_DEFINE_VARIABLE(double, AREA_VERTICAL_CENTRE)

  // TENSION

  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_XX)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_XY)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_XZ)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_YX)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_YY)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_YZ)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_ZX)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_ZY)
  KRATOS_DEFINE_VARIABLE(double, DEM_STRESS_ZZ)
  // *************** Continuum only END *************

  // Possible future blocks (no FEM) interaction

  KRATOS_DEFINE_VARIABLE(Vector, PARTICLE_BLOCK_CONTACT_FAILURE_ID)
  KRATOS_DEFINE_VARIABLE(Vector, PARTICLE_BLOCK_IF_INITIAL_CONTACT)
  KRATOS_DEFINE_VARIABLE(WeakPointerVector<Element >, NEIGHBOUR_PARTICLE_BLOCK_ELEMENTS)
  
  KRATOS_DEFINE_VARIABLE(WeakPointerVector<Condition >,     NEIGHBOUR_RIGID_FACES)
  
  KRATOS_DEFINE_VARIABLE(WeakPointerVector<Element >, NEIGHBOUR_PARTICLE_OF_RIGID_FACE)
  
  KRATOS_DEFINE_VARIABLE(Vector,   NEIGHBOUR_RIGID_FACES_PRAM)
  
  KRATOS_DEFINE_VARIABLE(Vector,   NEIGHBOUR_RIGID_FACES_CONTACT_FORCE)

  // DUMMIES INT AND DOUBLE VARIABLES

  KRATOS_DEFINE_VARIABLE(int, DUMMY_SWITCH)
  KRATOS_DEFINE_VARIABLE(double, DUMMY_FORCES)
  KRATOS_DEFINE_VARIABLE(double, DUMMY_LOCAL_AXES)
  
  // EXPORTS
  
  KRATOS_DEFINE_VARIABLE(double, EXPORT_GROUP_ID)
  KRATOS_DEFINE_VARIABLE(double, EXPORT_ID)
  KRATOS_DEFINE_VARIABLE(double, EXPORT_PARTICLE_FAILURE_ID)
  KRATOS_DEFINE_VARIABLE(double, EXPORT_SKIN_SPHERE)
  KRATOS_DEFINE_VARIABLE(int, PRINT_EXPORT_ID)
  KRATOS_DEFINE_VARIABLE(int, PRINT_SKIN_SPHERE)
  KRATOS_DEFINE_VARIABLE(int, PRINT_GROUP_ID)
  KRATOS_DEFINE_VARIABLE(int, PRINT_RADIAL_DISPLACEMENT)
  
    // For DEM_FEM Element
  KRATOS_DEFINE_VARIABLE(double, LOCAL_DAMP_RATIO)

  // DUMMY VARIABLES FOR CALCULATE
  KRATOS_DEFINE_VARIABLE(double, CALCULATE_COMPUTE_NEW_NEIGHBOURS_HISTORICAL_DATA)
  KRATOS_DEFINE_VARIABLE(double, CALCULATE_COMPUTE_NEW_RIGID_FACE_NEIGHBOURS_HISTORICAL_DATA)
  KRATOS_DEFINE_VARIABLE(double, CALCULATE_SET_INITIAL_CONTACTS)

  
  ///Cfeng,131013,RigidFace Movement
  KRATOS_DEFINE_VARIABLE(double, RIGID_FACE_ROTA_SPEED)
  KRATOS_DEFINE_VARIABLE(double, RIGID_FACE_AXIAL_SPEED)
  KRATOS_DEFINE_VARIABLE(int,    RIGID_FACE_PROP_ID)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_ORIGIN_COORD)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_AXIAL_DIR)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_GLOBAL_VELOCITY)
  KRATOS_DEFINE_VARIABLE(double, RIGID_FACE_BEGIN_TIME)
  KRATOS_DEFINE_VARIABLE(double, RIGID_FACE_END_TIME)
  KRATOS_DEFINE_VARIABLE(int, RIGID_FACE_FLAG)
  KRATOS_DEFINE_VARIABLE(Vector, RIGID_FACE_COMPUTE_MOVEMENT)
  
  //OPTIMIZATION
  KRATOS_DEFINE_VARIABLE(int,    VELOCITY_X_DOF_POS)
  KRATOS_DEFINE_VARIABLE(int,    VELOCITY_Y_DOF_POS)
  KRATOS_DEFINE_VARIABLE(int,    VELOCITY_Z_DOF_POS)
  KRATOS_DEFINE_VARIABLE(int,    ANGULAR_VELOCITY_X_DOF_POS)
  KRATOS_DEFINE_VARIABLE(int,    ANGULAR_VELOCITY_Y_DOF_POS)
  KRATOS_DEFINE_VARIABLE(int,    ANGULAR_VELOCITY_Z_DOF_POS)
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(OLD_COORDINATES)
  
class KratosDEMApplication : public KratosApplication
{
public:
    ///@name Type Definitions
    ///@{


    /// Pointer definition of KratosDEMSpheresApplication
    KRATOS_CLASS_POINTER_DEFINITION(KratosDEMApplication);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    KratosDEMApplication();

    /// Destructor.
    virtual ~KratosDEMApplication() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    virtual void Register();



    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        return "KratosDEMApplication";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
        PrintData(rOStream);
    }

    ///// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        //KRATOS_WATCH("in KratosDEMApplication");
        //KRATOS_WATCH(KratosComponents<VariableData>::GetComponents().size() );
        rOStream << "Variables:" << std::endl;
        KratosComponents<VariableData>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Elements:" << std::endl;
        KratosComponents<Element>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Conditions:" << std::endl;
        KratosComponents<Condition>().PrintData(rOStream);
    }


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{
    const CylinderParticle mCylinderParticle2D;
    const CylinderContinuumParticle mCylinderContinuumParticle2D;
    const SphericParticle mSphericParticle3D;
    const SphericContinuumParticle mSphericContinuumParticle3D;
    const SphericSwimmingParticle mSphericSwimmingParticle3D;

    //const DEM_FEM_Particle mDEM_FEM_Particle2D;
    const DEM_FEM_Particle mDEM_FEM_Particle3D;
    
    const Particle_Contact_Element mParticleContactElement;
    const VariablesList mVariablesList;
	
	
	const RigidFace3D  mRigidFace3D3N;
    const RigidFace3D  mRigidFace3D4N;
	const RigidEdge3D  mRigidEdge3D2N;

    //       static const ApplicationCondition  msApplicationCondition;

    ///@}
    ///@name Member Variables
    ///@{
//      const Elem2D   mElem2D;
//      const Elem3D   mElem3D;


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    KratosDEMApplication& operator=(KratosDEMApplication const& rOther);

    /// Copy constructor.
    KratosDEMApplication(KratosDEMApplication const& rOther);


    ///@}

}; // Class KratosDEMApplication

///@}


///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif // KRATOS_DEM_APPLICATION_H_INCLUDED  defined 


