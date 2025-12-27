#ifndef CELLSIM_MESSAGES_HPP
#define CELLSIM_MESSAGES_HPP

#include "base.hpp"

#include <map>
#include <string>
#include <string_view>

namespace CellSim
{
    /// @brief 画面に表示するメッセージ
    class Messages final {
        private:

        static bool s_loaded;
        static ::std::map<::std::string, ::std::string> s_map;
        
        static void s_getLanguage(
            char(&arr)[3]
        );

        static void s_loadMessages(
            ::std::string const& folderPath,
            ::std::string_view fileName
        );

        static void s_loadSingleMessage(
            ::std::string const& folderPath,
            ::std::string_view fileName,
            ::std::string messageName
        );

        static void s_setMessage(
            ::std::string_view languageName
        );

        public:

        CELLSIM_STATIC_CLASS(Messages);

        /// @brief メッセージを取得
        /// @param name メッセージ名
        /// @return メッセージ
        [[nodiscard]] static ::std::string const& Get(
            ::std::string const& name
        );

        static bool Initialize();
        [[nodiscard]] static bool IsInitialized() noexcept;

        static void Show(
            ::std::string const& name
        ) noexcept;
    };
}

namespace CellSim
{
    inline ::std::string const& Messages::Get(
        ::std::string const& name
    )
    {
        return s_map[name];
    }

    inline bool Messages::IsInitialized() noexcept
    {
        return s_loaded;
    }

    inline void Messages::Show(
        ::std::string const& name
    ) noexcept
    {
        ::puts(Get(name).c_str());
    }
}

#endif //!CELLSIM_MESSAGES_HPP