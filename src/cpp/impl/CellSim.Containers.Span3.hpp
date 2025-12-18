#ifndef CELLSIM_CONTAINERS_SPAN3_HPP
#define CELLSIM_CONTAINERS_SPAN3_HPP

#include "base.hpp"
#include "CellSim.Containers.Span2.hpp"

namespace CellSim::Containers
{
    /// @brief Span3[z][y][x]
    /// @tparam T 
    template <class T>
    class Span3 {
        private:

        size_t m_lengthX;
        size_t m_lengthY;
        size_t m_lengthZ;

        T* m_ptr;

        public:

        [[nodiscard]]
        static consteval size_t ElementSize(
        ) noexcept;

        Span3(
        ) = default;

        constexpr Span3(
            size_t lengthZ,
            size_t lengthY,
            size_t lengthX,
            T* ptr
        ) noexcept;

        [[nodiscard]]
        constexpr
        Span2<T> operator[](
            size_t index
        ) const noexcept;

        [[nodiscard]] constexpr T* begin() noexcept;
        [[nodiscard]] constexpr const T* begin() const noexcept;
        [[nodiscard]] constexpr T* end() noexcept;
        [[nodiscard]] constexpr const T* end() const noexcept;

        [[nodiscard]]
        constexpr T& At(
            size_t index1,
            size_t index2,
            size_t index3
        ) const noexcept;

        [[nodiscard]] constexpr size_t Length() const noexcept;
        [[nodiscard]] constexpr size_t LengthX() const noexcept;
        [[nodiscard]] constexpr size_t LengthY() const noexcept;
        [[nodiscard]] constexpr size_t LengthZ() const noexcept;
    };
}

namespace CellSim::Containers
{
    template <class T>
    consteval size_t Span3<T>::ElementSize() noexcept
    {
        return sizeof(T);
    }

    template <class T>
    constexpr Span3<T>::Span3(
        size_t lengthZ,
        size_t lengthY,
        size_t lengthX,
        T* ptr
    ) noexcept
        : m_lengthX(lengthX)
        , m_lengthY(lengthY)
        , m_lengthZ(lengthZ)
        , m_ptr(ptr)
    {
    }

    template <class T>
    constexpr Span2<T> Span3<T>::operator[](
        size_t index
    ) const noexcept
    {
        return Span2(m_lengthY, m_lengthX, m_ptr + (index * m_lengthY * m_lengthX));
    }

    template <class T>
    constexpr T* Span3<T>::begin() noexcept
    {
        return m_ptr;
    }

    template <class T>
    constexpr const T* Span3<T>::begin() const noexcept
    {
        return m_ptr;
    }

    template <class T>
    constexpr T* Span3<T>::end() noexcept
    {
        return m_ptr + Length();
    }

    template <class T>
    constexpr const T* Span3<T>::end() const noexcept
    {
        return m_ptr + Length();
    }

    template <class T>
    constexpr T& Span3<T>::At(
        size_t index1,
        size_t index2,
        size_t index3
    ) const noexcept
    {
        return *(m_ptr + ((index1 * m_lengthY + index2) * m_lengthX + index3));
    }

    template <class T>
    constexpr size_t Span3<T>::Length() const noexcept
    {
        return m_lengthX * m_lengthY * m_lengthZ;
    }

    template <class T>
    constexpr size_t Span3<T>::LengthX() const noexcept
    {
        return m_lengthX;
    }

    template <class T>
    constexpr size_t Span3<T>::LengthY() const noexcept
    {
        return m_lengthY;
    }

    template <class T>
    constexpr size_t Span3<T>::LengthZ() const noexcept
    {
        return m_lengthZ;
    }
}

#endif //!CELLSIM_CONTAINERS_SPAN3_HPP