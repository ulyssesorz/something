template<typename T>
struct RemoveReference
{
    using type = T;
};

template<typename T>
struct RemoveReference<T&>
{
    using type = T;
};

template<typename T>
struct RemoveReference<T&&>
{
    using type = T;
};

template<typename T>
typename RemoveReference<T>::type&& move(T&& item)
{
    return static_cast<typename RemoveReference<T>::type&&>(item);
}

template<typename T>
T&& forward(typename RemoveReference<T>::type& item)
{
    return static_cast<T&&>(item);
}

template<typename T>
T&& forward(typename RemoveReference<T>::type&& item)
{
    return static_cast<T&&>(item);
}