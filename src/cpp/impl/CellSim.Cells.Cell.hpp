#ifndef CELLSIM_CELLS_CELL_HPP
#define CELLSIM_CELLS_CELL_HPP

#include "base.hpp"
#include "CellSim.Cells.CellBehaviorPtr.hpp"
#include "CellSim.Cells.CellType.hpp"
#include "CellSim.Molecular.Molecule.hpp"
#include "CellSim.Numerics.Vector3T.hpp"

#include <map>
#include <span>
#include <string>
#include <vector>

#if CELLSIM_ENV_ARM64
    #define CELLSIM_CELLS_CELL_ALIGNAS 128
#else
    #define CELLSIM_CELLS_CELL_ALIGNAS 64
#endif

namespace CellSim::Cells
{
    /// @brief 細胞クラス
    class alignas(64) Cell final {
        friend CellCollection;
        public:

        using IdType = uint32_t;

        private:

        /// @brief ダミー細胞のID
        static constexpr IdType s_dummyId = static_cast<IdType>(-1);

        /// @brief 各細胞に割り振るID
        static IdType s_id;

        /// @brief 接着している細胞のリスト
        ::std::vector<Cell*> m_attachedCells;

        /// @brief ふるまい定義
        CellBehaviorPtr m_behaviorPtr;

        /// @brief 細胞が受けた力
        Numerics::Vector3 m_force;

        /// @brief 細胞内の分子の種類とその量
        ::std::map<Molecular::MoleculeKind, double> m_internalMolecules;
        
        /// @brief 細胞の質量
        double m_mass;

        /// @brief 極性ベクトル
        Numerics::Vector3 m_polarity;

        /// @brief 細胞の位置
        Numerics::Vector3 m_position;

        /// @brief 1step前に細胞が受けた力
        Numerics::Vector3 m_previusForce;

        /// @brief 細胞の半径
        double m_radius;

        /// @brief 細胞の種類
        CellType m_type;

        /// @brief 識別子
        IdType m_id;

        /// @brief この細胞が生きているかどうか
        bool m_isAlive;

        /// @brief ダミー用
        /// @param type 
        /// @param mass 
        /// @param radius 
        /// @param position 
        Cell(
            CellType type,
            bool isAlive,
            double mass,
            double radius,
            Numerics::Vector3 position
        );
        
        public:

        [[nodiscard]] static Cell CreateDummy(
            CellType type,
            bool isAlive,
            double mass,
            double radius,
            Numerics::Vector3 position
        ) noexcept;

        /// @brief 
        /// @param type 
        /// @param pBehavior 
        /// @param mass 
        /// @param radius 
        /// @param position 
        Cell(
            CellType type,
            CellBehaviorPtr pBehavior,
            double mass,
            double radius,
            Numerics::Vector3 position
        );

        Cell(Cell const&) = delete;
        Cell(Cell&&) noexcept = default;

        Cell& operator=(Cell const&) = delete;
        Cell& operator=(Cell&&) noexcept = default;

        // プロパティ

        /// @brief 接着している細胞のリスト
        [[nodiscard]] constexpr ::std::span<const Cell* const>
        AttachedCells() const noexcept;

        /// @brief 接着している細胞の数
        [[nodiscard]] constexpr size_t AttachedCellCount() const noexcept;

        /// @brief 細胞が受けた力
        [[nodiscard]] constexpr Numerics::Vector3 Force() const noexcept;

        /// @brief 識別子
        [[nodiscard]] constexpr IdType Id() const noexcept;

        /// @brief 細胞内の分子の種類とその量
        [[nodiscard]] constexpr ::std::map<Molecular::MoleculeKind, double> const&
        InternalMolecules() const noexcept;

        [[nodiscard]] constexpr bool IsAlive() const noexcept;

        /// @brief ダミー細胞かどうか
        [[nodiscard]] constexpr bool IsDummy() const noexcept;

        /// @brief 細胞の質量
        [[nodiscard]] constexpr double Mass() const noexcept;

