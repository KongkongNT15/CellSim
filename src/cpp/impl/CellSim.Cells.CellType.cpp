#include "CellSim.Cells.CellType.hpp"

#include <stdexcept>

namespace CellSim::Cells
{
    ::std::vector<CellType::s_data> CellType::s_names;
    constinit bool CellType::s_initialized = false;

    CellType CellType::AddName(
        ::std::string name,
        Graphics::Color color,
        Model::CellSimulationModel::Params* params
    )
    {
        // すでに名前が存在すればそのIdを返す
        for (size_t index = 0;index != s_names.size(); ++index) {
            if (s_names[index].Name == name) [[unlikely]] {
                throw ::std::invalid_argument(
                    "AddNameに既に存在する名前が指定されました"
                );
            }
        }

        s_names.emplace_back(
            ::std::move(name),
            color,
            params
        );

        return CellType(static_cast<uint32_t>(s_names.size() - 1));
    }

    CellType CellType::AddName(
        ::std::string_view name,
        Graphics::Color color,
        Model::CellSimulationModel::Params* params
    )
    {
        // すでに名前が存在すればそのIdを返す
        // と思っていたのか？
        for (s_data const& data : s_names) {
            if (data.Name == name) [[unlikely]] {
                throw ::std::invalid_argument(
                    "AddNameに既に存在する名前が指定されました"
                );
            }
        }

        s_names.emplace_back(
            ::std::string(name),
            color,
            params
        );

        return CellType(static_cast<uint32_t>(s_names.size() - 1));
    }

    CellType CellType::FromName(
        ::std::string_view name
    ) noexcept
    {
        // すでに名前が存在すればそのIdを返す
        for (size_t index = 0;index != s_names.size(); ++index) {
            if (s_names[index].Name == name) return CellType(static_cast<uint32_t>(index));
        }

        return Invalid();
    }

    bool CellType::Initialize()
    {
        if (s_initialized) return false;

        s_names.emplace_back(
            ::std::string("Invalid"),
            Graphics::Color{ 0, 0, 0, 0 },
            nullptr
        );

        s_initialized = true;

        return true;
    }

    void CellType::ResetUnsafe() noexcept
    {
        s_names.erase(
            s_names.begin() + 1,
            s_names.end()
        );
    }
}