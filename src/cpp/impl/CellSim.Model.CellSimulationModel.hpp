#ifndef CELLSIM_MODEL_SIMULATIONMODEL_HPP
#define CELLSIM_MODEL_SIMULATIONMODEL_HPP

#include "base.hpp"

#include <vector>

namespace CellSim::Model
{
    /// @brief 力学モデル
    class CellSimulationModel {
        public:

        /// @brief パラメータ
        class Params {
            protected:
            Params() = default;
            Params(Params const&) = default;
            Params(Params&&) = default;

            Params& operator=(Params const&) = default;
            Params& operator=(Params&&) = default;
            public:
            virtual ~Params() = default;
        };

        static void InitializeFromCsv(
            const Simulation* sender,
            Cells::CellCollection& cells,
            const char* filePath
        );

        virtual ~CellSimulationModel() = default;

        /// @brief CellSimulationTypeからインスタンスを作成
        /// @param type 種類
        /// @return インスタンス
        [[nodiscard]] static CellSimulationModel* FromType(
            CellSimulationType type
        );

        /// @brief 前処理
        /// @param sender このメンバー関数を呼んだインスタンス
        /// @param args 処理に必要な情報
        virtual void BeforeAdvanceStep(
            const Simulation* sender,
            SimulationModelStepArgs args
        ) = 0;

        /// @brief ターゲットの細胞かける力を計算
        /// @param sender このメンバー関数を呼んだインスタンス
        /// @param args 処理に必要な情報
        /// @return 計算結果
        [[nodiscard]] virtual Numerics::Vector3 ComputeForceOnCell(
            const Simulation* sender,
            SimulationModelForceComputationArgs args
        ) const = 0;

        /// @brief リストに細胞を格納
        /// @param sender このメンバー関数を呼んだインスタンス
        /// @param cells リスト
        virtual void InitializeCells(
            const Simulation* sender,
            Cells::CellCollection& cells
        );

        /// @brief 後処理
        /// @param sender このメンバー関数を呼んだインスタンス
        /// @param args 処理に必要な情報
        virtual void OnAdvanceStep(
            const Simulation* sender,
            SimulationModelStepArgs args
        ) = 0;

        /// @brief モデルでCellAlgorithmを使うかどうか
        [[nodiscard]] virtual bool UseCellAlgorithm() const noexcept = 0;
    };
}

#endif //!CELLSIM_MODEL_SIMULATIONMODEL_HPP