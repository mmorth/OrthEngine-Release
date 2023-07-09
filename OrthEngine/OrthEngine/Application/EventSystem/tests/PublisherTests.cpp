#include "gtest/gtest.h"
#include "Publisher.hpp"

class PublisherTest : public testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST(PublisherTest, TestPublishAndSubscribe)
{
    // Step 3: Instantiate the Publisher
    Publisher<std::string> publisher;

    // Step 4: Define test data and expected results
    std::string testData = "Test Data";
    std::string receivedData;
    bool callbackCalled = false;

    // Step 5: Publish and subscribe
    publisher.subscribe(this, [&receivedData, &callbackCalled](const std::string& data) {
        receivedData = data;
        callbackCalled = true;
    });

    publisher.publish(testData);

    // Step 6: Verify the results
    EXPECT_EQ(receivedData, testData);
    EXPECT_TRUE(callbackCalled);

    // Step 7: Clean up
    publisher.unsubscribe(this);
}
