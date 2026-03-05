#pragma once

template <typename T>
class ISingleton {
public:

    static T& getInstance() {
        static T instance;
        return instance;
    }

    ISingleton(const ISingleton&) = delete;
    ISingleton& operator=(const ISingleton&) = delete;

protected:
    ISingleton() = default;
    virtual ~ISingleton() = default;
};