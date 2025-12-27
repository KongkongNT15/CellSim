#ifndef CELLSIM_USERS_USERCELLSIMULATIONMODEL_HPP
#define CELLSIM_USERS_USERCELLSIMULATIONMODEL_HPP

#include "impl/base.hpp"
#include "impl/CellSim.Model.CellSimulationModel.hpp"

namespace CellSim::Users
{
    /// @brief ユーザー定義力学モデル
    class UserSimulationModel : public Model::CellSimulationModel {
        public:

        class Params : public Model::CellSimulationModel::Params {
            public:

            // ここに細胞の種類ごとのパラメータを追加します
        };

        UserSimulationModel();
        ~UserSimulationModel();

        void BeforeAdvanceStep(
            const Simulation* sender,
            Model::SimulationModelStepArgs args
        ) override;

        Numerics::Vector3 ComputeForceOnCell(
            const Simulation* sender,
            Model::SimulationModelForceComputationArgs args
        ) const override;

        void InitializeCells(
            const Simulation* sender,
            Cells::CellCollection& cells
        ) override;

        void OnAdvanceStep(
            const Simulation* sender,
            Model::SimulationModelStepArgs args
        ) override;

        bool UseCellAlgorithm() const noexcept override;
    };
}

#endif //!CELLSIM_USERS_USERCELLSIMULATIONMODEL_HPP