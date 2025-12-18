#ifndef CELLSIM_CLI_VALUEOPTION_HPP
#define CELLSIM_CLI_VALUEOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.CliOption.hpp"

#include <string>

namespace CellSim::Cli
{
    /// @brief 値を持つオプション
    class ValueOption : public CliOption {
        private:

        protected:

        ::std::string m_value;

        ValueOption() = default;

        constexpr ValueOption(
            ::std::string defaultValue
        ) noexcept;

        public:

        void AddValue(
            ::std::string_view value
        ) override;

        constexpr bool HasValue() const noexcept override;
        constexpr bool IsRepeatable() const noexcept override;

        constexpr ::std::string const& Value() const override;

        ::std::vector<::std::string> const& Values() const override;
    };
}

namespace CellSim::Cli
{
    constexpr ValueOption::ValueOption(
        ::std::string defaultValue
    ) noexcept
        : m_value(::std::move(defaultValue))
    {
    }

    inline void ValueOption::AddValue(
        ::std::string_view value
    )
    {
        m_value = value;
    }

    constexpr bool ValueOption::HasValue() const noexcept
    {
        return true;
    }

    constexpr bool ValueOption::IsRepeatable() const noexcept
    {
        return false;
    }

    constexpr ::std::string const& ValueOption::Value() const
    {
        return m_value;
    }
}

#endif //!CELLSIM_CLI_VALUEOPTION_HPP