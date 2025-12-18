#ifndef CELLSIM_CLI_CLIOPTION_HPP
#define CELLSIM_CLI_CLIOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.CliOptionType.hpp"
#include <string_view>
#include <string>
#include <vector>

namespace CellSim::Cli
{
    /// @brief コマンドラインオプション
    class CliOption {
        protected:

        bool m_isEnabled;

        constexpr CliOption() noexcept;

        constexpr CliOption(
            bool isEnabled
        ) noexcept;

        public:

        virtual ~CliOption() = default;

        /// @brief HasValue()が`true`の時に値を追加する
        /// @param value 追加する値
        virtual void AddValue(
            ::std::string_view value
        ) = 0;

        void Enable();

        [[nodiscard]]
        virtual ::std::string_view FullName() const noexcept = 0;

        /// @brief このオプションが値を持つかどうか
        [[nodiscard]]
        virtual bool HasValue() const noexcept = 0;

        /// @brief このオプションが有効かどうか
        [[nodiscard]]
        constexpr bool IsEnabled() const noexcept;

        /// @brief このオプションを複数回指定できるかどうか
        [[nodiscard]]
        virtual bool IsRepeatable() const noexcept = 0;

        /// @brief 
        /// @param optionName 
        /// @return 
        virtual bool IsMatch(
            ::std::string_view optionName
        ) const noexcept = 0;

        /// @brief オプションが有効な時に呼ばれる関数
        /// @param sender このメンバー関数を呼んだインスタンス
        /// @param args 処理に必要な情報
        virtual void OnActive(
            const CliOptions* sender,
            CliOptionActivationArgs args
        );
        
        [[nodiscard]]
        virtual ::std::string_view Names() const noexcept = 0;

        /// @brief このオプションの種類
        [[nodiscard]]
        virtual CliOptionType OptionType() const noexcept = 0;

        /// @brief TakesControl()が'true'のときに実行される
        virtual void Run(
            const CliOptions* sender,
            CliOptionArgs args
        );

        /// @brief 'true'のときはオプション自身が主役となって独自の動作を行う
        [[nodiscard]]
        virtual bool TakesControl() const noexcept = 0;

        /// @brief 値
        [[nodiscard]]
        virtual ::std::string const&
        Value() const = 0;
        
        [[nodiscard]]
        virtual ::std::vector<::std::string> const&
        Values() const = 0;
    };
}

namespace CellSim::Cli
{
    constexpr CliOption::CliOption() noexcept
        : m_isEnabled(false)
    {
    }

    constexpr CliOption::CliOption(
        bool isEnabled
    ) noexcept
        : m_isEnabled(isEnabled)
    {
    }

    constexpr bool CliOption::IsEnabled() const noexcept
    {
        return m_isEnabled;
    }
}

#endif //!CELLSIM_CLI_CLIOPTION_HPP