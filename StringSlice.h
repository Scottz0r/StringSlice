/// @file
/// Defines the StringSlice object.
#ifndef _SCOTTZ0R_STRING_SLICE_INCLUDE_GUARD
#define _SCOTTZ0R_STRING_SLICE_INCLUDE_GUARD

namespace scottz0r
{
    /// Non owning slice of a string. This has the same lifetime as the m_str pointer. This class does not
    /// throw exceptions.
    class StringSlice
    {
    public:
        using size_type = unsigned int;
        static constexpr size_type npos = (size_type)-1;

        /// Default construct to an empty slice.
        StringSlice() noexcept
            : m_str(nullptr), m_size(0)
        {
        }

        /// Construct with a string. This will loop until a null character is found. Does not include terminating
        /// null character.
        StringSlice(const char* str) noexcept
            : m_str(str), m_size(0)
        {
            const char* p_str = m_str;
            while (*p_str != 0)
            {
                ++p_str;
            }

            m_size = p_str - m_str;
        }

        /// Construct with a string and size. Null characters will be included.
        StringSlice(const char* str, size_type size) noexcept
            : m_str(str), m_size(size)
        {

        }

        /// Copy constructor. This slice will have the same lifetime as the other slice.
        StringSlice(const StringSlice& other) noexcept
        {
            m_str = other.m_str;
            m_size = other.m_size;
        }

        /// Get the item at the given index with bounds checking. Returns -1 if the item is out of range.
        /// Characters are converted into integers upon return.
        int at(size_type i) const noexcept
        {
            if (i < m_size)
            {
                return (int)m_str[i];
            }

            return -1;
        }

        /// Compare to another StringSlice. Returns -1 if this is less than the other slice. Returns 1 if this
        /// is greater than the other slice. Returns 0 if slices are equal.
        int compare(const StringSlice& other) const noexcept
        {
            for (size_type i = 0; i < m_size && i < other.m_size; ++i)
            {
                int diff = m_str[i] - other.m_str[i];

                if (diff < 0)
                {
                    return -1;
                }

                if (diff > 0)
                {
                    return 1;
                }
            }

            if (m_size < other.m_size)
            {
                return -1;
            }
            else if (m_size > other.m_size)
            {
                return 1;
            }

            return 0;
        }

        /// @see copy_to.
        template<size_type _Size>
        inline size_type copy_to(char(&dst)[_Size]) const
        {
            return copy_to(dst, _Size);
        }

        /// Copy this slice to a character buffer. This will always null terminate. Returns the number of
        /// characters copied, not including the null terminator. If the destination buffer is too small, the
        /// result will be truncated.
        size_type copy_to(char* dst, size_type dst_size) const
        {
            if (!dst || dst_size == 0)
            {
                return 0;
            }

            size_type i = 0;
            for (; i < dst_size && i < m_size; ++i)
            {
                dst[i] = m_str[i];
            }

            if (i < dst_size)
            {
                dst[i] = 0;
            }
            else
            {
                i = dst_size - 1;
                dst[i] = 0;
            }

            return i;
        }

        /// Get a pointer to the data.
        const char* data() const noexcept { return m_str; }

        /// Returns true if the slice is empty.
        bool empty() const noexcept { return m_size == 0; }

        /// Find the given character in the slice. Returns the index of the character. Returns StringSlice::npos
        /// if the character is not found.
        size_type find(char c, size_type start = 0) const noexcept
        {
            if (start >= m_size)
            {
                return npos;
            }

            for (size_type i = start; i < m_size; ++i)
            {
                if (m_str[i] == c)
                {
                    return i;
                }
            }

            return npos;
        }

        /// Returns a new slice without leading whitespace.
        StringSlice lstrip() const noexcept
        {
            const char* p = m_str;
            const char* end = m_str + m_size;

            while (p < end)
            {
                if (!is_whitespace(*p))
                {
                    break;
                }

                ++p;
            }

            size_type new_size = end - p;
            return StringSlice(p, new_size);
        }

