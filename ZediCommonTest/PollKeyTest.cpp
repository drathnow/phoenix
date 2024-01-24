
#include <gmock/gmock.h>
#include <io.h>

namespace zios {
namespace common {

class PollKeyTest : public testing::Test {
public:

};

class PollKeyForTesting : public PollKey {
public:
    PollKeyForTesting() : PollKey(NULL, NULL, 0) {}
    ~PollKeyForTesting() {}

    int eventMasktoEPollEventMask(int eventMask) { return PollKey::eventMasktoEPollEventMask(eventMask); }
};

TEST_F(PollKeyTest, shouldTranslateNativeEventMaskEPollEventMask) {
    PollKeyForTesting pollKey;

    int epollEventMask = pollKey.eventMasktoEPollEventMask(PollKey::EV_READ);
    ASSERT_TRUE(epollEventMask & EPOLLIN);
    ASSERT_TRUE(epollEventMask & EPOLLHUP);
    ASSERT_EQ(0, epollEventMask ^ (EPOLLIN | EPOLLHUP));

    epollEventMask = pollKey.eventMasktoEPollEventMask(PollKey::EV_WRITE);
    ASSERT_TRUE(epollEventMask & EPOLLOUT);
    ASSERT_EQ(0, epollEventMask ^ EPOLLOUT);

    epollEventMask = pollKey.eventMasktoEPollEventMask(PollKey::EV_ERROR);
    ASSERT_TRUE(epollEventMask & EPOLLERR);
    ASSERT_EQ(0, epollEventMask ^ EPOLLERR);

    epollEventMask = pollKey.eventMasktoEPollEventMask(PollKey::EV_READ | PollKey::EV_ERROR);
    ASSERT_TRUE(epollEventMask & EPOLLIN);
    ASSERT_TRUE(epollEventMask & EPOLLHUP);
    ASSERT_TRUE(epollEventMask & EPOLLERR);
    ASSERT_EQ(0, epollEventMask ^ (EPOLLIN | EPOLLHUP | EPOLLERR));
}

}
}
