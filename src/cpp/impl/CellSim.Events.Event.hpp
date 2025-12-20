#ifndef CELLSIM_EVENTS_EVENT_HPP
#define CELLSIM_EVENTS_EVENT_HPP

#include "base.hpp"

namespace CellSim::Events
{
    class Event {
        public:

        virtual ~Event() = default;

        /// @brief 
        /// @param args 
        /// @return 
        [[nodiscard]]
        virtual bool IsExpired(
            EventExpirationArgs args
        ) = 0;

        /// @brief イベントを実行
        /// @param sender 
        /// @param args 
        virtual void Run(
            const Simulation* sender,
            EventArgs args
        ) = 0;
    };
}

#endif //!CELLSIM_EVENTS_EVENT_HPP