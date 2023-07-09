#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <functional>
#include <map>
#include <vector>

template<typename T>
class Publisher 
{
public:
    using callback_t = std::function<void(T)>;
    virtual void publish(T data) 
    {
        //LOG(INFO) << "publish";

        for (auto& sub : subscribers_) 
        {
            sub.second(data);
        }
    }
    virtual void subscribe(void* id, callback_t cb) 
    {
        //LOG(INFO) << "New subscriber: " << id;

        subscribers_[id] = cb;
    }
    virtual void unsubscribe(void* id) 
    {
        //LOG(INFO) << "Subscribed removed: " << id;

        subscribers_.erase(id);
    }

private:
    std::map<void*, callback_t> subscribers_;
};

#endif
