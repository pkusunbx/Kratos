// KRATOS___
//     //   ) )
//    //         ___      ___
//   //  ____  //___) ) //   ) )
//  //    / / //       //   / /
// ((____/ / ((____   ((___/ /  MECHANICS
//
//  License:         geo_mechanics_application/license.txt
//
//  Main authors:    Vahid Galavi
//

// External includes

// Project includes
#include "custom_elements/updated_lagrangian_U_Pw_diff_order_element.hpp"
#include "utilities/math_utils.h"

namespace Kratos
{

//----------------------------------------------------------------------------------------
Element::Pointer UpdatedLagrangianUPwDiffOrderElement::
    Create(IndexType NewId,
           NodesArrayType const& ThisNodes,
           PropertiesType::Pointer pProperties) const
{
    return Element::Pointer( new UpdatedLagrangianUPwDiffOrderElement( NewId, this->GetGeometry().Create( ThisNodes ), pProperties ) );
}

//----------------------------------------------------------------------------------------
Element::Pointer UpdatedLagrangianUPwDiffOrderElement::
    Create(IndexType NewId,
           GeometryType::Pointer pGeom,
           PropertiesType::Pointer pProperties) const
{
    return Element::Pointer( new UpdatedLagrangianUPwDiffOrderElement( NewId, pGeom, pProperties ) );
}


//----------------------------------------------------------------------------------------
int UpdatedLagrangianUPwDiffOrderElement::
    Check( const ProcessInfo& rCurrentProcessInfo ) const
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::Check()") << std::endl;

    // Base class checks for positive area and Id > 0
    // Verify generic variables
    int ierr = SmallStrainUPwDiffOrderElement::Check(rCurrentProcessInfo);

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::Check()") << ierr << std::endl;

    return ierr;

    KRATOS_CATCH( "" );
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    Initialize(const ProcessInfo &rCurrentProcessInfo)
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::Initialize()") << std::endl;

    SmallStrainUPwDiffOrderElement::Initialize(rCurrentProcessInfo);

    const GeometryType::IntegrationPointsArrayType &IntegrationPoints =
        GetGeometry().IntegrationPoints(this->GetIntegrationMethod());

    const SizeType Dim = GetGeometry().WorkingSpaceDimension();

    const SizeType NumGPoints = IntegrationPoints.size();

    if ( mDetF0.size() != NumGPoints)
        mDetF0.resize( NumGPoints );

    if ( mF0.size() != NumGPoints)
        mF0.resize( NumGPoints );

    for (IndexType GPoint = 0; GPoint < IntegrationPoints.size(); ++GPoint)
    {
        mDetF0[GPoint] = 1.0;
        mF0[GPoint] = IdentityMatrix(Dim);
    }

    mF0Computed = false;

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::Initialize()") << std::endl;

