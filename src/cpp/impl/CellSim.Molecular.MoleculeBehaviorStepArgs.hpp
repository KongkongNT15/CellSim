#ifndef CELLSIM_MOLECULAR_MOLECULEBEHAVIORSTEPARGS_HPP
#define CELLSIM_MOLECULAR_MOLECULEBEHAVIORSTEPARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::Molecular
{
    struct MoleculeBehaviorStepArgs {
        const Cells::CellCollection* Cells;

        constexpr MoleculeBehaviorStepArgs(
            const Cells::CellCollection* cells
        ) noexcept;
    };
}

namespace CellSim::Molecular
{
    constexpr MoleculeBehaviorStepArgs::MoleculeBehaviorStepArgs(
        const Cells::CellCollection* cells
    ) noexcept
        : Cells(cells)
    {
    }
}

#endif //!CELLSIM_MOLECULAR_MOLECULEBEHAVIORSTEPARGS_HPP