#include "CellSim.Molecular.MoleculeField.hpp"
#include "CellSim.Molecular.MoleculeBehavior.hpp"
#include "CellSim.Molecular.MoleculeBehaviorStepArgs.hpp"
#include "CellSim.Molecular.MoleculeDiffusionArgs.hpp"
#include "CellSim.Molecular.MoleculeInitializationArgs.hpp"
#include "CellSim.Settings.Config.Simulation.hpp"
#include "CellSim.Messages.hpp"

#include <stdexcept>

namespace CellSim::Molecular
{
    MoleculeField::MoleculeField(
        size_t gridCount,
        bool enable2dMode,
        MoleculeKind kind,
        ::CellSim::Molecular::BoundaryCondition boundaryCondition,
        InitialMoleculeDistribution distributionType,
        double moleculeAmount,
        MoleculeBehavior* pBehavior
    )
        : m_boundaryCondition(boundaryCondition)
        , m_concentrations()
        , m_pConcentration(
            new double[
                enable2dMode ? gridCount * gridCount : gridCount * gridCount * gridCount
            ]
        )
        , m_enable2dMode(enable2dMode)
        , m_gridCountX(gridCount)
        , m_gridCountY(gridCount)
        , m_gridCountZ(enable2dMode ? 1 : gridCount)
        , m_gridLengthX(
            Settings::Config::Simulation::FieldRadiusX() * 2.0 / (gridCount - 2)
        )
        , m_gridLengthY(
            Settings::Config::Simulation::FieldRadiusY() * 2.0 / (gridCount - 2)
        )
        , m_gridLengthZ(
            Settings::Config::Simulation::FieldRadiusZ() * 2.0 / (gridCount - 2)
        )
        , m_reverseGridLengthX(1.0 / m_gridLengthX)
        , m_reverseGridLengthY(1.0 / m_gridLengthY)
        , m_reverseGridLengthZ(1.0 / m_gridLengthZ)
        , m_kind(kind)
        , m_pBehavior(pBehavior)
    {
        if (gridCount < 3) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("Molecular.MoleculeField.MoleculeField.Error.gridCount")
            );
        }
        if (moleculeAmount < 0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("Molecular.MoleculeField.MoleculeField.Error.moleculeAmount")
            );
        }

        m_concentrations = Containers::Span3<double>(
            gridCount,
            gridCount,
            m_gridCountZ,
            m_pConcentration
        );

        if (pBehavior == nullptr) [[unlikely]] {
            throw ::std::invalid_argument("MoleculeBehavior* is nullptr.");
        }

        if (enable2dMode) {
            m_reverseGridLengthZ = 0;
        }

        pBehavior->SetBuffer(gridCount, enable2dMode, m_boundaryCondition);
        pBehavior->InitializeMolecules(
            this,
            {
                m_concentrations,
                distributionType,
                moleculeAmount
            }
        );
    }

    void MoleculeField::BeforeAdvanceStep(
        Cells::CellCollection const& cells
    )
    {
        m_pBehavior->BeforeAdvanceStep(this, { &cells });
    }

    void MoleculeField::Diffuse()
    {
        m_pBehavior->Diffuse(this, { m_concentrations });
    }

    void MoleculeField::OnAdvanceStep(
        Cells::CellCollection const& cells
    )
    {
        m_pBehavior->OnAdvanceStep(this, { &cells });
    }

    Numerics::GridPosition3 MoleculeField::ToGridPosition3(
        Numerics::Vector3 position
    ) const noexcept
    {
        double x = position.X + Settings::Config::Simulation::FieldRadius();

        double y = position.Y + Settings::Config::Simulation::FieldRadius();
        
        if (m_enable2dMode) {
            return Numerics::GridPosition3(
                // 境界部分を無視
                static_cast<int32_t>(x * m_reverseGridLengthX) + 1,
                static_cast<int32_t>(y * m_reverseGridLengthY) + 1,
                0
            );
        }
        else {
            double z = position.Z + Settings::Config::Simulation::FieldRadius();

            return Numerics::GridPosition3(
                // 境界部分を無視
                static_cast<int32_t>(x * m_reverseGridLengthX) + 1,
                static_cast<int32_t>(y * m_reverseGridLengthY) + 1,
                static_cast<int32_t>(z * m_reverseGridLengthZ) + 1
            );
        }
    }
}