    KRATOS_CATCH( "" );
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    InitializeSolutionStep(const ProcessInfo &rCurrentProcessInfo)
{
    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::InitializeSolutionStep()") << std::endl;

    SmallStrainUPwDiffOrderElement::InitializeSolutionStep(rCurrentProcessInfo);

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::InitializeSolutionStep()") << std::endl;

    mF0Computed = false;
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    FinalizeSolutionStep(const ProcessInfo& rCurrentProcessInfo )
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::FinalizeSolutionStep()") << std::endl;

    //Definition of variables
    ElementVariables Variables;
    SmallStrainUPwDiffOrderElement::InitializeElementVariables( Variables, rCurrentProcessInfo );

    //Constitutive Law parameters
    ConstitutiveLaw::Parameters ConstitutiveParameters(this->GetGeometry(),
                                                       this->GetProperties(),
                                                       rCurrentProcessInfo);
    //ConstitutiveParameters.Set(ConstitutiveLaw::COMPUTE_STRESS);
    ConstitutiveParameters.Set(ConstitutiveLaw::USE_ELEMENT_PROVIDED_STRAIN);

    // Reading integration points
    for ( IndexType GPoint = 0; GPoint < mConstitutiveLawVector.size(); ++GPoint ) {
        // Compute element kinematics B, F, DNu_DX ...
        this->CalculateKinematics(Variables, GPoint);

        //set gauss points variables to constitutivelaw parameters
        this->SetElementalVariables(Variables, ConstitutiveParameters);

        // Call the constitutive law to update material variables
        //Compute constitutive tensor and stresses
        UpdateElementalVariableStressVector(Variables, GPoint);
        mConstitutiveLawVector[GPoint]->FinalizeMaterialResponseCauchy(ConstitutiveParameters);
        mStateVariablesFinalized[GPoint] = 
            mConstitutiveLawVector[GPoint]->GetValue( STATE_VARIABLES,
                                                      mStateVariablesFinalized[GPoint] );

        // Update the element internal variables
        this->UpdateHistoricalDatabase(Variables, GPoint);
    }

    //Assign pressure values to the intermediate nodes for post-processing
    AssignPressureToIntermediateNodes();

    mF0Computed = true;

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::FinalizeSolutionStep()") << std::endl;

    KRATOS_CATCH( "" );

}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    UpdateHistoricalDatabase(ElementVariables& rVariables,
                             const SizeType GPoint)
{
    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::UpdateHistoricalDatabase()") << std::endl;

    mDetF0[GPoint] = rVariables.detF;
    noalias(mF0[GPoint]) = rVariables.F;

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::UpdateHistoricalDatabase()") << std::endl;

}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    CalculateAll( MatrixType& rLeftHandSideMatrix,
                  VectorType& rRightHandSideVector,
                  const ProcessInfo& rCurrentProcessInfo,
                  const bool CalculateStiffnessMatrixFlag,
                  const bool CalculateResidualVectorFlag )
{
    KRATOS_TRY;

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateAll()") << std::endl;

    const GeometryType& rGeom = this->GetGeometry();

    //Definition of variables
    ElementVariables Variables;
    this->InitializeElementVariables(Variables,rCurrentProcessInfo);

    //Create constitutive law parameters:
    ConstitutiveLaw::Parameters ConstitutiveParameters( rGeom,
                                                        this->GetProperties(),
                                                        rCurrentProcessInfo );
    ConstitutiveParameters.GetOptions().Set(ConstitutiveLaw::USE_ELEMENT_PROVIDED_STRAIN);

    if (CalculateStiffnessMatrixFlag) ConstitutiveParameters.GetOptions().Set(ConstitutiveLaw::COMPUTE_CONSTITUTIVE_TENSOR);
    if (CalculateResidualVectorFlag)  ConstitutiveParameters.GetOptions().Set(ConstitutiveLaw::COMPUTE_STRESS);

    //Loop over integration points
    const GeometryType::IntegrationPointsArrayType
        &IntegrationPoints = rGeom.IntegrationPoints( this->GetIntegrationMethod() );

    // Computing in all integrations points
    for ( IndexType GPoint = 0; GPoint < IntegrationPoints.size(); ++GPoint )
    {

        // Compute element kinematics B, F, DNu_DX ...
        this->CalculateKinematics(Variables, GPoint);

        //set gauss points variables to constitutivelaw parameters
        this->SetElementalVariables(Variables,ConstitutiveParameters);

        // Cauchy strain: This needs to be investigated which strain measure should be used
        // In some references, e.g. Bathe, suggested to use Almansi strain measure
        noalias(Variables.StrainVector) = prod(Variables.B, Variables.DisplacementVector);

        //Compute constitutive tensor and stresses
        UpdateElementalVariableStressVector(Variables, GPoint);
        mConstitutiveLawVector[GPoint]->CalculateMaterialResponseCauchy(ConstitutiveParameters);
        UpdateStressVector(Variables, GPoint);

        // calculate Bulk modulus from stiffness matrix
        const double BulkModulus = CalculateBulkModulus(Variables.ConstitutiveMatrix);
        this->InitializeBiotCoefficients(Variables, BulkModulus);

        // Calculating weights for integration on the reference configuration
        this->CalculateIntegrationCoefficient( Variables.IntegrationCoefficient,
                                               Variables.detJ0,
                                               IntegrationPoints[GPoint].Weight() );

        if ( CalculateStiffnessMatrixFlag == true )
        {
            // Contributions to stiffness matrix calculated on the reference config
            /* Material stiffness matrix */
            this->CalculateAndAddLHS(rLeftHandSideMatrix, Variables);

            /* Geometric stiffness matrix */
            this->CalculateAndAddGeometricStiffnessMatrix( rLeftHandSideMatrix, Variables );
        }

        if (CalculateResidualVectorFlag)
        {
            //Contributions to the right hand side
            this->CalculateAndAddRHS(rRightHandSideVector, Variables);
        }
    }

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateAll()") << std::endl;

    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    CalculateAndAddGeometricStiffnessMatrix( MatrixType& rLeftHandSideMatrix,
                                             ElementVariables& rVariables )
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateAndAddGeometricStiffnessMatrix()") << std::endl;

    const GeometryType& rGeom = GetGeometry();
    const SizeType NumUNodes = rGeom.PointsNumber();
    const SizeType Dim = rGeom.WorkingSpaceDimension();

    Matrix StressTensor = MathUtils<double>::StressVectorToTensor( rVariables.StressVector );
    Matrix ReducedKgMatrix = prod( rVariables.DNu_DX,
                                   rVariables.IntegrationCoefficient *
                                   Matrix( prod( StressTensor, trans(rVariables.DNu_DX) ) ) ); //to be optimized

    Matrix UMatrix(NumUNodes*Dim, NumUNodes*Dim);
    noalias(UMatrix) = ZeroMatrix(NumUNodes*Dim, NumUNodes*Dim);
    MathUtils<double>::ExpandAndAddReducedMatrix( UMatrix, ReducedKgMatrix, Dim );

    //Distribute stiffness block matrix into the elemental matrix
    GeoElementUtilities::AssembleUBlockMatrix(rLeftHandSideMatrix, UMatrix, NumUNodes, Dim);

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateAndAddGeometricStiffnessMatrix()") << std::endl;

    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    CalculateKinematics(ElementVariables& rVariables,
                        const SizeType GPoint)
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateKinematics()") << std::endl;

    //Setting the vector of shape functions and the matrix of the shape functions global gradients
    noalias(rVariables.Nu) = row(rVariables.NuContainer, GPoint);
    noalias(rVariables.Np) = row(rVariables.NpContainer, GPoint);

    rVariables.detJ0 =
        this->CalculateDerivativesOnReferenceConfiguration(this->GetGeometry(),
                                                           rVariables.J0,
                                                           rVariables.InvJ0,
                                                           rVariables.DNu_DX,
                                                           GPoint,
                                                           this->GetIntegrationMethod());

    // Calculating jacobian and DNu_DX
    Matrix J, inv_J;
    rVariables.detJ0 =
        this->CalculateDerivativesOnCurrentConfiguration(this->GetGeometry(),
                                                         J,
                                                         inv_J,
                                                         rVariables.DNu_DX,
                                                         GPoint,
                                                         this->GetIntegrationMethod());

    KRATOS_ERROR_IF(rVariables.detJ0 < 0.0)
     << "ERROR:: ELEMENT ID: "
     << this->Id()
     << " INVERTED. DETJ0: "
     << rVariables.detJ0
     << std::endl;

    // Deformation gradient
    Matrix DF = prod( J, rVariables.InvJ0 );

    const double detDF = MathUtils<double>::Det(DF);
    rVariables.detF = detDF * this->ReferenceConfigurationDeformationGradientDeterminant(GPoint);
    noalias(rVariables.F) = prod(DF, this->ReferenceConfigurationDeformationGradient(GPoint));

    // Calculating operator B
    this->CalculateBMatrix( rVariables.B, rVariables.DNu_DX);

    // Calculating jacobian and DNp_DX
    double detJp = this->CalculateDerivativesOnCurrentConfiguration(*mpPressureGeometry,
                                                                    J,
                                                                    inv_J,
                                                                    rVariables.DNp_DX,
                                                                    GPoint,
                                                                    this->GetIntegrationMethod());

    KRATOS_ERROR_IF(detJp < 0.0)
     << "ERROR:: ELEMENT ID: "
     << this->Id()
     << " INVERTED. detJp: "
     << detJp
     << std::endl;

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateKinematics()") << std::endl;

    KRATOS_CATCH( "" )

}

//----------------------------------------------------------------------------------------
double UpdatedLagrangianUPwDiffOrderElement::
    CalculateDerivativesOnReferenceConfiguration(const GeometryType& Geometry,
                                                 Matrix& J0,
                                                 Matrix& InvJ0,
                                                 Matrix& DNu_DX,
                                                 const IndexType& GPoint,
                                                 IntegrationMethod ThisIntegrationMethod) const
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateDerivativesOnReferenceConfiguration()") << std::endl;

