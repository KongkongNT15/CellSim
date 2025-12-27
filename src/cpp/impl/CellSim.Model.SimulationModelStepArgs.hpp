#ifndef CELLSIM_MODEL_SIMULATIONMODELSTEPARGS_HPP
#define CELLSIM_MODEL_SIMULATIONMODELSTEPARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::Model
{
    /// @brief BeforeAdvanceStep(), OnAdvanceStep()の引数
    struct SimulationModelStepArgs {
        Cells::CellCollection* Cells;
        const ::std::vector<Molecular::MoleculeField>* Fields;

        constexpr SimulationModelStepArgs(
            Cells::CellCollection* cells,
            const ::std::vector<Molecular::MoleculeField>* fields
        ) noexcept;
    };
}

namespace CellSim::Model
{
    constexpr SimulationModelStepArgs::SimulationModelStepArgs(
        Cells::CellCollection* cells,
        const ::std::vector<Molecular::MoleculeField>* fields
    ) noexcept
        : Cells(cells)
        , Fields(fields)
    {
    }
}

#endif //!CELLSIM_MODEL_SIMULATIONMODELSTEPARGS_HPP