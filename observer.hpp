#pragma once

#include <forward_list>

//abstract base class for observers
template <typename T>
struct IObserver
{
    IObserver() {};
    virtual ~IObserver() {};
    virtual void onUpdate(T *subject) = 0;
};

//abstract base class for subjects
template <typename T>
struct ISubject
{
    ISubject() {};
    virtual ~ISubject() {}

    void subscribe(IObserver<T>* observer)
    {
        this->observers.push_front(observer);
    }

    void unsubscribe(IObserver<T>* observer)
    {
        this->observers.remove(observer);
    }

    void notify()
    {
        for (auto iter = observers.begin(); iter != observers.end(); ++iter)
        {
            (*iter)->onUpdate(static_cast<T*>(this));
        }
    }

private:
    std::forward_list<IObserver<T>*> observers;
};
