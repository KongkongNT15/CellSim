#ifndef CELLSIM_CELLALGORITHMS_CELLLIST_HPP
#define CELLSIM_CELLALGORITHMS_CELLLIST_HPP

#include "base.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Containers.Span3.hpp"

#include <vector>

namespace CellSim::CellAlgorithms
{
    /// @brief Cell-List
    class CellList : public CellAlgorithm {
        private:

        ::std::vector<::std::vector<Cells::CellInfo>> m_cellField;

        bool m_enable2dMode;

        size_t m_gridCountX;
        size_t m_gridCountY;
        size_t m_gridCountZ;

        double m_gridLengthX;
        double m_gridLengthY;
        double m_gridLengthZ;

        double m_reverseGridLengthX;
        double m_reverseGridLengthY;
        double m_reverseGridLengthZ;

        size_t m_searchGridCountX;
        size_t m_searchGridCountY;
        size_t m_searchGridCountZ;
        double m_searchRadius;

        /// @brief m_span[Z][Y][X]
        Containers::Span3<::std::vector<Cells::CellInfo>> m_span;

        double m_squareSeachRadius;

        public:
        
        CellList();

        CellList(
            size_t gridCount,
            bool enable2dMode,
            double searchRadius
        );

        CellList(CellList const&) = delete;

        CellList& operator=(CellList const&) = delete;

        void BeforeAdvanceStep(
            const Simulation* sender,
            CellAlgorithmStepArgs args
        ) override;

        ::std::vector<Cells::CellInfo> GetAffectableCellInfos(
            const Model::CellSimulationModel* sender,
            CellAlgorithmAffectableCellQueryArgs args
        ) const override;

        constexpr bool HasMultithreadingSupport() const noexcept override;

        /// @brief 2津の細胞間の距離がSearchRadius()以内であるかを判定
        /// @param cell1 
        /// @param cell2 
        /// @return 
        [[nodiscard]] constexpr bool IsWithinSearchRadius(
            Cells::Cell const& cell1,
            Cells::Cell const& cell2
        ) const noexcept;

        [[nodiscard]] constexpr bool IsWithinSearchRadius(
            Cells::Cell const& cell1,
            Numerics::Vector3 position2
        ) const noexcept;

        [[nodiscard]] constexpr bool IsWithinSearchRadius(
            Numerics::Vector3 position1,
            Cells::Cell const& cell2
        ) const noexcept;

        [[nodiscard]] constexpr bool IsWithinSearchRadius(
            Numerics::Vector3 position1,
            Numerics::Vector3 position2
        ) const noexcept;

        Threading::Generator<Cells::CellInfo> IterateAffectableCellInfos(
            const Model::CellSimulationModel* sender,
            CellAlgorithmAffectableCellQueryArgs args
        ) const override;

        void OnAdvanceStep(
            const Simulation* sender,
            CellAlgorithmStepArgs args
        ) override;

        void ResetCells() noexcept;

        [[nodiscard]] constexpr double SearchRadius() const noexcept;

        void SetCells(
            Cells::CellCollection const& cells
        ) noexcept;
        
        [[nodiscard]] Numerics::GridPosition3 ToGridPosition3(
            Cells::Cell const& cell
        ) const noexcept;

        [[nodiscard]] Numerics::GridPosition3 ToGridPosition3(
            Numerics::Vector3 position
        ) const noexcept;

    };
}

namespace CellSim::CellAlgorithms
{
    constexpr bool CellList::HasMultithreadingSupport() const noexcept
    {
        return true;
    }

    constexpr bool CellList::IsWithinSearchRadius(
        Cells::Cell const& cell1,
        Cells::Cell const& cell2
    ) const noexcept
    {
        return IsWithinSearchRadius(
            cell1.Position(),
            cell2.Position()
        );
    }

    constexpr bool CellList::IsWithinSearchRadius(
        Cells::Cell const& cell1,
        Numerics::Vector3 position2
    ) const noexcept
    {
        return IsWithinSearchRadius(
            cell1.Position(),
            position2
        );
    }

    constexpr bool CellList::IsWithinSearchRadius(
        Numerics::Vector3 position1,
        Cells::Cell const& cell2
    ) const noexcept
    {
        return IsWithinSearchRadius(
            position1,
            cell2.Position()
        );
    }

    constexpr bool CellList::IsWithinSearchRadius(
        Numerics::Vector3 position1,
        Numerics::Vector3 position2
    ) const noexcept
    {
        return (position1 - position2).SquareLength() < m_squareSeachRadius;
    }

    constexpr double CellList::SearchRadius() const noexcept
    {
        return m_searchRadius;
    }

    inline Numerics::GridPosition3 CellList::ToGridPosition3(
        Cells::Cell const& cell
    ) const noexcept
    {
        return ToGridPosition3(cell.Position());
    }
}

#endif //!CELLSIM_CELLALGORITHMS_CELLLIST_HPP