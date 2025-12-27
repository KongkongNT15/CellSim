#ifndef CELLSIM_SETTINGS_CONFIG_CELL_HPP
#define CELLSIM_SETTINGS_CONFIG_CELL_HPP

#include "base.hpp"
#include "CellSim.Cells.CellCreateInfo.hpp"
#include "CellSim.Cells.CellBehaviorType.hpp"
#include "CellSim.Cells.CellType.hpp"
#include "CellSim.Settings.Config.hpp"

#include <nlohmann/json_fwd.hpp>

#include <vector>

namespace CellSim::Settings
{
    /// @brief 細胞設定
    class Config::Cell final {
        private:
        static inline bool s_enableGrowth;
        static inline double s_initialPlacementRadius;
        static inline bool s_isSensitiveToMolecules;
        static inline uint32_t s_initialPlacementSeed;
        static inline uint64_t s_totalCellCount;

        static inline ::std::vector<Cells::CellCreateInfo> s_cells;

        public:

        CELLSIM_STATIC_CLASS(Cell);

        // プロパティ

        [[nodiscard]]
        static ::std::vector<Cells::CellCreateInfo> const& Cells() noexcept;

        /// @brief 細胞の成長・分裂を有効にするかどうか
        [[nodiscard]]
        static bool EnableGrowth() noexcept;

        /// @brief 細胞が配置される円・球の半径
        [[nodiscard]]
        static double InitialPlacementRadius() noexcept;

        /// @brief 細胞の初期配置を決めるシード値
        [[nodiscard]]
        static uint32_t InitialPlacementSeed() noexcept;

        /// @brief 細胞が周囲の分子の影響を受けるかどうか
        [[nodiscard]]
        static bool IsSensitiveToMolecules() noexcept;

        /// @brief 細胞の総数
        [[nodiscard]]
        static uint64_t TotalCellCount() noexcept;

        // メソッド

        static void Load(
            ::nlohmann::json& config
        );
    };
}

namespace CellSim::Settings
{
    inline ::std::vector<Cells::CellCreateInfo> const&
    Config::Cell::Cells() noexcept
    {
        return s_cells;
    }

    inline bool Config::Cell::EnableGrowth() noexcept
    {
        return s_enableGrowth;
    }

    inline double Config::Cell::InitialPlacementRadius() noexcept
    {
        return s_initialPlacementRadius;
    }

    inline uint32_t Config::Cell::InitialPlacementSeed() noexcept
    {
        return s_initialPlacementSeed;
    }

    inline bool Config::Cell::IsSensitiveToMolecules() noexcept
    {
        return s_isSensitiveToMolecules;
    }

    inline uint64_t Config::Cell::TotalCellCount() noexcept
    {
        return s_totalCellCount;
    }
}

#endif //!CELLSIM_SETTINGS_CONFIG_CELL_HPP