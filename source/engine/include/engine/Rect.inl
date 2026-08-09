template <typename T>
Rect<T>::Rect()
: left{T{0}}
, top{T{0}}
, width{T{0}}
, height{T{0}}
{
}

template <typename T>
Rect<T>::Rect(const T left, const T top, const T width, const T height)
: left  (left)
, top   (top)
, width (width)
, height(height)
{
}

template <typename T>
Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) :
left  (position.x),
top   (position.y),
width (size.x),
height(size.y)
{
}

template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U>& rectangle) :
left  (static_cast<T>(rectangle.left)),
top   (static_cast<T>(rectangle.top)),
width (static_cast<T>(rectangle.width)),
height(static_cast<T>(rectangle.height))
{
}

template <typename T>
bool Rect<T>::contains(const T x, const T y) const
{
    T minX = std::min(left, static_cast<T>(left + width));
    T maxX = std::max(left, static_cast<T>(left + width));
    T minY = std::min(top, static_cast<T>(top + height));
    T maxY = std::max(top, static_cast<T>(top + height));

    return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
}

template <typename T>
bool Rect<T>::contains(const Vector2<T>& point) const
{
    return contains(point.x, point.y);
}

template <typename T>
bool Rect<T>::intersects(const Rect<T>& rectangle) const
{
    Rect<T> intersection;
    return intersects(rectangle, intersection);
}

template <typename T>
bool Rect<T>::intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    T r1_min_x = std::min(left, static_cast<T>(left + width));
    T r1_max_x = std::max(left, static_cast<T>(left + width));
    T r1_min_y = std::min(top, static_cast<T>(top + height));
    T r1_max_y = std::max(top, static_cast<T>(top + height));

    T r2_min_x = std::min(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
    T r2_max_x = std::max(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
    T r2_min_y = std::min(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
    T r2_max_y = std::max(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));

    T inter_left   = std::max(r1_min_x, r2_min_x);
    T inter_top    = std::max(r1_min_y, r2_min_y);
    T inter_right  = std::min(r1_max_x, r2_max_x);
    T inter_bottom = std::min(r1_max_y, r2_max_y);

    if ((inter_left < inter_right) && (inter_top < inter_bottom))
    {
        intersection = Rect<T>(inter_left, inter_top, inter_right - inter_left, inter_bottom - inter_top);
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}

template <typename T>
engine::Vector2<T> Rect<T>::getPosition() const
{
    return engine::Vector2<T>(left, top);
}

template <typename T>
engine::Vector2<T> Rect<T>::getSize() const
{
    return engine::Vector2<T>(width, height);
}

template <typename T>
inline bool operator ==(const Rect<T>& left, const Rect<T>& right)
{
    return (left.left == right.left) && (left.width == right.width) &&
           (left.top == right.top) && (left.height == right.height);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Rect<T>& left, const Rect<T>& right)
{
    return !(left == right);
}
