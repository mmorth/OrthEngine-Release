#ifndef PUBLISHERMOCK_HPP
#define PUBLISHERMOCK_HPP

#include <gmock/gmock.h>

#include "Publisher.hpp"

template<typename T>
class PublisherMock : public Publisher<T> {
public:
    MOCK_METHOD(void, publish, (T), (override));
    MOCK_METHOD(void, subscribe, (void*, typename Publisher<T>::callback_t), (override));
    MOCK_METHOD(void, unsubscribe, (void*), (override));
};

#endif // PUBLISHERMOCK_HPP
