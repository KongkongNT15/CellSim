#include "CellSim.Events.RemoveCellEvent.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Events.EventArgs.hpp"
#include "CellSim.Events.EventExpirationArgs.hpp"

#include <stdexcept>
#include <nlohmann/json.hpp>

#include "CellSim.Messages.hpp"

namespace CellSim::Events
{
    RemoveCellEvent* RemoveCellEvent::FromJson(
        ::nlohmann::json const& j
    )
    {
        try {
            return new RemoveCellEvent(
                j.at("cellId").get<uint32_t>()
            );
        }
        catch (::std::bad_alloc) {
            ::std::rethrow_exception(::std::current_exception());
        }
        catch (...) {
            throw ::std::runtime_error(
                Messages::Get(
                    "Events.RemoveCellEvent.FromJson.JsonError"
                )
            );
        }
    }

    bool RemoveCellEvent::IsExpired(
        const Simulation*,
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
        args.Cells->Remove(m_cellId);
        
        m_isDeleteCell = true;
    }
}