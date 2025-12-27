#ifndef CELLSIM_CELLALGORITHMS_NULLALGORITHM_HPP
#define CELLSIM_CELLALGORITHMS_NULLALGORITHM_HPP

#include "base.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmStepArgs.hpp"
#include "CellSim.Threading.Generator.hpp"

namespace CellSim::CellAlgorithms
{
    /// @brief 何もしないアルゴリズム
    class NullAlgorithm : public CellAlgorithm {
        public:

        constexpr void BeforeAdvanceStep(
            const Simulation* sender,
            CellAlgorithmStepArgs args
        ) override;

        ::std::vector<Cells::CellInfo> GetAffectableCellInfos(
            const Model::CellSimulationModel* sender,
            CellAlgorithmAffectableCellQueryArgs args
        ) const override;

        constexpr bool HasMultithreadingSupport() const noexcept override;

        Threading::Generator<Cells::CellInfo> IterateAffectableCellInfos(
            const Model::CellSimulationModel* sender,
            CellAlgorithmAffectableCellQueryArgs args
        ) const override;

        constexpr void OnAdvanceStep(
            const Simulation* sender,
            CellAlgorithmStepArgs args
        ) override;
    };
}

namespace CellSim::CellAlgorithms
{
    constexpr void NullAlgorithm::BeforeAdvanceStep(
            const Simulation*,
            CellAlgorithmStepArgs
    )
    {
    }

    inline ::std::vector<Cells::CellInfo> NullAlgorithm::GetAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs
    ) const
    {
        return {};
    }

    constexpr bool NullAlgorithm::HasMultithreadingSupport() const noexcept
    {
        return true;
    }

    inline Threading::Generator<Cells::CellInfo>
    NullAlgorithm::IterateAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs
    ) const
    {
        co_return;
    }

    constexpr void NullAlgorithm::OnAdvanceStep(
            const Simulation*,
            CellAlgorithmStepArgs
    )
    {
    }
}

#endif //!CELLSIM_CELLALGORITHMS_NULLALGORITHM_HPP