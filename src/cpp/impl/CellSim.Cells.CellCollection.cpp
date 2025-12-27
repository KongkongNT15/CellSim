#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Settings.Config.Cell.hpp"

#include <charconv>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string_view>

#include "CellSim.Messages.hpp"

namespace CellSim::Cells
{
    bool CellCollection::Detach(
        Cell& cell1,
        Cell& cell2
    ) noexcept
    {
        auto itr = cell1.m_attachedCells.begin();
        auto end = cell1.m_attachedCells.end();

        while (itr != end) {

            if (*itr == &cell2) {

                cell1.m_attachedCells.erase(itr);

                auto itr2 = cell2.m_attachedCells.begin();
                auto end2 = cell2.m_attachedCells.end();

                while (itr2 != end2) {
                    if (*itr2 == &cell1) {
                        cell2.m_attachedCells.erase(itr2);
                        break;
                    }

                    ++itr2;
                }

                return true;
            }

            ++itr;
        }

        return false;
    }

    CellCollection CellCollection::FromCsv(
            const char* filePath
        )
    {
        ::std::map<::std::string_view, Cells::CellBehaviorPtr> behaviorMap;
        ::std::vector<::std::pair<Cell*, ::std::vector<Cell::IdType>>> attachedCells;
        CellCollection collection;

        for (Cells::CellCreateInfo const& info : Settings::Config::Cell::Cells()) {
            behaviorMap[info.Type.Name()] = info.Behavior;
        }

        ::std::ifstream ifs(filePath);
        int c;
        ::std::string s;
        ::std::ifstream sin;

        if (!ifs) [[unlikely]] throw ::std::runtime_error(
            Messages::Get(
                "Cells.CellCollection.FromCsv.Error.FileError"
            )
        );

        // 1行目は無視
        while ((c = ifs.get()) != '\n' && c != EOF);

        while (ifs) {
            Cell::IdType id;
            double mass;
            double radius;
            Numerics::Vector3 position;
            Numerics::Vector3 velocity;
            Cells::CellBehaviorPtr behavior;
            CellType type;
            size_t attachedCellCount;
            int isAliveInt;

            auto getStringValue = [&] () {
                s.clear();
                while ((c = ifs.get()) != ',' && c != EOF) {
                    s.push_back(static_cast<char>(c));
                }
            };

            auto setValue = [&] (auto& value) {
                getStringValue();
                ::std::from_chars(
                    s.data(),
                    s.data() + s.size(),
                    value
                );
            };

            // Id
            setValue(id);

            // Type
            getStringValue();
            type = CellType::FromName(s);
            behavior = behaviorMap[s];

            // Position
            setValue(position.X);
            setValue(position.Y);
            setValue(position.Z);

            // Velocity
            setValue(velocity.X);
            setValue(velocity.Y);
            setValue(velocity.Z);

            // Radius
            setValue(radius);

            // Mass
            setValue(mass);

            // IsAlive
            setValue(isAliveInt);

            // AttachedCellCount
            setValue(attachedCellCount);

            Cell& cell = collection.Emplace(
                type,
                behavior,
                mass,
                radius,
                position
            );

            // 値を無理矢理変更
            cell.m_isAlive = static_cast<bool>(isAliveInt);
            cell.m_id = id;

            attachedCells.push_back(
                {
                    &cell,
                    {}
                }
            );

            auto& attachedCellIds = attachedCells.back().second;

            for (size_t i = 0; i < attachedCellCount; i++) {
                Cell::IdType attachedCellId;
                setValue(attachedCellId);

                attachedCellIds.push_back(attachedCellId);
            }

            // 開業文字を空回し
            (void)ifs.get();
        }

        // 
        for (auto& pair : attachedCells) {
            Cell& cell = *pair.first;
            
            for (Cell::IdType cellId : pair.second) {
                for (Cell& cell1 : collection) {
                    if (cell1.Id() == cellId) {
                        cell.m_attachedCells.push_back(&cell1);
                        break;
                    }
                }
            }
        }

        return collection;
    }

    void CellCollection::RemoveWithDetachUnsafe(
        ::std::vector<Cell>::iterator itr
    ) noexcept
    {
        ::puts("void CellCollection::RemoveWithDetachUnsafe()は実装されていません");

        ::exit(1);
    }
}