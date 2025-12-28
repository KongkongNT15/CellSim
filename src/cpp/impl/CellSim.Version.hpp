#ifndef CELLSIM_VERSIONS_HPP
#define CELLSIM_VERSIONS_HPP

#include "base.hpp"

namespace CellSim
{
    /// @brief CellSimのバージョン
    class Version final {
        private:

        static constexpr const char s_extension[] = "Alpha 2";

        public:

        CELLSIM_STATIC_CLASS(Version);

        [[nodiscard]] static constexpr int Major() noexcept;
        [[nodiscard]] static constexpr int Minor() noexcept;
        [[nodiscard]] static constexpr int Patch() noexcept;
        [[nodiscard]] static constexpr int Build() noexcept;
        [[nodiscard]] static constexpr const char* Extension() noexcept;
    };
}

namespace CellSim
{
    constexpr int Version::Major() noexcept
    {
        return 2;
    }

    constexpr int Version::Minor() noexcept
    {
        return 2;
    }

    constexpr int Version::Patch() noexcept
    {
        return 0;
    }

    constexpr int Version::Build() noexcept
    {
        return 20;
    }

    constexpr const char* Version::Extension() noexcept
    {
        return s_extension;
    }
}

#endif //!CELLSIM_VERSIONS_HPP