    J0.clear();

    double detJ0;

    Matrix deltaDisplacement;
    deltaDisplacement = this->CalculateDeltaDisplacement(deltaDisplacement);

    J0 = Geometry.Jacobian(J0, GPoint, ThisIntegrationMethod, deltaDisplacement);

    const Matrix& DN_De = Geometry.ShapeFunctionsLocalGradients(ThisIntegrationMethod)[GPoint];

    MathUtils<double>::InvertMatrix( J0, InvJ0, detJ0 );

    noalias( DNu_DX ) = prod( DN_De, InvJ0);

    return detJ0;

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateDerivativesOnReferenceConfiguration()") << std::endl;

    KRATOS_CATCH( "" )

}

//----------------------------------------------------------------------------------------
double UpdatedLagrangianUPwDiffOrderElement::
    CalculateDerivativesOnCurrentConfiguration( const GeometryType& Geometry,
                                                Matrix& rJ,
                                                Matrix& rInvJ,
                                                Matrix& rDN_DX,
                                                const IndexType& PointNumber,
                                                IntegrationMethod ThisIntegrationMethod ) const
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateDerivativesOnCurrentConfiguration()") << std::endl;

    double detJ;
    rJ = Geometry.Jacobian( rJ, PointNumber, ThisIntegrationMethod );
    const Matrix& DN_De = Geometry.ShapeFunctionsLocalGradients(ThisIntegrationMethod)[PointNumber];
    MathUtils<double>::InvertMatrix( rJ, rInvJ, detJ );
    GeometryUtils::ShapeFunctionsGradients(DN_De, rInvJ, rDN_DX);

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateDerivativesOnCurrentConfiguration()") << std::endl;

    return detJ;

    KRATOS_CATCH( "" )

}

