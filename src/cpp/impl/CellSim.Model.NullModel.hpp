#ifndef CELLSIM_MODEL_NULLMODEL_HPP
#define CELLSIM_MODEL_NULLMODEL_HPP

#include "base.hpp"
#include "CellSim.Model.CellSimulationModel.hpp"
#include "CellSim.Model.SimulationModelForceComputationArgs.hpp"
#include "CellSim.Model.SimulationModelStepArgs.hpp"
#include "CellSim.Numerics.Vector3T.hpp"

namespace CellSim::Model
{
    /// @brief 何もしない力学モデル
    class NullModel : public CellSimulationModel {
        public:

        class Params : public CellSimulationModel::Params {
            public:
        };

        constexpr void BeforeAdvanceStep(
            const Simulation* sender,
            SimulationModelStepArgs args
        ) override;

        constexpr Numerics::Vector3 ComputeForceOnCell(
            const Simulation* sender,
            SimulationModelForceComputationArgs args
        ) const override;

        constexpr void InitializeCells(
            const Simulation* sender,
            Cells::CellCollection& cells
        ) override;

        constexpr void OnAdvanceStep(
            const Simulation* sender,
            SimulationModelStepArgs args
        ) override;

        constexpr bool UseCellAlgorithm() const noexcept override;
    };
}

namespace CellSim::Model
{
    constexpr void NullModel::BeforeAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs
    )
    {
    }

    constexpr Numerics::Vector3 NullModel::ComputeForceOnCell(
        const Simulation*,
        SimulationModelForceComputationArgs
    ) const
    {
        return Numerics::Vector3();
    }

    constexpr void NullModel::InitializeCells(
        const Simulation*,
        Cells::CellCollection&
    )
    {
    }

    constexpr void NullModel::OnAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs
    )
    {
    }

    constexpr bool NullModel::UseCellAlgorithm() const noexcept
    {
        return false;
    }
}

#endif //!CELLSIM_MODEL_NULLMODEL_HPP