#ifndef CELLSIM_CELLS_CELLCOLLECTION_HPP
#define CELLSIM_CELLS_CELLCOLLECTION_HPP

#include "base.hpp"
#include "CellSim.Cells.Cell.hpp"

#include <vector>

namespace CellSim::Cells
{
    class CellCollection {
        private:

        ::std::vector<Cell> m_vector;

        constexpr void RemoveUnsafe(
            ::std::vector<Cell>::iterator itr
        ) noexcept;

        [[deprecated("実装されていません、代わりにRemoveUnsafe()を使用してください。")]]
        void RemoveWithDetachUnsafe(
            ::std::vector<Cell>::iterator itr
        ) noexcept;

        public:

        static constexpr void AttachUnsafe(
            Cell& cell1,
            Cell& cell2
        ) noexcept;

        static bool Detach(
            Cell& cell1,
            Cell& cell2
        ) noexcept;

        static CellCollection FromCsv(
            const char* filePath
        );

        [[nodiscard]] constexpr Cell& operator[](size_t index) noexcept;
        [[nodiscard]] constexpr Cell const& operator[](size_t index) const noexcept;

        [[nodiscard]] constexpr Cell* begin() noexcept;
        [[nodiscard]] constexpr const Cell* begin() const noexcept;
        [[nodiscard]] constexpr Cell* end() noexcept;
        [[nodiscard]] constexpr const Cell* end() const noexcept;

        constexpr void Append(
            Cell&& cell
        );

        constexpr void DetachAll() noexcept;

        template <class... Args>
        Cell& Emplace(
            Args&&... args
        );

        /// @brief 
        /// @param cellId 
        /// @return Idに一致する要素がない場合は nullptr
        [[nodiscard]]
        constexpr Cell* GetCellPointer(
            uint32_t cellId
        ) noexcept;

        [[nodiscard]]
        constexpr size_t Length() const noexcept;

        constexpr bool Remove(
            uint32_t cellId
        ) noexcept;

        constexpr bool Remove(
            Cell& cell
        ) noexcept;

        /// @brief ::std::erase_if()と同じ
        /// @tparam TFunction 
        /// @param func 
        /// @return 
        template <class TFunction>
        size_t RemoveIf(
            TFunction func
        ) noexcept;

        constexpr void RemoveUnsafe(
            Cell& cell
        ) noexcept;
    };
}

namespace CellSim::Cells
{
    constexpr void CellCollection::RemoveUnsafe(
        ::std::vector<Cell>::iterator itr
    ) noexcept
    {
        m_vector.erase(itr);
    }
    
    constexpr void CellCollection::AttachUnsafe(
        Cell& cell1,
        Cell& cell2
    ) noexcept
    {
        cell1.m_attachedCells.push_back(&cell2);
        cell2.m_attachedCells.push_back(&cell1);
    }

    constexpr Cell& CellCollection::operator[](
        size_t index
    ) noexcept
    {
        return m_vector[index];
    }

    constexpr Cell const& CellCollection::operator[](
        size_t index
    ) const noexcept
    {
        return m_vector[index];
    }

    constexpr Cell* CellCollection::begin() noexcept
    {
        return m_vector.data();
    }

    constexpr const Cell* CellCollection::begin() const noexcept
    {
        return m_vector.data();
    }

    constexpr Cell* CellCollection::end() noexcept
    {
        return m_vector.data() + m_vector.size();
    }

    constexpr const Cell* CellCollection::end() const noexcept
    {
        return m_vector.data() + m_vector.size();
    }

    constexpr void CellCollection::Append(
        Cell&& cell
    )
    {
        m_vector.push_back(
            ::std::move(cell)
        );
    }

    constexpr void CellCollection::DetachAll() noexcept
    {
        for (Cell& cell : m_vector) {
            cell.m_attachedCells.clear();
        }
    }

    template <class... Args>
    Cell& CellCollection::Emplace(
        Args&&... args
    )
    {
        return m_vector.emplace_back(
            ::std::forward<Args>(args)...
        );
    }

    constexpr Cell* CellCollection::GetCellPointer(
        uint32_t cellId
    ) noexcept
    {
        for (Cell& cell : m_vector) {
            if (cell.Id() == cellId) return &cell;
        }
        return nullptr;
    }

    constexpr size_t CellCollection::Length() const noexcept
    {
        return m_vector.size();
    }

    constexpr bool CellCollection::Remove(
        uint32_t cellId
    ) noexcept
    {
        for (
            auto itr = m_vector.begin(), end = m_vector.end();
            itr != end;
            ++itr
        ) {
            if (itr->Id() == cellId) {
                RemoveUnsafe(itr);
                return true;
            }
        }

        return false;
    }

    constexpr bool CellCollection::Remove(
        Cell& cell
    ) noexcept
    {
        ptrdiff_t diff = &cell - m_vector.data();

        if (
            diff < 0 ||
            static_cast<ptrdiff_t>(m_vector.size()) <= diff
        ) [[unlikely]] return false;
    
        RemoveUnsafe(
            m_vector.begin() + diff
        );

        return true;
    }

    template <class TFunction>
    size_t CellCollection::RemoveIf(
        TFunction func
    ) noexcept
    {
        size_t removed = 0;

        auto itr = m_vector.begin();
        auto end = m_vector.end();

        while (itr != end) {
            if (func(*itr)) {
                itr = m_vector.erase(itr);
                end = m_vector.end();
                ++removed;

                continue;
            }
            ++itr;
        }

        return removed;
    }

    constexpr void CellCollection::RemoveUnsafe(
        Cell& cell
    ) noexcept
    {
        RemoveUnsafe(
            m_vector.begin() + (&cell - m_vector.data())
        );
    }
}

#endif //!CELLSIM_CELLS_CELLCOLLECTION_HPP