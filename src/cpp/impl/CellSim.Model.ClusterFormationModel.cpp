#include "CellSim.Model.ClusterFormationModel.hpp"
#include "CellSim.Model.SimulationModelForceComputationArgs.hpp"
#include "CellSim.Model.SimulationModelStepArgs.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Numerics.Vector3T.hpp"

#include <stdexcept>

namespace CellSim::Model
{
    Numerics::Vector3 ClusterFormationModel::m_computeRemoteForce(
        Numerics::Vector3 diff,
        double dist,
        double targetMass,
        double cellMass,
        double reverseLambda
    ) const noexcept
    {
        const double mass = targetMass * cellMass;

        return (
            -mass *
            ::exp(-dist * reverseLambda) /
            dist
        ) * diff;
    }

    Numerics::Vector3 ClusterFormationModel::m_computeVolumeExclusion(
        Numerics::Vector3 diff,
        double dist,
        double targetRadius,
        double cellRadius
    ) const noexcept
    {
        const double sumRadius = targetRadius + cellRadius;

        if (dist < sumRadius) {

            double tmp = 1.0 - dist / sumRadius;

            return diff * (tmp * tmp);
        }

        return Numerics::Vector3();
    }

    ClusterFormationModel::Params::Params(
        double adhesiveRepulsionFactor,
        double lambda,
        double remoteForceFactor
    )
        : AdhesiveRepulsionFactor(adhesiveRepulsionFactor)
        , Lambda(lambda)
        , ReverseLambda(1.0 / lambda)
        , RemoteForceFactor(remoteForceFactor)
    {
        if (adhesiveRepulsionFactor < 0.0) {
            [[unlikely]] throw ::std::invalid_argument(
                Messages::Get(
                    "Model.ClusterFormationModel.ClusterFormationModel.Error.adhesiveRepulsionFactor"
                )
            );
        }
        if (lambda == 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get(
                    "Model.ClusterFormationModel.ClusterFormationModel.Error.lambda"
                )
            );
        }
        if (remoteForceFactor < 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get(
                    "Model.ClusterFormationModel.ClusterFormationModel.Error.remoteForceFactor"
                )
            );
        }
    }

    void ClusterFormationModel::BeforeAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs
    )
    {
    }

    Numerics::Vector3 ClusterFormationModel::ComputeForceOnCell(
        const Simulation*,
        SimulationModelForceComputationArgs args
    ) const
    {
        Cells::CellInfo info{ *args.Target };

        Params params = *static_cast<Params*>(info.Type.Params());
        
        if (info.IsAlive) {
            Numerics::Vector3 vec1;
            Numerics::Vector3 vec2;

            CELLSIM_CELLALGORITHMS_CELLALGORITHM_ITERATE(
                cellInfo,
                args.CellAlgorithm,
                args.Target,
                args.AffectedCells,
                args.Fields,
                {
                    Numerics::Vector3 diff = info.Position - cellInfo.Position;
                    double dist = diff.Length();
                    if (cellInfo.IsAlive) {
                        vec1 += m_computeRemoteForce(
                            diff,
                            dist,
                            info.Mass,
                            cellInfo.Mass,
                            params.ReverseLambda
                        );
                    }
                    vec2 += m_computeVolumeExclusion(
                        diff,
                        dist,
                        info.Radius,
                        cellInfo.Radius
                    );
                }
            )

            return params.RemoteForceFactor * vec1 + params.AdhesiveRepulsionFactor * vec2;
        }
        
        Numerics::Vector3 vec;

        CELLSIM_CELLALGORITHMS_CELLALGORITHM_ITERATE(
            cellInfo,
            args.CellAlgorithm,
            args.Target,
            args.AffectedCells,
            args.Fields,
            {
                Numerics::Vector3 diff = info.Position - cellInfo.Position;
                double dist = diff.Length();
                vec += m_computeVolumeExclusion(diff, dist, info.Radius, cellInfo.Radius);
            }
        )

        return params.AdhesiveRepulsionFactor * vec;
    }

    void ClusterFormationModel::OnAdvanceStep(
        const Simulation*,
        SimulationModelStepArgs
    )
    {
    }
}