        /// @brief 極性ベクトル
        [[nodiscard]] constexpr Numerics::Vector3 Polarity() const noexcept;

        /// @brief 細胞の位置
        [[nodiscard]] constexpr Numerics::Vector3 Position() const noexcept;

        /// @brief 細胞のX座標
        [[nodiscard]] constexpr double PositionX() const noexcept;

        /// @brief 細胞のY座標
        [[nodiscard]] constexpr double PositionY() const noexcept;

        /// @brief 細胞のZ座標
        [[nodiscard]] constexpr double PositionZ() const noexcept;

        /// @brief 1step前に細胞が受けた力
        [[nodiscard]] constexpr Numerics::Vector3 PreviusForce() const noexcept;

        /// @brief 1step前の細胞の速度
        [[nodiscard]] constexpr Numerics::Vector3 PreviusVelocity() const noexcept;

        /// @brief 細胞の半径
        [[nodiscard]] constexpr double Radius() const noexcept;

        [[nodiscard]] bool ShouldDivideThisStep() const noexcept;

        /// @brief 細胞の種類
        [[nodiscard]] constexpr CellType Type() const noexcept;

        /// @brief 細胞の速度
        [[nodiscard]] constexpr Numerics::Vector3 Velocity() const noexcept;

        // メソッド

        /// @brief 接着
        /// @param cell 接着する細胞
        void AdhereUnsafe(
            Cell& cell
        );

        /// @brief 分子を追加
        /// @param kind 分子の種類
        /// @return 分子を追加したかどうか
        /// @note 初期状態では分子の数はゼロ
        bool AppendMolecule(
            Molecular::MoleculeKind kind
        );

        /// @brief 細胞に力を加える
        /// @param force 加える力
        constexpr void ApplyForce(
            Numerics::Vector3 force
        ) noexcept;
        
        /// @brief 結合
        /// @param c 結合する細胞
        /// @note 結合すると引数に与えたcは無効になります
        void Combine(
            Cell& c
        ) noexcept;

        /// @brief 細胞死
        constexpr void Die() noexcept;

        /// @brief 分裂
        /// @return 分裂したもう1つの細胞
        [[nodiscard]] Cell Divide();

        constexpr bool EraseAttachedCell(
            Cell const& cell
        ) noexcept;

        /// @brief 細胞が成長
        void Grow();

        [[nodiscard]] bool IsAdheringTo(
            Cell const& cell
        ) const noexcept;

        /// @brief 細胞が移動
        void Move() noexcept;

        /// @brief 細胞内と空間中の分子の状態を更新
        /// @param field 分子空間
        void ProcessMolecules(
            Molecular::MoleculeField& field
        );

        /// @brief 細胞内と空間中の分子の状態を更新
        /// @param fields 分子空間リスト
        void ProcessMolecules(
            ::std::vector<Molecular::MoleculeField>& fields
        );

        /// @brief 細胞にかかっている力をゼロにする
        constexpr void ResetForce() noexcept;

        void SenseMolecules(
            Molecular::MoleculeField const& field
        );

        void SenseMolecules(
            ::std::vector<Molecular::MoleculeField> const& fields
        );

    };

    [[nodiscard]] constexpr bool operator==(
        Cell const& left,
        Cell const& right
    ) noexcept;

    [[nodiscard]] constexpr bool operator!=(
        Cell const& left,
        Cell const& right
    ) noexcept;
}

namespace CellSim::Cells
{
    inline Cell::Cell(
        CellType type,
        bool isAlive,
        double mass,
        double radius,
        Numerics::Vector3 position
    )
        : m_attachedCells()
        , m_behaviorPtr(CellBehaviorPtr::Null())
        , m_force()
        , m_id(s_dummyId)
        , m_internalMolecules()
        , m_isAlive(isAlive)
        , m_mass(mass)
        , m_polarity()
        , m_position(position)
        , m_previusForce()
        , m_radius(radius)
        , m_type(type)
    {
    }

