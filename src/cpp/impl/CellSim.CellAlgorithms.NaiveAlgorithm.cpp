#include "CellSim.CellAlgorithms.NaiveAlgorithm.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmForceComputationArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmInteractionArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmStepArgs.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Model.CellSimulationModel.hpp"
#include "CellSim.Model.SimulationModelForceComputationArgs.hpp"
#include "CellSim.Threading.Generator.hpp"
#include "CellSim.Threading.ThreadPool.hpp"

namespace CellSim::CellAlgorithms
{
    void NaiveAlgorithm::ApplyInteraction(
        const Simulation* sender,
        CellAlgorithmInteractionArgs args
    )
    {
        Threading::ThreadPool::ParallelFor(
            args.Cells->begin(),
            args.Cells->end(),
            [sender, args] (Cells::Cell& cell) {
                cell.ApplyForce(
                    args.SimulationModel->ComputeForceOnCell(
                        sender,
                        {
                            &cell,
                            args.Cells,
                            args.Cells,
                            args.Fields,
                            nullptr
                        }
                    )
                );
            }
        );
    }

    void NaiveAlgorithm::BeforeAdvanceStep(
            const Simulation*,
            CellAlgorithmStepArgs
    )
    {
    }

    Numerics::Vector3 NaiveAlgorithm::ComputeForceOnCell(
        const Simulation* sender,
        CellAlgorithmForceComputationArgs args
    ) const
    {
        return args.SimulationModel->ComputeForceOnCell(
            sender,
            {
                args.Target,
                args.Cells,
                args.Cells,
                args.Fields,
                nullptr
            }
        );
    }

    ::std::vector<Cells::CellInfo> NaiveAlgorithm::GetAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs args
    ) const
    {
        ::std::vector<Cells::CellInfo> vec;

        if (args.Cells->Length() > 0) [[likely]] vec.reserve(args.Cells->Length() - 1);

        for (Cells::Cell const& cell : *args.Cells) {
            if (&cell == args.Target) continue;
            vec.emplace_back(cell);
        }

        return vec;
    }

    bool NaiveAlgorithm::HasMultithreadingSupport() const noexcept
    {
        return true;
    }

    Threading::Generator<Cells::CellInfo> NaiveAlgorithm::IterateAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs args
    ) const
    {
        for (Cells::Cell const& cell : *args.Cells) {
            if (&cell == args.Target) continue;
            co_yield cell;
        }
    }

    void NaiveAlgorithm::OnAdvanceStep(
            const Simulation*,
            CellAlgorithmStepArgs
    )
    {
    }

    bool NaiveAlgorithm::OverrideForceComputation() const noexcept
    {
        return true;
    }

    bool NaiveAlgorithm::OverrideInteraction() const noexcept
    {
        return true;
    }
}