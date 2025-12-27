#include "CellSim.CellAlgorithms.CellList.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmStepArgs.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.Settings.Config.Simulation.hpp"
#include "CellSim.Settings.Config.CellAlgorithm.CellList.hpp"
#include "CellSim.Threading.Generator.hpp"

#include <stdexcept>

namespace CellSim::CellAlgorithms
{
    CellList::CellList()
        : CellList(
            Settings::Config::CellAlgorithm::CellList::GridCount(),
            Settings::Config::Simulation::Enable2dMode(),
            Settings::Config::CellAlgorithm::CellList::SearchRadius()
        )
    {
    }

    CellList::CellList(
        size_t gridCount,
        bool enable2dMode,
        double searchRadius
    )
        : m_cellField(enable2dMode ? (gridCount * gridCount) : (gridCount * gridCount * gridCount))
        , m_enable2dMode(enable2dMode)
        , m_gridCountX(gridCount)
        , m_gridCountY(gridCount)
        , m_gridCountZ(enable2dMode ? 1 : gridCount)
        , m_gridLengthX(Settings::Config::Simulation::FieldRadius() * 2.0 / gridCount)
        , m_gridLengthY(Settings::Config::Simulation::FieldRadius() * 2.0 / gridCount)
        , m_gridLengthZ(Settings::Config::Simulation::FieldRadius() * 2.0 / m_gridCountZ)
        , m_reverseGridLengthX(1.0 / m_gridLengthX)
        , m_reverseGridLengthY(1.0 / m_gridLengthY)
        , m_reverseGridLengthZ(1.0 / m_gridLengthZ)
        , m_searchGridCountX(static_cast<size_t>(2.0 * searchRadius / m_gridLengthX))
        , m_searchGridCountY(static_cast<size_t>(2.0 * searchRadius / m_gridLengthY))
        , m_searchGridCountZ(static_cast<size_t>(2.0 * searchRadius / m_gridLengthZ))
        , m_searchRadius(searchRadius)
        , m_span(m_gridCountZ, gridCount, gridCount, m_cellField.data())
        , m_squareSeachRadius(searchRadius * searchRadius)
    {
        if (gridCount == 0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("CellAlgorithms.CellList.CellList.Error.gridCount")
            );
        }

