#ifndef CELLSIM_IO_NAMEDPIPE_HPP
#define CELLSIM_IO_NAMEDPIPE_HPP

#include "base.hpp"
#include <string>
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
            const char* name,
            int desiredAccess
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

        template <class TContainer>
        [[nodiscard]]
        bool ReceiveData(
            TContainer& container
        ) noexcept;

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
            const char* nameIn,
            const char* nameOut
        ) noexcept;

        [[nodiscard]]
        static bool Receive(
            ::std::string& str
        ) noexcept;

        [[nodiscard]]
        static bool Receive(
            ::std::vector<uint8_t>& vec
        ) noexcept;

        template <class T>
        [[nodiscard]]
        static bool Send(
            ::std::span<T> const& span
        );
        
        template <class T>
        [[nodiscard]]
        static bool Send(
            ::std::vector<T> const& vec
        );

        static bool SendUnsafe(
            size_t length,
            ::std::nullptr_t
        ) = delete;

        [[nodiscard]]
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

    inline bool NamedPipe::Receive(
        ::std::string& str
    ) noexcept
    {
        return s_in.ReceiveData(
            str
        );
    }

    inline bool NamedPipe::Receive(
        ::std::vector<uint8_t>& vec
    ) noexcept
    {
        return s_in.ReceiveData(
            vec
        );
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