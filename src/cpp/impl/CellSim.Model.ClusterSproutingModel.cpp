#include "CellSim.Model.ClusterSproutingModel.hpp"
#include "CellSim.Model.SimulationModelForceComputationArgs.hpp"
#include "CellSim.Model.SimulationModelStepArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellBehaviorPtr.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Numerics.Vector3T.hpp"
#include "CellSim.Settings.Config.Cell.hpp"
#include "CellSim.Settings.Config.Simulation.hpp"
#include "CellSim.Settings.Config.SimulationModel.ClusterSprouting.hpp"
#include "CellSim.Messages.hpp"

#include <numbers>
#include <random>
#include <stdexcept>

namespace CellSim::Model
{

    ClusterSproutingModel::Params::Params(
        size_t adhesionThreshold,
        double coefficientCd,
        double followerAttractionFactor,
        double globalAttractionFactor,
        double lambda,
        double leaderRepulsionFactor,
        double leaderRepulsionMaxDistance,
        double leaderRepulsionMinDistance
    )
        : AdhesionThreshold(adhesionThreshold)
        , CoefficientCd(coefficientCd)
        , FollowerAttractionFactor(followerAttractionFactor)
        , GlobalAttractionFactor(globalAttractionFactor)
        , Lambda(lambda)
        , LeaderRepulsionFactor(leaderRepulsionFactor)
        , LeaderRepulsionMaxDistance(leaderRepulsionMaxDistance)
        , LeaderRepulsionMinDistance(leaderRepulsionMinDistance)
        , LeaderRepulsionRange(leaderRepulsionMaxDistance - leaderRepulsionMinDistance)
    {
        if (coefficientCd < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'coefficientCd' must be greater than or equal to zero."
            );
        }
        if (followerAttractionFactor < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'followerAttractionFactor' must be greater than or equal to zero."
            );
        }
        if (globalAttractionFactor < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'globalAttractionFactor' must be greater than or equal to zero."
            );
        }
        if (lambda == 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'lambda' must be non-zero."
            );
        }
        if (leaderRepulsionFactor < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'leaderRepulsionFactor' must be greater than or equal to zero."
            );
        }
        if (leaderRepulsionMaxDistance < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'leaderRepulsionMaxDistance' must be greater than or equal to zero."
            );
        }
        if (leaderRepulsionMinDistance < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'leaderRepulsionMinDistance' must be greater than or equal to zero."
            );
        }
        
        if (LeaderRepulsionRange <= 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "'leaderRepulsionMinDistance' must be less than 'leaderRepulsionMaxDistance'."
            );
        }
    }

    ClusterSproutingModel::ClusterSproutingModel()
        : ClusterSproutingModel(Settings::Config::SimulationModel::ClusterSprouting::ContactDistance())
    {
    }

    ClusterSproutingModel::ClusterSproutingModel(
        double contactDistance
    )
        : m_contactDistance(contactDistance)
        , m_squareContactDistance(contactDistance * contactDistance)
    {
        if (contactDistance < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                "The parameter 'contactDistance' must be greater than or equal to zero."
            );
        }
    }

    void ClusterSproutingModel::BeforeAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs args
    )
    {
        args.Cells->DetachAll();

        for (auto itr = args.Cells->begin(), end = args.Cells->end(); itr != end; ++itr) {
            Cells::Cell& cell1 = *itr;

            if (!cell1.IsAlive()) continue;

            for (auto itr2 = itr + 1; itr2 != end; ++itr2) {
                Cells::Cell& cell2 = *itr2;

                if (!cell2.IsAlive()) continue;

                Numerics::Vector3 diff = cell1.Position() - cell2.Position();
                
                if (diff.SquareLength() < m_squareContactDistance) {
                    Cells::CellCollection::AttachUnsafe(
                        cell1,
                        cell2
                    );
                }
            }
        }
    }

    Numerics::Vector3 ClusterSproutingModel::ComputeForceOnCell(
        const Simulation*,
        SimulationModelForceComputationArgs args
    ) const
    {
        Numerics::Vector3 force1;
        Numerics::Vector3 force2;
        Numerics::Vector3 force3;
        Numerics::Vector3 force4;
        Cells::CellInfo info{ *args.Target };
        Params params = *static_cast<Params*>(info.Type.Params());

        size_t attachedCellCount = args.Target->AttachedCellCount();

        for (const Cells::Cell* pCell : args.Target->AttachedCells()) {
            Numerics::Vector3 diff = info.Position - pCell->Position();
            double dist = diff.Length();

            if (attachedCellCount <= params.AdhesionThreshold) {
                // 接着している細胞から離れようとする
                force1 += diff * ((params.LeaderRepulsionMaxDistance - dist) / (params.LeaderRepulsionMaxDistance * dist));
            }
            else {
                double v = dist - params.LeaderRepulsionMinDistance;

                // 近すぎると何も起こらない
                if (v > 0.0) {
                    force2 -= diff * (v / (dist * params.LeaderRepulsionRange));

                    continue;
                }
            }

            // 近すぎると反発力が発生
            if (dist < m_contactDistance) {
                force3 += diff * ((m_contactDistance - dist) / (m_contactDistance * dist));
            }
        }

        CELLSIM_CELLALGORITHMS_CELLALGORITHM_ITERATE(
            cellInfo,
            args.CellAlgorithm,
            args.Target,
            args.AffectedCells,
            args.Fields,
            {
                Numerics::Vector3 diff = info.Position - cellInfo.Position;
                double dist = diff.Length();

                // すべての細胞に働く力
                force4 -= diff * (::exp(-dist / params.Lambda) / dist);
            }
        )

        return params.LeaderRepulsionFactor * force1 + params.FollowerAttractionFactor * force2 + params.CoefficientCd * force3 + params.GlobalAttractionFactor * force4;
    }

    void ClusterSproutingModel::OnAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs
    )
    {
    }
}