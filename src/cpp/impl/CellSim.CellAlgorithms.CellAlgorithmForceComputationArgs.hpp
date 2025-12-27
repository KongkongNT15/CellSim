#ifndef CELLSIM_CELLALGORITHMS_CELLALGORITHMFORCECOMPUTATIONARGS_HPP
#define CELLSIM_CELLALGORITHMS_CELLALGORITHMFORCECOMPUTATIONARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::CellAlgorithms
{
    struct CellAlgorithmForceComputationArgs {
        const Cells::CellCollection* Cells;
        const ::std::vector<Molecular::MoleculeField>* Fields;
        const Model::CellSimulationModel* SimulationModel;
        const Cells::Cell* Target;

        constexpr CellAlgorithmForceComputationArgs(
            const Cells::Cell* target,
            const Cells::CellCollection* cells,
            const ::std::vector<Molecular::MoleculeField>* fields,
            const Model::CellSimulationModel* simulationModel
        ) noexcept;
    };
}

namespace CellSim::CellAlgorithms
{
    constexpr CellAlgorithmForceComputationArgs::CellAlgorithmForceComputationArgs(
        const Cells::Cell* target,
        const Cells::CellCollection* cells,
        const ::std::vector<Molecular::MoleculeField>* fields,
        const Model::CellSimulationModel* simulationModel
    ) noexcept
        : Cells(cells)
        , Fields(fields)
        , SimulationModel(simulationModel)
        , Target(target)
    {
    }
}

#endif //!CELLSIM_CELLALGORITHMS_CELLALGORITHMFORCECOMPUTATIONARGS_HPP