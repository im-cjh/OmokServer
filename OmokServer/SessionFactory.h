#pragma once

template <typename T>
class SessionFactory
{
public:
    static shared_ptr<Session> CreateSession()
    {
        return make_shared<T>();
    }
};