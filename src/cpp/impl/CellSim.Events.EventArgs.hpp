#ifndef CELLSIM_EVENTS_EVENTARGS_HPP
#define CELLSIM_EVENTS_EVENTARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::Events
{
    struct EventArgs {
        Cells::CellCollection* Cells;

        ::std::vector<Molecular::MoleculeField>* Molecules;

        constexpr EventArgs(
            Cells::CellCollection* cells,
            ::std::vector<Molecular::MoleculeField>* molecules
        ) noexcept;
    };
}

namespace CellSim::Events
{
    constexpr EventArgs::EventArgs(
        Cells::CellCollection* cells,
        ::std::vector<Molecular::MoleculeField>* molecules
    ) noexcept
        : Cells(cells)
        , Molecules(molecules)
    {
    }
}

#endif //!CELLSIM_EVENTS_EVENTARGS_HPP