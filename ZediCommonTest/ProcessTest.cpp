#include <sys/time.h>
#include <vector>
#include <string>

#include <gtest/gtest.h>
#include "SystemTime.h"
#include "Process.h"
#include "ProcessNotFoundException.h"

using namespace zios::common;
using namespace std;


class ProcessTest: public testing::Test {
protected:

};

TEST_F(ProcessTest, shouldRunChildProcessAndTerminateNormally) {
    Process runner("/bin/ls");
    runner.arguments.push_back("/");

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

    EXPECT_NO_THROW( runner.run() );

    // the values set during run() will remain until after at least the first
    // wait() call, as they are only updated on wait.
    EXPECT_NE(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_TRUE(runner.isRunning());

    // in this test, wait until the process ends
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

}

TEST_F(ProcessTest, DISABLED_shouldGenerateErrorWithNonexistingChild) {
    Process runner("/usr/local/bin/missingprogram");

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

    EXPECT_NO_THROW( runner.run() );

    // the values set during run() will remain until after at least the first
    // wait() call, as they are only updated on wait.
    EXPECT_NE(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_TRUE(runner.isRunning());

    // wait for the child to finish.  This should be fast because the child
    // isn't supposed to exist
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_EQ(0, runner.getPid());
    EXPECT_TRUE(Process::ExitCodeFailedLaunch == runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

}

TEST_F(ProcessTest, shouldAllowRunnerToRunMultipleTimes) {
    // always returns 0 as exit code
    Process runner("/bin/true");


    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

    EXPECT_NO_THROW( runner.run() );

    // the values set during run() will remain until after at least the first
    // wait() call, as they are only updated on wait.
    int pid1 = runner.getPid();
    EXPECT_NE(0, pid1);
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_TRUE(runner.isRunning());

    // wait for the child to finish.  This should be quick because command is trivial
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

    // run it again
    EXPECT_NO_THROW( runner.run() );

    // the values set during run() will remain until after at least the first
    // wait() call, as they are only updated on wait.
    int pid2 = runner.getPid();
    EXPECT_NE(0, pid2);
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_TRUE(runner.isRunning());

    // wait for the child to finish.  This should be quick because command is trivial
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(0, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());

    EXPECT_TRUE(pid1 != pid2);
}


TEST_F(ProcessTest, shouldReturnExitCodeProperly) {
    // always returns 1
    Process runner("/bin/false");

    EXPECT_NO_THROW( runner.run() );
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_EQ(0, runner.getPid());
    EXPECT_EQ(1, runner.getExitCode());
    EXPECT_EQ(0, runner.getExitSignal());
    EXPECT_FALSE(runner.isRunning());
}

TEST_F(ProcessTest, shouldReturnImmediatelyOnNonblockingWait) {
    zios::common::SystemTime time;

    Process runner("/bin/sleep");
    runner.arguments.push_back("2");        // how many sec to sleep

    EXPECT_NO_THROW( runner.run() );

    clock_t time0 = time.currentTimeMS();

    // should return immediately - record exit time
    EXPECT_NO_THROW( runner.wait(true) );
    clock_t time1 = time.currentTimeMS();
    EXPECT_TRUE(runner.isRunning());

    sleep(1);

    // should still be running
    EXPECT_NO_THROW( runner.wait(true) );
    clock_t time2 = time.currentTimeMS();
    EXPECT_TRUE(runner.isRunning());

    sleep(2);

    // should have stopped
    EXPECT_NO_THROW( runner.wait(true) );
    clock_t time3 = time.currentTimeMS();
    EXPECT_FALSE(runner.isRunning());

    // wait for exit this time, just to clean up
    EXPECT_NO_THROW( runner.wait(false) );
    clock_t time4 = time.currentTimeMS();

    // check times - values in msec
    EXPECT_TRUE( (time1 - time0) < 200);
    EXPECT_TRUE( (time2 - time1) >= 1000);
    EXPECT_TRUE( (time2 - time1) < 1200);
    EXPECT_TRUE( (time3 - time2) >= 2000);
    EXPECT_TRUE( (time3 - time2) < 2200);
    EXPECT_TRUE( (time4 - time0) < 3500);
}

TEST_F(ProcessTest, shouldReturnSignalWhenKilled) {
    zios::common::SystemTime time;

    Process runner("/bin/sleep");
    runner.arguments.push_back("5");

    EXPECT_NO_THROW( runner.run() );

    // mark time when we started
    clock_t time0 = time.currentTimeMS();

    // verify is running
    EXPECT_NO_THROW( runner.wait(true) );
    clock_t time1 = time.currentTimeMS();
    EXPECT_TRUE(runner.isRunning());

    // give the child a chance to start before we kill it
    usleep(10 * 1000);

    // send kill, then wait to get new status
    EXPECT_NO_THROW( runner.kill(SIGKILL));

    // should return immediately - record exit time
    EXPECT_NO_THROW( runner.wait(false) );
    clock_t time2 = time.currentTimeMS();
    EXPECT_FALSE(runner.isRunning());
    EXPECT_EQ(0, runner.getExitCode());     // exit code should be 0 because it didn't exit normally
    EXPECT_EQ(SIGKILL, runner.getExitSignal());

    // check times - values in msec
    EXPECT_TRUE( (time1 - time0) < 200);
    EXPECT_TRUE( (time2 - time1) < 200);
}

TEST_F(ProcessTest, shouldNotReadFromChildWhenNotRedirectingOutput) {

    Process runner("echo");
    runner.arguments.push_back("-e");
    runner.arguments.push_back("-n");   // don't have echo add a newline
    runner.arguments.push_back("one\ntwo\n");

    runner.isRedirectStdout = false;     // we do not want the output

    // run and wait for exit
    EXPECT_NO_THROW( runner.run() );
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_FALSE(runner.isRunning());

    int fd = runner.getOutputFileDescriptor();

    // -1 implies output was closed
    EXPECT_EQ(-1, fd);      // the output should be discarded
}

TEST_F(ProcessTest, shouldReadFromChildProcessSTDOUT) {
    // return 2 lines
    const char *expected = "one\ntwo\n";
    char buffer[1024];

    Process runner("echo");
    runner.arguments.push_back("-e");
    runner.arguments.push_back("-n");   // don't have echo add a newline
    runner.arguments.push_back("one\ntwo\n");
    runner.isRedirectStdout = true;     // we want to see output

    // run and wait for exit
    EXPECT_NO_THROW( runner.run() );
    EXPECT_NO_THROW( runner.wait(false) );

    EXPECT_FALSE(runner.isRunning());

    int fd = runner.getOutputFileDescriptor();

    // -1 implies output was closed
    EXPECT_NE(-1, fd);

    int bytesRead = read(fd, buffer, sizeof(buffer));

    int expectedRead = strlen(expected);

    // stop if not matching or next checks will be meaningless
    ASSERT_TRUE(expectedRead == bytesRead);

    // use memcmp because there won't be a null
    EXPECT_EQ(0, memcmp(expected, buffer, expectedRead));

}


TEST_F(ProcessTest, shouldHaveValidProcessState) {
    zios::common::SystemTime time;

    Process runner("/bin/sleep");
    runner.arguments.push_back("2");        // how many sec to sleep

    // process isn't running yet, so PID shouldn't exist
    EXPECT_EQ(Process::Unknown, runner.getProcessState() );

    EXPECT_NO_THROW( runner.run() );

    // give child a chance to start, which will then result in it sleeping
    sleep(1);

    EXPECT_EQ(Process::InterruptibleSleep, runner.getProcessState() );

    // should return immediately - record exit time
    EXPECT_NO_THROW( runner.wait(true) );
    // should still be sleeping
    EXPECT_EQ(Process::InterruptibleSleep, runner.getProcessState() );

    sleep(2);

    // stopped processes turn into zombies until the final wait
    EXPECT_EQ(Process::Defunct, runner.getProcessState() );

    // should have stopped
    EXPECT_NO_THROW( runner.wait(false) );
    EXPECT_FALSE(runner.isRunning());

    // after wait, process shouldn't exist
    EXPECT_EQ(Process::Unknown, runner.getProcessState() );

}

TEST_F(ProcessTest, shouldThrowExceptionWhenKillingMissingProcess) {
    // always returns 1
    Process runner("/bin/false");

    // run child process and wait for execution to finish
    // pid should now be missing from ps list
    EXPECT_NO_THROW( runner.run() );
    int pid = runner.getPid();  // keep track of pid
    EXPECT_NO_THROW( runner.wait(false) );

    // pid is cleared after a wait that finds the child is gone
    EXPECT_EQ(0, runner.getPid());

    // use the static method for kill that supplies an arbitrary pid
    // use the pid from the earlier part of the test as we know it is "safe" to use and we won't
    // kill something else that might be important.
    EXPECT_THROW(runner.kill(pid, SIGKILL), ProcessNotFoundException);
}

