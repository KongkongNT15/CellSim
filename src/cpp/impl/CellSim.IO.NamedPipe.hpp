#ifndef CELLSIM_IO_NAMEDPIPE_HPP
#define CELLSIM_IO_NAMEDPIPE_HPP

#include "base.hpp"
#include <string_view>
#include <vector>
#include <span>

#if CELLSIM_ENV_WINDOWS
    #include <Windows.h>
#elif CELLSIM_ENV_UNIX

#else
        static_assert(false, "class CellSim::IO::NamedPipe is not supported.");
#endif

namespace CellSim::IO
{
    class NamedPipe final {
        private:
        static inline bool s_isInitialized;
        static NamedPipe s_in;
        static NamedPipe s_out;

#if CELLSIM_ENV_WINDOWS
        ::HANDLE m_handle;
#elif CELLSIM_ENV_UNIX

#endif

        NamedPipe(
        ) noexcept;

        NamedPipe(
            ::std::string_view name
        ) noexcept;

        NamedPipe(
            NamedPipe const&
        ) = delete;

        NamedPipe& operator=(
            NamedPipe const&
        ) = delete;

        NamedPipe& operator=(
            NamedPipe&& other
        ) noexcept;

        [[nodiscard]]
        bool IsInvalid(
        ) const noexcept;

        [[nodiscard]]
        bool SendDataUnsafe(
            size_t length,
            void* p
        ) noexcept;

        public:

        ~NamedPipe();

        [[nodiscard]]
        static bool IsInitialized(
        ) noexcept;

        [[nodiscard]]
        static bool Initialize(
            ::std::string_view nameIn,
            ::std::string_view nameOut
        ) noexcept;

        template <class T>
        static bool Send(
            ::std::span<T> const& span
        );
        
        template <class T>
        static bool Send(
            ::std::vector<T> const& vec
        );

        static bool SendUnsafe(
            size_t length,
            ::std::nullptr_t
        ) = delete;

        static bool SendUnsafe(
            size_t length,
            void* p
        ) noexcept;
    };
}

namespace CellSim::IO
{
    inline bool NamedPipe::IsInitialized(
    ) noexcept
    {
        return s_isInitialized;
    }

    template <class T>
    bool NamedPipe::Send(
        ::std::span<T> const& span
    )
    {
        return SendUnsafe(
            span.size() * sizeof(T),
            span.data()
        );
    }

    template <class T>
    bool NamedPipe::Send(
        ::std::vector<T> const& vec
    )
    {
        return SendUnsafe(
            vec.size() * sizeof(T),
            vec.data()
        );
    }

    inline bool NamedPipe::SendUnsafe(
        size_t length,
        void* p
    ) noexcept
    {
        return s_out.SendDataUnsafe(
            length,
            p
        );
    }
}

#endif //!CELLSIM_IO_NAMEDPIPE_HPP