    inline Cell Cell::CreateDummy(
        CellType type,
        bool isAlive,
        double mass,
        double radius,
        Numerics::Vector3 position
    ) noexcept
    {
        return Cell(
            type,
            isAlive,
            mass,
            radius,
            position
        );
    }

    constexpr ::std::span<const Cell* const>
    Cell::AttachedCells() const noexcept
    {
        auto begin = static_cast<const Cell* const*>(
            m_attachedCells.data()
        );

        auto end = begin + m_attachedCells.size();
        
        return ::std::span<const Cell* const>(
            begin,
            end
        );
    }

    constexpr size_t Cell::AttachedCellCount() const noexcept
    {
        return m_attachedCells.size();
    }

    constexpr Numerics::Vector3 Cell::Force() const noexcept
    {
        return m_force;
    }

    constexpr Cell::IdType Cell::Id() const noexcept
    {
        return m_id;
    }

    constexpr ::std::map<Molecular::MoleculeKind, double> const&
    Cell::InternalMolecules() const noexcept
    {
        return m_internalMolecules;
    }

    constexpr bool Cell::IsAlive() const noexcept
    {
        return m_isAlive;
    }

    constexpr bool Cell::IsDummy() const noexcept
    {
        return m_id == s_dummyId;
    }

    constexpr double Cell::Mass() const noexcept
    {
        return m_mass;
    }

    constexpr Numerics::Vector3 Cell::Polarity() const noexcept
    {
        return m_polarity;
    }

    constexpr Numerics::Vector3 Cell::Position() const noexcept
    {
        return m_position;
    }

    constexpr double Cell::PositionX() const noexcept
    {
        return m_position.X;
    }

    constexpr double Cell::PositionY() const noexcept
    {
        return m_position.Y;
    }

    constexpr double Cell::PositionZ() const noexcept
    {
        return m_position.Z;
    }

    constexpr Numerics::Vector3 Cell::PreviusForce() const noexcept
    {
        return m_previusForce;
    }

    constexpr Numerics::Vector3 Cell::PreviusVelocity() const noexcept
    {
        return m_previusForce / m_mass;
    }

    constexpr double Cell::Radius() const noexcept
    {
        return m_radius;
    }

    constexpr CellType Cell::Type() const noexcept
    {
        return m_type;
    }

    constexpr Numerics::Vector3 Cell::Velocity() const noexcept
    {
        return m_force / m_mass;
    }

    inline bool Cell::AppendMolecule(
        Molecular::MoleculeKind kind
    )
    {
        return m_internalMolecules.try_emplace(kind, 0).second;
    }

    constexpr void Cell::ApplyForce(
        Numerics::Vector3 force
    ) noexcept
    {
        m_force += force;
    }

    inline void Cell::AdhereUnsafe(
        Cell& cell
    )
    {
        m_attachedCells.push_back(&cell);
    }

    constexpr void Cell::Die() noexcept
    {
        m_isAlive = false;
    }

    constexpr bool Cell::EraseAttachedCell(
        Cell const& cell
    ) noexcept
    {
        for (auto itr = m_attachedCells.begin(), end = m_attachedCells.end(); itr != end; ++itr) {
            if (*itr == &cell) {
                m_attachedCells.erase(itr);

                return true;
            }
        }

        return false;
    }

    inline bool Cell::IsAdheringTo(
        Cell const& cell
    ) const noexcept
    {
        const Cell* pCell = &cell;

        for (const Cell* pAttached : m_attachedCells) {
            if (pAttached == pCell) return true;
        }

        return false;
    }

    constexpr void Cell::ResetForce() noexcept
    {
        m_previusForce = m_force;
        m_force = Numerics::Vector3::Zero();
    }

    constexpr bool operator==(
        Cell const& left,
        Cell const& right
    ) noexcept
    {
        return &left == &right;
    }

    constexpr bool operator!=(
        Cell const& left,
        Cell const& right
    ) noexcept
    {
        return &left != &right;
    }
}

#endif //!CELLSIM_CELLS_CELL_HPP