#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockClock.h>
#include <ctime>
#include <iostream>

#include "JobQueue.h"
#include "MockJob.h"

namespace zios::foundation
{

using namespace std;
using namespace zios::test;

using testing::Mock;
using testing::Return;

class JobQueueTest : public testing::Test
{
public:
    JobQueueTest() : _jobQueueUnderTest(_systemClock) {}
    virtual ~JobQueueTest() {}

    MockClock _systemClock;
    JobQueue _jobQueueUnderTest;

    static ExtendedPriorityQueue<JobQueue::tqe>& theQueueForJobQueue(JobQueue& jobQueueUnderTest)
    {
        return jobQueueUnderTest._theQueue;
    }
};

class WhenJobQueueIsAskedToSubmitAJob : public JobQueueTest
{
public:
    WhenJobQueueIsAskedToSubmitAJob() {}
    ~WhenJobQueueIsAskedToSubmitAJob() {}
};

TEST_F(WhenJobQueueIsAskedToSubmitAJob, shouldSubmitJobAndPutAtHeadOfQueue)
{
    MockJob* job = new MockJob(true);

    time_t now = ::time(nullptr);
    time_t dueTime = now + 10;

    EXPECT_CALL(*job, destroyMockJob());
    EXPECT_CALL(_systemClock, currentTime()).WillOnce(Return(now));

    _jobQueueUnderTest.submit(*job, dueTime);

    ASSERT_EQ(1, theQueueForJobQueue(_jobQueueUnderTest).size());
}

TEST_F(WhenJobQueueIsAskedToSubmitAJob, shouldSubmitJobAndOrderJobsAccordingToTime)
{
    MockJob* job1 = new MockJob(true);
    MockJob* job2 = new MockJob(true);
    MockJob* job3 = new MockJob(true);

    time_t now = ::time(nullptr);
    time_t dueTime1 = now + 10;
    time_t dueTime2 = now + 20;
    time_t dueTime3 = now + 30;

    EXPECT_CALL(*job1, destroyMockJob());
    EXPECT_CALL(_systemClock, currentTime())
        .Times(3)
        .WillOnce(Return(now));

    _jobQueueUnderTest.submit(*job1, dueTime1);
    _jobQueueUnderTest.submit(*job3, dueTime3);
    _jobQueueUnderTest.submit(*job2, dueTime2);

    ASSERT_EQ(3, theQueueForJobQueue(_jobQueueUnderTest).size());
    ASSERT_EQ(job1, theQueueForJobQueue(_jobQueueUnderTest).top().job);
    delete theQueueForJobQueue(_jobQueueUnderTest).top().job;
    theQueueForJobQueue(_jobQueueUnderTest).pop();

    ASSERT_EQ(2, theQueueForJobQueue(_jobQueueUnderTest).size());
    ASSERT_EQ(job2, theQueueForJobQueue(_jobQueueUnderTest).top().job);
    delete theQueueForJobQueue(_jobQueueUnderTest).top().job;
    theQueueForJobQueue(_jobQueueUnderTest).pop();

    ASSERT_EQ(job3, theQueueForJobQueue(_jobQueueUnderTest).top().job);
    delete theQueueForJobQueue(_jobQueueUnderTest).top().job;
    theQueueForJobQueue(_jobQueueUnderTest).pop();
}

}
