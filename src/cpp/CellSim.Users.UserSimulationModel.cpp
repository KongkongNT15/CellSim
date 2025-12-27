#include "CellSim.Users.UserSimulationModel.hpp"
#include "impl/CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "impl/CellSim.Model.SimulationModelForceComputationArgs.hpp"
#include "impl/CellSim.Model.SimulationModelStepArgs.hpp"
#include "impl/CellSim.Molecular.MoleculeField.hpp"
#include "impl/CellSim.Numerics.Vector3T.hpp"

#include <nlohmann/json.hpp>

using namespace CellSim;
using namespace CellSim::CellAlgorithms;
using namespace CellSim::Cells;
using namespace CellSim::Model;
using namespace CellSim::Molecular;

using namespace nlohmann;

namespace CellSim::Users
{
    UserSimulationModel::UserSimulationModel()
    {
        // TODO: ここに処理を追加します
    }

    UserSimulationModel::~UserSimulationModel()
    {
        // TODO: ここに処理を追加します
    }

    void UserSimulationModel::BeforeAdvanceStep(
        [[maybe_unused]] const Simulation* sender,
        [[maybe_unused]] SimulationModelStepArgs args
    )
    {
        // TODO: ここに処理を追加します
    }

    Numerics::Vector3 UserSimulationModel::ComputeForceOnCell(
        [[maybe_unused]] const Simulation* sender,
        [[maybe_unused]] SimulationModelForceComputationArgs args
    ) const
    {
        // TODO: ここに処理を追加します

        return Numerics::Vector3();
    }

    void UserSimulationModel::InitializeCells(
        const Simulation* sender,
        Cells::CellCollection& cells
    )
    {
        // TODO: ユーザー定義の処理に変更します
        CellSimulationModel::InitializeCells(sender, cells);
    }

    void UserSimulationModel::OnAdvanceStep(
        [[maybe_unused]] const Simulation* sender,
        [[maybe_unused]] SimulationModelStepArgs args
    )
    {
        // TODO: ここに処理を追加します
    }

    bool UserSimulationModel::UseCellAlgorithm() const noexcept
    {
        // TODO: ComputeForceOnCell()で引数cellAlgorithmを使用する場合は'true'に置き換えます
        return false;
    }
}