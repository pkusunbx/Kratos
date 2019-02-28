//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Riccardo Rossi
//

#if !defined(KRATOS_RESIDUAL_CRITERIA )
#define  KRATOS_RESIDUAL_CRITERIA

// System includes

// External includes

// Project includes
#include "includes/model_part.h"
#include "includes/define.h"
#include "solving_strategies/convergencecriterias/convergence_criteria.h"

namespace Kratos
{

///@name Kratos Globals
///@{


///@}
///@name Type Definitions
///@{

///@}


///@name  Enum's
///@{


///@}
///@name  Functions
///@{



///@}
///@name Kratos Classes
///@{

/**
 * @class ResidualCriteria
 * @ingroup KratosCore
 * @brief This is a convergence criteria that employes the residual as criteria
 * @details The reactions from the RHS are not computed in the residual
 * @author Riccardo Rossi
*/
template<class TSparseSpace,
         class TDenseSpace
         >
class ResidualCriteria
    : public  ConvergenceCriteria< TSparseSpace, TDenseSpace >
{
public:
    ///@name Type Definitions
    ///@{

    KRATOS_CLASS_POINTER_DEFINITION( ResidualCriteria );

    typedef ConvergenceCriteria< TSparseSpace, TDenseSpace > BaseType;

    typedef TSparseSpace SparseSpaceType;

    typedef typename BaseType::TDataType TDataType;

    typedef typename BaseType::DofsArrayType DofsArrayType;

    typedef typename BaseType::TSystemMatrixType TSystemMatrixType;

    typedef typename BaseType::TSystemVectorType TSystemVectorType;

    typedef std::size_t IndexType;

    typedef std::size_t SizeType;

    ///@}
    ///@name Life Cycle
    ///@{

    //* Constructor.
    explicit ResidualCriteria(Kratos::Parameters Settings)
        : BaseType()
    {
        if (Settings.Has("residual_absolute_tolerance")) {
            mAlwaysConvergedNorm = Settings["residual_absolute_tolerance"].GetDouble();
        } else if (Settings.Has("absolute_tolerance")) {
            mAlwaysConvergedNorm = Settings["absolute_tolerance"].GetDouble();
        } else {
            KRATOS_WARNING("ResidualCriteria") << "residual_absolute_tolerance or absolute_tolerance nor defined on settings. Using default 1.0e-9" << std::endl;
            mAlwaysConvergedNorm = 1.0e-9;
        }
        if (Settings.Has("residual_relative_tolerance")) {
            mRatioTolerance = Settings["residual_relative_tolerance"].GetDouble();
        } else if (Settings.Has("relative_tolerance")) {
            mRatioTolerance = Settings["relative_tolerance"].GetDouble();
        } else {
            KRATOS_WARNING("ResidualCriteria") << "residual_relative_tolerance or relative_tolerance nor defined on settings. Using default 1.0e-4" << std::endl;
            mRatioTolerance = 1.0e-4;
        }

        this->mActualizeRHSIsNeeded = true;
    }

    //* Constructor.
    explicit ResidualCriteria(
        TDataType NewRatioTolerance,
        TDataType AlwaysConvergedNorm)
        : BaseType(),
          mRatioTolerance(NewRatioTolerance),
          mAlwaysConvergedNorm(AlwaysConvergedNorm)
    {
        this->mActualizeRHSIsNeeded = true;
    }

    //* Copy constructor.
    explicit ResidualCriteria( ResidualCriteria const& rOther )
      :BaseType(rOther)
      ,mRatioTolerance(rOther.mRatioTolerance)
      ,mInitialResidualNorm(rOther.mInitialResidualNorm)
      ,mCurrentResidualNorm(rOther.mCurrentResidualNorm)
      ,mAlwaysConvergedNorm(rOther.mAlwaysConvergedNorm)
      ,mReferenceDispNorm(rOther.mReferenceDispNorm)
    {
        this->mActualizeRHSIsNeeded = true;
    }

    //* Destructor.
    ~ResidualCriteria() override {}

    ///@}
    ///@name Operators
    ///@{

    /**
     * @brief Criterias that need to be called after getting the solution
     * @details Compute relative and absolute error.
     * @param rModelPart Reference to the ModelPart containing the problem.
     * @param rDofSet Reference to the container of the problem's degrees of freedom (stored by the BuilderAndSolver)
     * @param A System matrix (unused)
     * @param Dx Vector of results (variations on nodal variables)
     * @param b RHS vector (residual + reactions)
     * @return true if convergence is achieved, false otherwise
     */
    bool PostCriteria(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& A,
        const TSystemVectorType& Dx,
        const TSystemVectorType& b
        ) override
    {
        const SizeType size_b = TSparseSpace::Size(b);
        if (size_b != 0) { //if we are solving for something

            SizeType size_residual;
            CalculateResidualNorm(mCurrentResidualNorm, size_residual, rDofSet, b);

            TDataType ratio = 0.0;
            if(mInitialResidualNorm < std::numeric_limits<TDataType>::epsilon()) {
                ratio = 0.0;
            } else {
                ratio = mCurrentResidualNorm/mInitialResidualNorm;
            }

            const TDataType float_size_residual = static_cast<TDataType>(size_residual);
            const TDataType absolute_norm = (mCurrentResidualNorm/float_size_residual);

            KRATOS_INFO_IF("RESIDUAL CRITERION", this->GetEchoLevel() > 0 && rModelPart.GetCommunicator().MyPID() == 0) << " :: [ Obtained ratio = " << ratio << "; Expected ratio = " << mRatioTolerance << "; Absolute norm = " << absolute_norm << "; Expected norm =  " << mAlwaysConvergedNorm << "]" << std::endl;

            rModelPart.GetProcessInfo()[CONVERGENCE_RATIO] = ratio;
            rModelPart.GetProcessInfo()[RESIDUAL_NORM] = absolute_norm;

            if (ratio <= mRatioTolerance || absolute_norm < mAlwaysConvergedNorm) {
                KRATOS_INFO_IF("RESIDUAL CRITERION", this->GetEchoLevel() > 0 && rModelPart.GetCommunicator().MyPID() == 0) << "Convergence is achieved" << std::endl;
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    /**
     * @brief This function initialize the convergence criteria
     * @param rModelPart Reference to the ModelPart containing the problem. (unused)
     */
    void Initialize(ModelPart& rModelPart) override
    {
        BaseType::Initialize(rModelPart);
    }

    /**
     * @brief This function initializes the solution step
     * @param rModelPart Reference to the ModelPart containing the problem.
     * @param rDofSet Reference to the container of the problem's degrees of freedom (stored by the BuilderAndSolver)
     * @param rA System matrix (unused)
     * @param rDx Vector of results (variations on nodal variables)
     * @param rb RHS vector (residual + reactions)
     */
    void InitializeSolutionStep(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& rA,
        const TSystemVectorType& rDx,
        const TSystemVectorType& rb
        ) override
    {
        BaseType::InitializeSolutionStep(rModelPart, rDofSet, rA, rDx, rb);

        mActiveDofs.resize(rDofSet.size());

        #pragma omp parallel for
        for(int i=0; i<static_cast<int>(mActiveDofs.size()); ++i) {
            mActiveDofs[i] = true;
        }

        #pragma omp parallel for 
        for (int i=0; i<static_cast<int>(rDofSet.size()); ++i) {
            auto it_dof = rDofSet.begin() + i;
            if (it_dof->IsFixed()) {
                mActiveDofs[it_dof->EquationId()] = false;
            }
        }
        
        for (const auto& r_mpc : rModelPart.MasterSlaveConstraints()) {
            for (const auto& r_dof : r_mpc.GetMasterDofsVector()) {
                mActiveDofs[r_dof->EquationId()] = false;
            }
            for (const auto& r_dof : r_mpc.GetSlaveDofsVector()) {
                mActiveDofs[r_dof->EquationId()] = false;                
            }
        }

        SizeType size_residual;
        CalculateResidualNorm(mInitialResidualNorm, size_residual, rDofSet, rb);
    }

    /**
     * @brief This function finalizes the solution step
     * @param rModelPart Reference to the ModelPart containing the problem.
     * @param rDofSet Reference to the container of the problem's degrees of freedom (stored by the BuilderAndSolver)
     * @param A System matrix (unused)
     * @param Dx Vector of results (variations on nodal variables)
     * @param b RHS vector (residual + reactions)
     */
    void FinalizeSolutionStep(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& A,
        const TSystemVectorType& Dx,
        const TSystemVectorType& b
        ) override
    {
        BaseType::FinalizeSolutionStep(rModelPart, rDofSet, A, Dx, b);
    }

    ///@}
    ///@name Operations
    ///@{


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
    std::string Info() const override
    {
        return "ResidualCriteria";
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << Info();
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
        rOStream << Info();
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

    /**
     * @brief This method computes the norm of the residual
     * @details It checks if the dof is fixed
     * @param rResidualSolutionNorm The norm of the residual
     * @param rDofNum The number of DoFs
     * @param rDofSet Reference to the container of the problem's degrees of freedom (stored by the BuilderAndSolver)
     * @param b RHS vector (residual + reactions)
     */
    virtual void CalculateResidualNorm(
        TDataType& rResidualSolutionNorm,
        SizeType& rDofNum,
        DofsArrayType& rDofSet,
        const TSystemVectorType& b
        )
    {
        // Initialize
        TDataType residual_solution_norm = TDataType();
        SizeType dof_num = 0;

        // Loop over Dofs
        #pragma omp parallel for reduction(+:residual_solution_norm,dof_num)
        for (int i = 0; i < static_cast<int>(rDofSet.size()); i++) {
            auto it_dof = rDofSet.begin() + i;

            IndexType dof_id = it_dof->EquationId();
            TDataType residual_dof_value;

            if (mActiveDofs[dof_id]) {
                residual_dof_value = TSparseSpace::GetValue(b,dof_id);
                residual_solution_norm += std::pow(residual_dof_value, 2);
                dof_num++;
            }
        }

        rDofNum = dof_num;
        rResidualSolutionNorm = std::sqrt(residual_solution_norm);
    }

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


    ///@}
    ///@name Member Variables
    ///@{

    TDataType mRatioTolerance;      /// The ratio threshold for the norm of the residual

    TDataType mInitialResidualNorm; /// The reference norm of the residual

    TDataType mCurrentResidualNorm; /// The current norm of the residual

    TDataType mAlwaysConvergedNorm; /// The absolute value threshold for the norm of the residual

    TDataType mReferenceDispNorm;   /// The norm at the beginning of the iterations

    std::vector<bool> mActiveDofs;


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


    ///@}

}; // Class ClassName

///@}

///@name Type Definitions
///@{


///@}

}  // namespace Kratos.

#endif // KRATOS_NEW_DISPLACEMENT_CRITERIA  defined

