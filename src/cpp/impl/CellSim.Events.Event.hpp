#ifndef CELLSIM_EVENTS_EVENT_HPP
#define CELLSIM_EVENTS_EVENT_HPP

#include "base.hpp"

#include <nlohmann/json_fwd.hpp>

namespace CellSim::Events
{
    class Event {
        public:

        [[nodiscard]]
        static Event* FromJson(
            ::nlohmann::json const& j
        );

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