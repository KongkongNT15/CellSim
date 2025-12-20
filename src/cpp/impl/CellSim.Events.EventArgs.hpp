#ifndef CELLSIM_EVENTS_EVENTARGS_HPP
#define CELLSIM_EVENTS_EVENTARGS_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::Events
{
    struct EventArgs {
        ::std::vector<Cells::Cell>* Cells;

        ::std::vector<Molecular::MoleculeField>* Molecules;

        constexpr EventArgs(
            ::std::vector<Cells::Cell>* cells,
            ::std::vector<Molecular::MoleculeField>* molecules
        ) noexcept;
    };
}

namespace CellSim::Events
{
    constexpr EventArgs::EventArgs(
        ::std::vector<Cells::Cell>* cells,
        ::std::vector<Molecular::MoleculeField>* molecules
    ) noexcept
        : Cells(cells)
        , Molecules(molecules)
    {
    }
}

#endif //!CELLSIM_EVENTS_EVENTARGS_HPP