        if (searchRadius < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("CellAlgorithms.CellList.CellList.Error.searchRadius")
            );
        }
        
        if (enable2dMode) {
            m_searchGridCountZ = 0;
            m_reverseGridLengthZ = 0;
        }
    }

    void CellList::BeforeAdvanceStep(
        const Simulation*,
        CellAlgorithmStepArgs args
    )
    {
        SetCells(*args.Cells);
    }

    ::std::vector<Cells::CellInfo> CellList::GetAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs args
    ) const
    {
        Numerics::Vector3 position = args.Target->Position();
        Numerics::GridPosition3 gridPosition = ToGridPosition3(position);
        
        ::std::vector<Cells::CellInfo> result;

        int32_t xMin = static_cast<int32_t>(gridPosition.X - m_searchGridCountX);
        int32_t yMin = static_cast<int32_t>(gridPosition.Y - m_searchGridCountY);
        int32_t zMin = static_cast<int32_t>(gridPosition.Z - m_searchGridCountZ);
        int32_t xMax = static_cast<int32_t>(gridPosition.X + m_searchGridCountX);
        int32_t yMax = static_cast<int32_t>(gridPosition.Y + m_searchGridCountY);
        int32_t zMax = static_cast<int32_t>(gridPosition.Z + m_searchGridCountZ);

        if (xMin < 0) xMin = 0;
        if (yMin < 0) yMin = 0;
        if (zMin < 0) zMin = 0;

        if (static_cast<size_t>(xMax) >= m_gridCountX) xMax = static_cast<int32_t>(m_gridCountX) - 1;
        if (static_cast<size_t>(yMax) >= m_gridCountY) yMax = static_cast<int32_t>(m_gridCountY) - 1;
        if (static_cast<size_t>(zMax) >= m_gridCountZ) zMax = static_cast<int32_t>(m_gridCountZ) - 1;

        for (size_t z = zMin; z <= zMax; z++) {
            auto span2 = m_span[z];
            for (size_t y = yMin; y <= yMax; y++) {
                auto span = span2[y];
                for (size_t x = xMin; x <= xMax; x++) {
                    for (Cells::CellInfo const& info : span[x]) {
                        if (info.Position == position) continue;

                        if (IsWithinSearchRadius(position, info.Position)) {
                            result.emplace_back(info);
                        }
                    }
                }
            }
        }

        return result;
    }

    Threading::Generator<Cells::CellInfo> CellList::IterateAffectableCellInfos(
        const Model::CellSimulationModel*,
        CellAlgorithmAffectableCellQueryArgs args
    ) const
    {
        Numerics::Vector3 position = args.Target->Position();
        Numerics::GridPosition3 gridPosition = ToGridPosition3(position);
        
        int32_t xMin = static_cast<int32_t>(gridPosition.X - m_searchGridCountX);
        int32_t yMin = static_cast<int32_t>(gridPosition.Y - m_searchGridCountY);
        int32_t zMin = static_cast<int32_t>(gridPosition.Z - m_searchGridCountZ);
        int32_t xMax = static_cast<int32_t>(gridPosition.X + m_searchGridCountX);
        int32_t yMax = static_cast<int32_t>(gridPosition.Y + m_searchGridCountY);
        int32_t zMax = static_cast<int32_t>(gridPosition.Z + m_searchGridCountZ);

        if (xMin < 0) xMin = 0;
        if (yMin < 0) yMin = 0;
        if (zMin < 0) zMin = 0;

        if (static_cast<size_t>(xMax) >= m_gridCountX) xMax = static_cast<int32_t>(m_gridCountX) - 1;
        if (static_cast<size_t>(yMax) >= m_gridCountY) yMax = static_cast<int32_t>(m_gridCountY) - 1;
        if (static_cast<size_t>(zMax) >= m_gridCountZ) zMax = static_cast<int32_t>(m_gridCountZ) - 1;

        for (size_t z = zMin; z <= zMax; z++) {
            auto span2 = m_span[z];
            for (size_t y = yMin; y <= yMax; y++) {
                auto span = span2[y];
                for (size_t x = xMin; x <= xMax; x++) {
                    for (Cells::CellInfo const& info : span[x]) {
                        if (info.Position == position) continue;

                        if (IsWithinSearchRadius(position, info.Position)) {
                            co_yield info;
                        }
                    }
                }
            }
        }

        co_return;
    }

    void CellList::OnAdvanceStep(
        const Simulation*,
        CellAlgorithmStepArgs
    )
    {
        ResetCells();
    }

    void CellList::ResetCells() noexcept
    {
        for (auto& vec : m_cellField) {
            vec.clear();
        }
    }

    void CellList::SetCells(
        Cells::CellCollection const& cells
    ) noexcept
    {
        for (Cells::Cell const& cell : cells) {
            Numerics::Vector3 position = cell.Position();

            double x = position.X + Settings::Config::Simulation::FieldRadius();

            if (x < 0.0) continue;

            double y = position.Y + Settings::Config::Simulation::FieldRadius();

            if (y < 0.0) continue;

            double z;

            if (m_enable2dMode) {
                z = 0.0;
            }
            else {
                z = position.Z + Settings::Config::Simulation::FieldRadius();
                if (z < 0.0) continue;
            }
            
            int32_t atX = static_cast<int32_t>(x * m_reverseGridLengthX);
            int32_t atY = static_cast<int32_t>(y * m_reverseGridLengthY);
            int32_t atZ = static_cast<int32_t>(z * m_reverseGridLengthZ);

            if (static_cast<size_t>(atX) >= m_gridCountX) continue;
            if (static_cast<size_t>(atY) >= m_gridCountY) continue;
            if (static_cast<size_t>(atZ) >= m_gridCountZ) continue;

            m_span.At(atZ, atY, atX).emplace_back(cell);
        }
    }

    Numerics::GridPosition3 CellList::ToGridPosition3(
        Numerics::Vector3 position
    ) const noexcept
    {
        double x = position.X + Settings::Config::Simulation::FieldRadius();

        double y = position.Y + Settings::Config::Simulation::FieldRadius();
        
        if (m_enable2dMode) {
            return Numerics::GridPosition3(
                static_cast<int32_t>(x * m_reverseGridLengthX),
                static_cast<int32_t>(y * m_reverseGridLengthY),
                0
            );
        }
        else {
            double z = position.Z + Settings::Config::Simulation::FieldRadius();

            return Numerics::GridPosition3(
                static_cast<int32_t>(x * m_reverseGridLengthX),
                static_cast<int32_t>(y * m_reverseGridLengthY),
                static_cast<int32_t>(z * m_reverseGridLengthZ)
            );
        }
        
    }

}