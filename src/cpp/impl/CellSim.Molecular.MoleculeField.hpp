#ifndef CELLSIM_MOLECULAR_MOLECULEFILED_HPP
#define CELLSIM_MOLECULAR_MOLECULEFILED_HPP

#include "base.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Containers.Span3.hpp"
#include "CellSim.Molecular.MoleculeKind.hpp"
#include "CellSim.Numerics.Vector3T.hpp"

#include <vector>
#include <nlohmann/json_fwd.hpp>

namespace CellSim::Molecular
{
    /// @brief 分子空間
    class MoleculeField final {
        private:

        class DoublePtr {
            private:
            double* m_ptr;

            public:

            constexpr DoublePtr() noexcept : m_ptr(nullptr) {}
            constexpr DoublePtr(double* ptr) noexcept : m_ptr(ptr) {}
            constexpr DoublePtr(DoublePtr&& right) noexcept : m_ptr(right.m_ptr) { right.m_ptr = nullptr; }

            ~DoublePtr() noexcept { if (m_ptr != nullptr) delete[] m_ptr; }

            operator double*() const noexcept { return m_ptr; }

            DoublePtr& operator=(DoublePtr&& right) noexcept
            {
                if (this == &right) [[unlikely]] return *this;

                if (m_ptr != nullptr) delete[] m_ptr;
                m_ptr = right.m_ptr;
                right.m_ptr = nullptr;

                return *this;
            }
        };

        ::CellSim::Molecular::BoundaryCondition m_boundaryCondition;

        /// @brief 
        Containers::Span3<double> m_concentrations;

        DoublePtr m_pConcentration;

        bool m_enable2dMode;

        size_t m_gridCountX;
        size_t m_gridCountY;
        size_t m_gridCountZ;

        double m_gridLengthX;
        double m_gridLengthY;
        double m_gridLengthZ;

        /// @brief 分子の種類
        MoleculeKind m_kind;

        /// @brief ふるまい定義
        MoleculeBehavior* m_pBehavior;

        double m_reverseGridLengthX;
        double m_reverseGridLengthY;
        double m_reverseGridLengthZ;

        public:

        MoleculeField(
            size_t gridCount,
            bool enable2dMode,
            MoleculeKind kind,
            ::CellSim::Molecular::BoundaryCondition boundaryCondition,
            InitialMoleculeDistribution distributionType,
            double moleculeAmount,
            MoleculeBehavior* pBehavior
        );

        MoleculeField(MoleculeField const&) = delete;
        MoleculeField(MoleculeField&&) = default;

        MoleculeField& operator=(MoleculeField const&) = delete;
        MoleculeField& operator=(MoleculeField&&) = default;

        void BeforeAdvanceStep(
            Cells::CellCollection const& cells
        );

        [[nodiscard]] constexpr ::CellSim::Molecular::BoundaryCondition
        BoundaryCondition() const noexcept;

        [[nodiscard]] constexpr Containers::Span3<double>
        Concentrations() noexcept;

        [[nodiscard]] constexpr Containers::ReadOnlySpan3<double>
        Concentrations() const noexcept;

        /// @brief 分子を拡散させる
        void Diffuse();

        [[nodiscard]] constexpr bool Enable2dMode() const noexcept;

        /// @brief 境界を含むX方向のグリッドの数
        [[nodiscard]] constexpr size_t GridCountX() const noexcept;

        /// @brief 境界を含むY方向のグリッドの数
        [[nodiscard]] constexpr size_t GridCountY() const noexcept;

        /// @brief 境界を含むZ方向のグリッドの数
        [[nodiscard]] constexpr size_t GridCountZ() const noexcept;

        [[nodiscard]] constexpr double GridLength() const noexcept;

        [[nodiscard]] constexpr double GridLengthX() const noexcept;
        [[nodiscard]] constexpr double GridLengthY() const noexcept;
        [[nodiscard]] constexpr double GridLengthZ() const noexcept;

        /// @brief 分子の種類
        [[nodiscard]] constexpr MoleculeKind Kind() const noexcept;

        void OnAdvanceStep(Cells::CellCollection const& cells);

        [[nodiscard]] Numerics::GridPosition3 ToGridPosition3(
            Cells::Cell const& cell
        ) const noexcept;

        [[nodiscard]] Numerics::GridPosition3 ToGridPosition3(
            Numerics::Vector3 position
        ) const noexcept;
    };
}

namespace CellSim::Molecular
{
    constexpr ::CellSim::Molecular::BoundaryCondition
    MoleculeField::BoundaryCondition() const noexcept
    {
        return m_boundaryCondition;
    }

    constexpr Containers::Span3<double> MoleculeField::Concentrations() noexcept
    {
        return m_concentrations;
    }

    constexpr Containers::ReadOnlySpan3<double>
    MoleculeField::Concentrations() const noexcept
    {
        return Containers::ReadOnlySpan3<double>(
            m_gridCountX,
            m_gridCountY,
            m_gridCountZ,
            m_pConcentration
        );
    }

    constexpr bool MoleculeField::Enable2dMode() const noexcept
    {
        return m_enable2dMode;
    }

    constexpr size_t MoleculeField::GridCountX() const noexcept
    {
        return m_gridCountX;
    }

    constexpr size_t MoleculeField::GridCountY() const noexcept
    {
        return m_gridCountY;
    }

    constexpr size_t MoleculeField::GridCountZ() const noexcept
    {
        return m_gridCountZ;
    }

    constexpr double MoleculeField::GridLength() const noexcept
    {
        return m_gridLengthX;
    }

    constexpr double MoleculeField::GridLengthX() const noexcept
    {
        return m_gridLengthX;
    }

    constexpr double MoleculeField::GridLengthY() const noexcept
    {
        return m_gridLengthY;
    }

    constexpr double MoleculeField::GridLengthZ() const noexcept
    {
        return m_gridLengthZ;
    }

    constexpr MoleculeKind MoleculeField::Kind() const noexcept
    {
        return m_kind;
    }

    inline Numerics::GridPosition3
    MoleculeField::ToGridPosition3(
        Cells::Cell const& cell
    ) const noexcept
    {
        return ToGridPosition3(cell.Position());
    }
}

#endif //!CELLSIM_MOLECULAR_MOLECULEFILED_HPP