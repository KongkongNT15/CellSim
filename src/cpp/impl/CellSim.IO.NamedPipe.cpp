#include "CellSim.IO.NamedPipe.hpp"

namespace CellSim::IO
{
    static NamedPipe s_in;
    static NamedPipe s_out;

#if CELLSIM_ENV_WINDOWS
    bool NamedPipe::Initialize(
        const char* nameIn,
        const char* nameOut
    ) noexcept
    {
        if (s_isInitialized) [[unlikely]] return false;
        if (!nameIn || !nameOut) [[unlikely]] return false;

        s_in = NamedPipe(
            nameIn,
            GENERIC_READ
        );

        s_out = NamedPipe(
            nameOut,
            GENERIC_WRITE
        );

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
        const char* name,
        int desiredAccess
    ) noexcept
        : NamedPipe()
    {
        // ここで name は nullptr ではありません
        ::HANDLE namedPipe = ::CreateFileA(
            name,
            desiredAccess,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        // 開けなかったよ
        if (namedPipe == INVALID_HANDLE_VALUE) return;

        // パイプじゃない何かを開いたよ
        if (::GetFileType(namedPipe) != FILE_TYPE_PIPE) return;
    
        m_handle = namedPipe;
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

    template <class TContainer>
    bool NamedPipe::ReceiveData(
        TContainer& container
    ) noexcept
    {
        ::DWORD length;

        ::BOOL result = ::ReadFile(
            m_handle,
            &length,
            sizeof(::DWORD),
            nullptr,
            nullptr
        );

        if (result == FALSE) return false;

        container.resize(length);

        result = ::ReadFile(
            m_handle,
            container.data(),
            length,
            nullptr,
            nullptr
        );

        return result == TRUE;
    }

    bool NamedPipe::SendDataUnsafe(
        size_t length,
        void* p
    ) noexcept
    {
        ::DWORD dLength = static_cast<::DWORD>(length);

        ::BOOL result = ::WriteFile(
            m_handle,
            &dLength,
            sizeof(::DWORD),
            nullptr,
            nullptr
        );

        if (result == FALSE) return false;

        result = ::WriteFile(
            m_handle,
            p,
            dLength,
            nullptr,
            nullptr
        );

        return result == TRUE;
    }
#elif CELLSIM_ENV_UNIX

#endif

    template bool NamedPipe::ReceiveData(::std::string&);
    template bool NamedPipe::ReceiveData(::std::vector<uint8_t>&);
}