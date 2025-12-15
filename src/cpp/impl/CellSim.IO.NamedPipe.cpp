#include "CellSim.IO.NamedPipe.hpp"

namespace CellSim::IO
{
    static NamedPipe s_in;
    static NamedPipe s_out;

#if CELLSIM_ENV_WINDOWS
    bool NamedPipe::Initialize(
        ::std::string_view nameIn,
        ::std::string_view nameOut
    ) noexcept
    {
        if (s_isInitialized) [[unlikely]] return;

        s_in = NamedPipe(nameIn);
        s_out = NamedPipe(nameOut);

        if (s_in.IsInvalid() || s_out.IsInvalid()) return false;

        s_isInitialized = true;

        return true;
    }

    NamedPipe::NamedPipe(
    ) noexcept
        : m_handle(INVALID_HANDLE_VALUE)
    {
    }

    NamedPipe::NamedPipe(
        ::std::string_view name
    ) noexcept
    {
        
    }

    NamedPipe::~NamedPipe()
    {
        if (IsInvalid()) return;

        ::CloseHandle(m_handle);
    }

    NamedPipe& NamedPipe::operator=(
        NamedPipe&& other
    ) noexcept
    {
        if (&other != this) [[likely]] {
            if (!IsInvalid()) ::CloseHandle(m_handle);

            m_handle = other.m_handle;

            other.m_handle = INVALID_HANDLE_VALUE;
        }

        return *this;
    }

    bool NamedPipe::IsInvalid(
    ) const noexcept
    {
        return m_handle == INVALID_HANDLE_VALUE;
    }
#elif CELLSIM_ENV_UNIX

#endif
}