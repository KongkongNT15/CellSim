#ifndef CELLSIM_SIMULATION_HPP
#define CELLSIM_SIMULATION_HPP

#include "base.hpp"
#include "CellSim.SimulationResultWriter.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Molecular.MoleculeField.hpp"
#include <vector>

#include <nlohmann/json.hpp>

namespace CellSim
{
    /// @brief シミュレーション実行クラス
    /// @attention このクラスはスレッドセーフではありません
    class Simulation final {
        private:

        /// @brief 最近アクセスしたインスタンスへのポインター
        /// @note 見つからない場合はnullptr
        static inline constinit const Simulation* s_current = nullptr;

        /// @brief 細胞リスト
        ::std::vector<Cells::Cell> m_cells;

        /// @brief 設定
        ::nlohmann::json m_config;

        /// @brief 複数スレッドによる処理をするかどうか
        bool m_enableMultithreading;

        /// @brief 分子空間リスト
        ::std::vector<Molecular::MoleculeField> m_molecules;

        /// @brief アルゴリズム側で相互作用を行うかどうか
        bool m_overrideInteraction;

        /// @brief アルゴリズム側で力を計算するかどうか
        bool m_overrideForceComputation;

        /// @brief 使用する高速化アルゴリズムへのポインター
        /// @note シミュレーションモデルのUserCellAlgorithm()がfalseの場合はnullptr
        CellAlgorithms::CellAlgorithm* m_pCellAlgorithm;

        /// @brief クラスタモデルで使うCellListインスタンスへのポインター
        CellAlgorithms::CellList* m_pCellList;

        /// @brief シミュレーションモデルインスタンスへのポインター
        Model::CellSimulationModel* m_pCellSimulationModel;

        /// @brief 結果を出力するクラス
        SimulationResultWriter m_writer;

        /// @brief 力を計算
        void m_applyForce();

        /// @brief 1step進める
        void m_advanceStep();

        /// @brief m_advanceStep()の前処理
        void m_beforeAdvanceStep();

        /// @brief 高速化アルゴリズムインスタンスの初期化
        void m_initializeCellAlgorithm();

        public:

        /// @brief 最近アクセスしたインスタンスへのポインター
        /// @note 見つからない場合はnullptr
        [[nodiscard]] static const Simulation* Current() noexcept;

        /// @brief 初期化
        Simulation(
            SimulationOption option,
            ::nlohmann::json config
        );

        Simulation(
            Simulation const&
        ) = delete;

        ~Simulation();

        Simulation& operator=(
            Simulation const&
        ) = delete;

        // プロパティ
        
        [[nodiscard]]
        constexpr const CellAlgorithms::CellAlgorithm*
        CellAlgorithmPtr() const noexcept;

        [[nodiscard]]
        constexpr const CellAlgorithms::CellList*
        CellListPtr() const noexcept;

        [[nodiscard]]
        constexpr const Model::CellSimulationModel*
        CellSimulationModelPtr() const noexcept;

        [[nodiscard]]
        constexpr ::std::vector<Cells::Cell> const&
        Cells() const noexcept;

        [[nodiscard]]
        constexpr ::std::vector<Molecular::MoleculeField> const&
        Molecules() const noexcept;

        // メソッド

        /// @brief シミュレーションを実行
        void Run();
    };
}

namespace CellSim
{
    inline const Simulation* Simulation::Current() noexcept
    {
        return s_current;
    }

    constexpr const CellAlgorithms::CellAlgorithm*
    Simulation::CellAlgorithmPtr() const noexcept
    {
        return m_pCellAlgorithm;
    }

    constexpr const CellAlgorithms::CellList*
    Simulation::CellListPtr() const noexcept
    {
        return m_pCellList;
    }

    constexpr const Model::CellSimulationModel*
    Simulation::CellSimulationModelPtr() const noexcept
    {
        return m_pCellSimulationModel;
    }

    constexpr ::std::vector<Cells::Cell> const&
    Simulation::Cells() const noexcept
    {
        return m_cells;
    }

    constexpr ::std::vector<Molecular::MoleculeField> const&
    Simulation::Molecules() const noexcept
    {
        return m_molecules;
    }
}

#endif //!CELLSIM_SIMULATION_HPP