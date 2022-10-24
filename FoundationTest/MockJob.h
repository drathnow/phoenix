#ifndef __MOCKJOB_H___
#define __MOCKJOB_H___

#include <gmock/gmock.h>
#include <JobQueue.h>

using namespace dios::foundation;

class MockJob : public Job
{
public:
    MockJob(bool careAboutDestroy = false) : _careAboutDestroy(careAboutDestroy)
    {
    }

    ~MockJob()
    {
        if (_careAboutDestroy)
            destroyMockJob();
    }

    MOCK_METHOD(void, execute, ());
    MOCK_METHOD0(destroyMockJob, void());

    void expectToBeDestroyed()
    {
        EXPECT_CALL(*this, destroyMockJob());
    }

    bool _careAboutDestroy;

};

#endif /* __MOCKJOB_H___ */
