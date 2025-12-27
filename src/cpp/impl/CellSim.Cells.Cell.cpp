#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellDivisionResult.hpp"
#include "CellSim.Cells.CellGrowthResult.hpp"
#include "CellSim.Cells.CellMoleculeSensingArgs.hpp"
#include "CellSim.Cells.MolecularProcessArgs.hpp"
#include "CellSim.Cells.MolecularProcessResult.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.Molecular.Molecule.hpp"
#include "CellSim.Molecular.MoleculeInfo.hpp"
#include "CellSim.Molecular.MoleculeField.hpp"
#include "CellSim.Settings.Config.Simulation.hpp"

#include <stdexcept>

namespace CellSim::Cells
{
    constinit Cell::IdType Cell::s_id = 0;

    Cell::Cell(
        CellType type,
        CellBehaviorPtr pBehavior,
        double mass,
        double radius,
        Numerics::Vector3 position
    )
        : m_attachedCells()
        , m_behaviorPtr(::std::move(pBehavior))
        , m_force()
        , m_internalMolecules()
        , m_mass(mass)
        , m_polarity()
        , m_position(position)
        , m_previusForce()
        , m_radius(radius)
        , m_type(type)
        , m_id(s_id)
        , m_isAlive(true)
    {
        if (mass <= 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("Cells.Cell.Cell.Error.mass")
            );
        }
        if (radius <= 0.0) [[unlikely]] {
            throw ::std::invalid_argument(
                Messages::Get("Cells.Cell.Cell.Error.radius")
            );
        }

        ++s_id;
    }

    bool Cell::ShouldDivideThisStep() const noexcept
    {
        return m_behaviorPtr->ShouldDivideThisStep(this);
    }

    void Cell::Combine(Cell& c) noexcept
    {
        m_radius = ::cbrt(m_radius * m_radius * m_radius + c.m_radius * c.m_radius * c.m_radius);
        m_position = (m_position * m_mass + c.m_position * c.m_mass) / (m_mass + c.m_mass);
        m_mass += c.m_mass;

        c.m_type = CellType::Invalid();
    }

    Cell Cell::Divide()
    {
        CellDivisionResult result = m_behaviorPtr->ComputeDivisionOutcome(this);

        m_mass = result.OriginalDaughter.NewMass;
        m_position = result.OriginalDaughter.NewPosition;
        m_radius = result.OriginalDaughter.NewRadius;

        Cell newDaughter(
            m_type,
            m_behaviorPtr,
            result.NewDaughter.NewMass,
            result.NewDaughter.NewRadius,
            result.NewDaughter.NewPosition
        );

        // 分子の量を半分にしてコピー
        for (auto& pair : m_internalMolecules) {
            pair.second /= 2;
        }
        newDaughter.m_internalMolecules = m_internalMolecules;

        return newDaughter;
    }

    void Cell::Grow()
    {
        CellGrowthResult result = m_behaviorPtr->ComputeGrowth(this);

        m_radius = result.NewRadius;
        m_mass = result.NewMass;
    }

    void Cell::Move() noexcept
    {
        m_position += m_force * (Settings::Config::Simulation::DeltaTime() / m_mass);
    }

    void Cell::ProcessMolecules(Molecular::MoleculeField& field)
    {
        Numerics::GridPosition3 position3 = field.ToGridPosition3(m_position);

        Molecular::MoleculeKind kind = field.Kind();

        auto& intracellularAmount = m_internalMolecules[kind];
        auto& extracellularAmount = field.Concentrations().At(position3.X, position3.Y, position3.Z);

        MolecularProcessResult result = m_behaviorPtr->ComputeMolecularProcess(
            this,
            {
                extracellularAmount,
                &field,
                intracellularAmount,
                kind,
                position3
            }
        );

        intracellularAmount += result.IntracellularChange;
        extracellularAmount += result.ExtracellularChange;
    }

    void Cell::ProcessMolecules(
        ::std::vector<Molecular::MoleculeField>& fields
    )
    {
        for (Molecular::MoleculeField& field : fields) {
            ProcessMolecules(field);
        }
    }

    void Cell::SenseMolecules(
        Molecular::MoleculeField const& field
    )
    {
        m_force += m_behaviorPtr->OnSenseMolecules(this, { &field });
    }

    void Cell::SenseMolecules(
        ::std::vector<Molecular::MoleculeField> const& fields
    )
    {
        for (Molecular::MoleculeField const& field : fields) {
            SenseMolecules(field);
        }
    }
}