#include "CellSim.Events.RemoveCellEvent.hpp"
#include "CellSim.Events.EventArgs.hpp"
#include "CellSim.Events.EventExpirationArgs.hpp"

namespace CellSim::Events
{
    bool RemoveCellEvent::IsExpired(
        EventExpirationArgs
    )
    {
        return m_isDeleteCell;
    }

    void RemoveCellEvent::Run(
        const Simulation*,
        EventArgs args
    )
    {
        auto cells = *args.Cells;
        for (auto itr = cells.begin(), end = cells.end(); itr != end; ++itr) {
            if (itr->Id() == m_cellId) {
                cells.erase(itr);
                break;
            }
        }

        m_isDeleteCell = true;
    }
}