#ifndef CELLSIM_CELLALGORITHMS_CELLALGORITHMSTEPARGS_HPP
#define CELLSIM_CELLALGORITHMS_CELLALGORITHMSTEPARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::CellAlgorithms
{
    /// @brief BeforeAdvanceStep(), OnAdvanceStep()の引数
    struct CellAlgorithmStepArgs {
        const Cells::CellCollection* Cells;
        const ::std::vector<Molecular::MoleculeField>* Fields;

        constexpr CellAlgorithmStepArgs(
            const Cells::CellCollection* cells,
            const ::std::vector<Molecular::MoleculeField>* fields
        ) noexcept;
    };
}

namespace CellSim::CellAlgorithms
{
    constexpr CellAlgorithmStepArgs::CellAlgorithmStepArgs(
        const Cells::CellCollection* cells,
        const ::std::vector<Molecular::MoleculeField>* fields
    ) noexcept
        : Cells(cells)
        , Fields(fields)
    {
    }
}

#endif //!CELLSIM_CELLALGORITHMS_CELLALGORITHMSTEPARGS_HPP