//----------------------------------------------------------------------------------------
Matrix& UpdatedLagrangianUPwDiffOrderElement::
    CalculateDeltaDisplacement(Matrix& DeltaDisplacement) const
{
    KRATOS_TRY

    //KRATOS_INFO("0-UpdatedLagrangianUPwDiffOrderElement::CalculateDeltaDisplacement()") << std::endl;

    const GeometryType& rGeom = GetGeometry();
    const SizeType NumUNodes = rGeom.PointsNumber();
    const SizeType Dim = rGeom.WorkingSpaceDimension();

    DeltaDisplacement.resize(NumUNodes , Dim, false);

    for ( IndexType iNode = 0; iNode < NumUNodes; iNode++ ) {
        const array_1d<double, 3>& currentDisplacement  = GetGeometry()[iNode].FastGetSolutionStepValue(DISPLACEMENT);
        const array_1d<double, 3>& previousDisplacement = GetGeometry()[iNode].FastGetSolutionStepValue(DISPLACEMENT,1);

        for ( IndexType iDim = 0; iDim < Dim; ++iDim )
            DeltaDisplacement(iNode, iDim) = currentDisplacement[iDim] - previousDisplacement[iDim];
    }

    //KRATOS_INFO("1-UpdatedLagrangianUPwDiffOrderElement::CalculateDeltaDisplacement()") << std::endl;

    return DeltaDisplacement;

    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------
double UpdatedLagrangianUPwDiffOrderElement::
    ReferenceConfigurationDeformationGradientDeterminant(const IndexType GPoint) const
{
    if (mF0Computed == false)
        return mDetF0[GPoint];

    return 1.0;
}

//----------------------------------------------------------------------------------------
Matrix UpdatedLagrangianUPwDiffOrderElement::
    ReferenceConfigurationDeformationGradient(const IndexType GPoint) const
{
    if (mF0Computed == false)
        return mF0[GPoint];

    const SizeType Dim = GetGeometry().WorkingSpaceDimension();
    return IdentityMatrix(Dim);
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    CalculateOnIntegrationPoints(const Variable<double>& rVariable,
                                 std::vector<double>& rValues,
                                 const ProcessInfo& rCurrentProcessInfo)
{
    if (rVariable == REFERENCE_DEFORMATION_GRADIENT_DETERMINANT) {
        if (rValues.size() != mConstitutiveLawVector.size())
            rValues.resize(mConstitutiveLawVector.size());

        for ( IndexType GPoint = 0; GPoint < mConstitutiveLawVector.size(); ++GPoint )
            rValues[GPoint] = mDetF0[GPoint];
    } else {
        SmallStrainUPwDiffOrderElement::CalculateOnIntegrationPoints(rVariable, rValues, rCurrentProcessInfo);
    }
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    CalculateOnIntegrationPoints(const Variable<Matrix>& rVariable,
                                 std::vector<Matrix>& rValues,
                                 const ProcessInfo& rCurrentProcessInfo)
{
    if (rVariable == REFERENCE_DEFORMATION_GRADIENT) {
        if (rValues.size() != mConstitutiveLawVector.size())
            rValues.resize(mConstitutiveLawVector.size());

        for ( IndexType GPoint = 0; GPoint < mConstitutiveLawVector.size(); ++GPoint )
            rValues[GPoint] = mF0[GPoint];
    } else {
        SmallStrainUPwDiffOrderElement::CalculateOnIntegrationPoints(rVariable, rValues, rCurrentProcessInfo);
    }
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    SetValuesOnIntegrationPoints(const Variable<double>& rVariable,
                                 std::vector<double>& rValues,
                                 const ProcessInfo& rCurrentProcessInfo)
{
    if (rVariable == REFERENCE_DEFORMATION_GRADIENT_DETERMINANT) {
        KRATOS_ERROR_IF(rValues.size() != mConstitutiveLawVector.size()) 
            << "Can not set REFERENCE_DEFORMATION_GRADIENT_DETERMINANT, expected size: "
            << mConstitutiveLawVector.size() << " current size: " << rValues.size() << std::endl;

        for ( IndexType GPoint = 0; GPoint < mConstitutiveLawVector.size(); ++GPoint ) {
            mDetF0[GPoint] = rValues[GPoint];
        }
    } else {
        SmallStrainUPwDiffOrderElement::SetValuesOnIntegrationPoints(rVariable, rValues, rCurrentProcessInfo);
    }
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::
    SetValuesOnIntegrationPoints(const Variable<Matrix>& rVariable,
                                 std::vector<Matrix>& rValues,
                                 const ProcessInfo& rCurrentProcessInfo)
{
    if (rVariable == REFERENCE_DEFORMATION_GRADIENT) {
        KRATOS_ERROR_IF(rValues.size() != mConstitutiveLawVector.size())
            << "Can not set REFERENCE_DEFORMATION_GRADIENT, expected size: "
            << mConstitutiveLawVector.size() << " current size: " << rValues.size() << std::endl;

        for ( IndexType GPoint = 0; GPoint < mConstitutiveLawVector.size(); ++GPoint )
            mF0[GPoint] = rValues[GPoint];
    } else {
        SmallStrainUPwDiffOrderElement::SetValuesOnIntegrationPoints(rVariable, rValues, rCurrentProcessInfo);
    }
}


//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::save( Serializer& rSerializer ) const
{
    KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, SmallStrainUPwDiffOrderElement );
    rSerializer.save("F0Computed", mF0Computed);
    rSerializer.save("DetF0", mDetF0);
    rSerializer.save("F0", mF0);
}

//----------------------------------------------------------------------------------------
void UpdatedLagrangianUPwDiffOrderElement::load( Serializer& rSerializer )
{
    KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, SmallStrainUPwDiffOrderElement );
    rSerializer.load("F0Computed", mF0Computed);
    rSerializer.load("DetF0", mDetF0);
    rSerializer.load("F0", mF0);
}

} // Namespace Kratos


