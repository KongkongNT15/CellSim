#ifndef CELLSIM_EVENTS_REMOVECELLEVENT_HPP
#define CELLSIM_EVENTS_REMOVECELLEVENT_HPP

#include "base.hpp"
#include "CellSim.Events.Event.hpp"

#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellInfo.hpp"

namespace CellSim::Events
{
    class RemoveCellEvent : public Event {
        private:
        uint32_t m_cellId;
        bool m_isDeleteCell;
        public:

        constexpr RemoveCellEvent(
            uint32_t cellId
        ) noexcept;

        constexpr RemoveCellEvent(
            Cells::Cell const& cell
        ) noexcept;

        /// @brief ターゲットのCellのId
        [[nodiscard]]
        constexpr uint32_t CellId() const noexcept;

        bool IsExpired(
            EventExpirationArgs args
        ) override;

        /// @brief イベントを実行
        /// @param sender 
        /// @param args 
        void Run(
            const Simulation* sender,
            EventArgs args
        ) override;
    };
}

namespace CellSim::Events
{
    constexpr RemoveCellEvent::RemoveCellEvent(
        uint32_t cellId
    ) noexcept
        : m_cellId(cellId)
        , m_isDeleteCell(false)
    {
    }

    constexpr RemoveCellEvent::RemoveCellEvent(
        Cells::Cell const& cell
    ) noexcept
        : RemoveCellEvent(cell.Id())
    {
    }

    constexpr uint32_t RemoveCellEvent::CellId() const noexcept
    {
        return m_cellId;
    }
}

#endif //!CELLSIM_EVENTS_REMOVECELLEVENT_HPP