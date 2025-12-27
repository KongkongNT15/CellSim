#ifndef CELLSIM_CELLALGORITHMS_CELLALGORITHMAFFECTABLECELLQUERYARGS
#define CELLSIM_CELLALGORITHMS_CELLALGORITHMAFFECTABLECELLQUERYARGS

#include "base.hpp"

#include <vector>

namespace CellSim::CellAlgorithms
{
    struct CellAlgorithmAffectableCellQueryArgs {
        const Cells::CellCollection* Cells;
        const ::std::vector<Molecular::MoleculeField>* Fields;
        const Cells::Cell* Target;

        constexpr CellAlgorithmAffectableCellQueryArgs(
            const Cells::Cell* target,
            const Cells::CellCollection* cells,
            const ::std::vector<Molecular::MoleculeField>* fields
        ) noexcept;
    };
}

namespace CellSim::CellAlgorithms
{
    constexpr CellAlgorithmAffectableCellQueryArgs::CellAlgorithmAffectableCellQueryArgs(
        const Cells::Cell* target,
        const Cells::CellCollection* cells,
        const ::std::vector<Molecular::MoleculeField>* fields
    ) noexcept
        : Cells(cells)
        , Fields(fields)
        , Target(target)
    {
    }
}

#endif //!CELLSIM_CELLALGORITHMS_CELLALGORITHMAFFECTABLECELLQUERYARGS