        /// Returns a new slice without trailing whitespace.
        StringSlice rstrip() const noexcept
        {
            const char* p = m_str + m_size;

            while ((p--) > m_str)
            {
                if (!is_whitespace(*p))
                {
                    break;
                }
            }

            // Increment pointer because of post decrement in while loop.
            ++p;

            size_type new_size = p - m_str;
            return StringSlice(m_str, new_size);
        }

        /// Returns the number of characters in the slice.
        size_type size() const noexcept { return m_size; }

        /// @brief Return a new slice with leading and trailing whitespace removed.
        StringSlice strip() const noexcept
        {
            return rstrip().lstrip();
        }

        /// Returns a new slice that is a substring of this slice. An empty slice is returned if the starting
        /// position is out of range. If len is larger than the remaining bytes in the slice, the extra length is
        /// ignored.
        StringSlice substr(size_type pos, size_type len = npos) const noexcept
        {
            if (pos < m_size)
            {
                size_type real_len = m_size - pos;
                real_len = real_len < len ? real_len : len;
                return StringSlice(m_str + pos, real_len);
            }

            return StringSlice();
        }

        /// Get the character at the given index without range checking.
        char operator[](size_type i) const noexcept
        {
            return m_str[i];
        }

        /// Assignment operator. The lifetime of this slice will become the same as the other slice.
        StringSlice& operator=(const StringSlice& other) noexcept
        {
            m_str = other.m_str;
            m_size = other.m_size;
            return *this;
        }

        /// Returns true if this slice is not empty.
        explicit operator bool() const noexcept
        {
            return m_size > 0;
        }

        /// Returns true if this slice is empty.
        bool operator!() const noexcept
        {
            return m_size == 0;
        }

        bool operator==(const StringSlice& other) const noexcept
        {
            if (other.m_size != m_size)
            {
                return false;
            }

            for (size_type i = 0; i < m_size; ++i)
            {
                if (m_str[i] != other.m_str[i])
                {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const StringSlice& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const StringSlice& other) const noexcept
        {
            int c = compare(other);
            return c < 0;
        }

        bool operator<=(const StringSlice& other) const noexcept
        {
            int c = compare(other);
            return c <= 0;
        }

        bool operator>(const StringSlice& other) const noexcept
        {
            int c = compare(other);
            return c > 0;
        }

        bool operator>=(const StringSlice& other) const noexcept
        {
            int c = compare(other);
            return c >= 0;
        }

    private:

        inline bool is_whitespace(char c) const noexcept
        {
            return c == '\r' || c == '\n' || c == '\t' || c == ' ';
        }

        const char* m_str;
        size_type m_size;
    };

    /// Converts a character buffer to a slice using a C++ array size template. This assumes the character buffer
    /// is null terminated at the last index. The null terminator at the last index will not be included in the slice.
    // Null terminators not at the end of the array will be included in the slice.
    // Using any array of size 0 is undefined.
    // This is intended for use with constant char arrays (ex: `const char abc[] = "xyz"`).
    template<unsigned int Size>
    constexpr StringSlice to_slice(const char(&str)[Size]) noexcept
    {
        static_assert(Size != 0, "Buffer size cannot be 0");

        return StringSlice(str, Size - 1);
    }

    /// Get a line from a slice. Includes the newline in the returned slice. If no newlines are found, the entire
    /// slice is returned.
    inline StringSlice get_line(const StringSlice& slice) noexcept
    {
        for (StringSlice::size_type i = 0; i < slice.size(); ++i)
        {
            if (slice[i] == '\n')
            {
                // Add one to size to pick up newline character.
                return StringSlice(slice.data(), i + 1);
            }
        }

        // Return entire slice if a newline was not found.
        return slice;
    }
}

#endif // _SCOTTZ0R_STRING_SLICE_INCLUDE_GUARD
