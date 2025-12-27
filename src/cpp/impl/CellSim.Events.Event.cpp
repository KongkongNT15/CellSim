#include "CellSim.Events.Event.hpp"
#include "CellSim.Events.RemoveCellEvent.hpp"

#include <stdexcept>
#include <nlohmann/json.hpp>

namespace CellSim::Events
{
    Event* Event::FromJson(
        ::nlohmann::json const& j
    )
    {
        throw ::std::runtime_error("Not Implemented");
    }
}