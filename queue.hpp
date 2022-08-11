// Created by Matthew Gimlin on August 10, 2022.

// This class is a queue data structure. The underlying
// container is an array.

#pragma once

template<typename T>
class Queue
{
public:
    using SizeType       = std::size_t;
    using ValueType      = T;
    using Reference      = T&;
    using ConstReference = const T&;

    /**
     * @brief Default constructor.
     * 
     * Creates an empty queue.
     */
    Queue()
        : m_Container( (T*) ::operator new(INITIAL_CAPACITY * sizeof(T)) ),
          m_Size(0),
          m_Capacity(INITIAL_CAPACITY)
    { }

    /**
     * @brief Copy constructor.
     * @param other The queue to copy.
     * 
     * Creates a queue by copying the contents of another.
     */
    Queue(const Queue& other)
        : m_Container( (T*) ::operator new(other.m_Capacity * sizeof(T)) ),
          m_Size(other.m_Size),
          m_Capacity(other.m_Capacity)
    {
        for (SizeType i = 0; i < m_Size; ++i)
            new (&m_Container[i]) T(other.m_Container[i]);
    }

    /**
     * @brief Move constructor.
     * @param other The queue to move.
     * 
     * Creates a queue by moving the contents of another.
     */
    Queue(Queue&& other)
        : m_Container(other.m_Capacity),
          m_Size(other.m_Size),
          m_Capacity(other.m_Capacity)
    {
        other.m_Capacity = ::operator new(INITIAL_CAPACITY * sizeof(T));
        other.m_Size = 0;
        other.m_Capacity = INITIAL_CAPACITY;
    }

    ~Queue()
    {
        Clear();
        ::operator delete(m_Container, m_Capacity * sizeof(T));
    }

    /**
     * @brief Copy assignment operator.
     * @param other The queue to copy.
     * 
     * Clears the queue, and recreates it by copying the contents of another.
     */
    Queue& operator=(const Queue& other)
    {
        if (this == &other) return *this;

        Clear();
        ::operator delete(m_Container, m_Capacity * sizeof(T));

        m_Container = (T*) ::operator new(other.m_Capacity * sizeof(T));
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        for (SizeType i = 0; i < m_Size; ++i)
            new (&m_Container[i]) T(other.m_Container[i]);
        
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The queue to move.
     * 
     * Clears the queue, and recreates it by moving the contents of another.
     */
    Queue& operator=(Queue&& other)
    {
        if (this == &other) return *this;

        m_Container = other.m_Capacity;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        other.m_Capacity = (T*) ::operator new(INITIAL_CAPACITY * sizeof(T));
        other.m_Size = 0;
        other.m_Capacity = INITIAL_CAPACITY;
        
        return *this;
    }

    // private member data getters
    bool Empty() const { return m_Size == 0; }
    SizeType Size() const { return m_Size; }
    SizeType Capacity() const { return m_Capacity; }

    // get the first or last elements of the queue
    Reference Front() { return m_Container[0]; }
    ConstReference Front() const { return m_Container[0]; }
    Reference Back() { return m_Container[m_Size - 1]; }
    ConstReference Back() const { return m_Container[m_Size - 1]; }
    
    /**
     * @brief Pushes onto the queue.
     * @param value The value to push.
     * 
     * Adds value to the back of the queue by copying it.
     */
    void Push(const ValueType& value)
    {
        if (m_Size >= m_Capacity)
            Reallocate(m_Capacity * 2);
        
        new (&m_Container[m_Size]) T(value);
        ++m_Size;
    }

    /**
     * @brief Pushes onto the queue.
     * @param value The value to push.
     * 
     * Adds value to the back of the queue by moving it.
     */
    void Push(ValueType&& value)
    {
        if (m_Size >= m_Capacity)
            Reallocate(m_Capacity * 2);
        
        new (&m_Container[m_Size]) T( std::move(value) );
        ++m_Size;
    }

    /**
     * @brief Pops the queue.
     * 
     * Destructs the front element of the queue, and moves forward all other elements.
     */
    void Pop()
    {
        if (m_Size == 0) return;

        --m_Size;
        for (SizeType i = 0; i < m_Size; ++i)
        {
            m_Container[i].~T();
            new (&m_Container[i]) T( std::move(m_Container[i + 1]) );
        }
        m_Container[m_Size].~T();
    }

    /**
     * @brief Clears the container.
     * 
     * Destructs each element of the container.
     */
    void Clear()
    {
        for (SizeType i = 0; i < m_Size; ++i)
            m_Container[i].~T();
        
        m_Size = 0;
    }

private:
    T* m_Container;
    SizeType m_Size;
    SizeType m_Capacity;

    static const SizeType INITIAL_CAPACITY = 1;

    /**
     * @brief Grows the container.
     * @param newCapacity The new capacity of the container.
     * 
     * Creates a new larger container, and moves the contents over.
     */
    void Reallocate(SizeType newCapacity)
    {
        if (newCapacity <= m_Capacity)
            throw std::invalid_argument("The new capacity must be larger than the current.");

        T* newContainer = (T*) ::operator new(newCapacity * sizeof(T));

        // move contents and call destructors
        for (SizeType i = 0; i < m_Size; ++i)
        {
            new (&newContainer[i]) ValueType( std::move(m_Container[i]) );
            m_Container[i].~ValueType();
        }

        // reassign member data
        ::operator delete(m_Container, m_Capacity * sizeof(T));
        m_Container = newContainer;
        m_Capacity = newCapacity;
    }
};