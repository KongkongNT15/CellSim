#include "CellSim.IO.NamedPipe.hpp"

#if CELLSIM_ENV_UNIX
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

namespace CellSim::IO
{
    NamedPipe NamedPipe::s_in;
    NamedPipe NamedPipe::s_out;

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
    bool NamedPipe::Initialize(
        const char* nameIn,
        const char* nameOut
    ) noexcept
    {
        if (s_isInitialized) [[unlikely]] return false;
        if (!nameIn || !nameOut) [[unlikely]] return false;

        s_in = NamedPipe(
            nameIn,
            O_RDONLY
        );

        s_out = NamedPipe(
            nameOut,
            O_WRONLY
        );

        if (s_in.IsInvalid() || s_out.IsInvalid()) return false;

        s_isInitialized = true;

        return true;
    }

    NamedPipe::NamedPipe(
    ) noexcept
        : m_handle(EOF)
    {
    }

    NamedPipe::NamedPipe(
        const char* name,
        int desiredAccess
    ) noexcept
        : NamedPipe()
    {
        // ここで name は nullptr ではありません
        int fd = ::open(
            name,
            desiredAccess
        );

        // 開けなかったよ
        if (fd == EOF) return;

        struct ::stat statbuf;

        ::fstat(
            fd,
            &statbuf
        );

        // パイプじゃない何かを開いたよ
        if (!S_ISFIFO(statbuf.st_mode)) return;
    
        m_handle = fd;
    }

    NamedPipe::~NamedPipe()
    {
        if (IsInvalid()) return;

        ::close(m_handle);
    }

    NamedPipe& NamedPipe::operator=(
        NamedPipe&& other
    ) noexcept
    {
        if (&other != this) [[likely]] {
            if (!IsInvalid()) ::close(m_handle);

            m_handle = other.m_handle;

            other.m_handle = EOF;
        }

        return *this;
    }

    bool NamedPipe::IsInvalid(
    ) const noexcept
    {
        return m_handle == EOF;
    }

    template <class TContainer>
    bool NamedPipe::ReceiveData(
        TContainer& container
    ) noexcept
    {
        uint32_t length;

        ssize_t result = ::read(
            m_handle,
            &length,
            sizeof(uint32_t)
        );

        if (result == EOF) return false;

        container.resize(length);

        result = ::read(
            m_handle,
            container.data(),
            length
        );

        return result != EOF;
    }

    bool NamedPipe::SendDataUnsafe(
        size_t length,
        void* p
    ) noexcept
    {
        uint32_t dLength = static_cast<uint32_t>(length);

        ssize_t result = ::write(
            m_handle,
            &dLength,
            sizeof(uint32_t)
        );

        if (result == EOF) return false;

        result = ::write(
            m_handle,
            p,
            length
        );

        return result != EOF;
    }
#endif

    template bool NamedPipe::ReceiveData(::std::string&);
    template bool NamedPipe::ReceiveData(::std::vector<uint8_t>&);
}