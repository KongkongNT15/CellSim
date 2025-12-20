#ifndef CELLSIM_EVENTS_EVENTEXPIRATIONARGS_HPP
#define CELLSIM_EVENTS_EVENTEXPIRATIONARGS_HPP

#include "base.hpp"

namespace CellSim::Events
{
    struct EventExpirationArgs {
        uint64_t Step;

        constexpr EventExpirationArgs(
            uint64_t step
        ) noexcept;
    };
}

namespace CellSim::Events
{
    constexpr EventExpirationArgs::EventExpirationArgs(
        uint64_t step
    ) noexcept
        : Step(step)
    {
    }
}

#endif //!CELLSIM_EVENTS_EVENTEXPIRATIONARGS_HPP