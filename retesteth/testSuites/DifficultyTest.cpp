#include "DifficultyTest.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/types/DifficultyTests/DifficultyTest.h>
#include <retesteth/testStructures/types/DifficultyTests/DifficultyTestFiller.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ExitHandler.h>

using namespace std;
using namespace test;
using namespace test::session;
namespace fs = boost::filesystem;
namespace
{

spDataObject makeTest(FORK const& _fork, VALUE const& _bn, VALUE const& _td, VALUE const& _pd, VALUE const& _un)
{
    spDataObject test;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    VALUE const res = session.test_calculateDifficulty(_fork, _bn, 0, _pd, _td, _un);
    (*test)["parentTimestamp"] = "0x00";
    (*test)["parentUncles"] = _un.asString();
    (*test)["parentDifficulty"] = _pd.asString();
    (*test)["currentTimestamp"] = _td.asString();
    (*test)["currentBlockNumber"] = _bn.asString();
    (*test)["currentDifficulty"] = res.asString();
    return test;
}

spDataObject FillTest(DifficultyTestInFiller const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    spDataObject filledTest;
    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.info().rawData();

    size_t i = 0;
    for (auto const& fork : _test.networks())
    {
        if (ExitHandler::receivedExitSignal())
            break;

        // Skip by --singlenet option
        bool networkSkip = false;
        Options const& opt = Options::get();

        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            networkSkip = true;

        if (networkSkip)
            continue;

        spDataObject filledTestNetwork;
        for (auto const& bn : _test.blocknumbers().vector())
        {
            for (auto const& td : _test.timestumps().vector())
            {
                for (auto const& pd : _test.parentdiffs().vector())
                {
                    for (auto const& un : _test.uncles())
                    {
                        if (ExitHandler::receivedExitSignal())
                            break;
                        string const testname = _test.testName() + "-" + test::fto_string(i++);
                        (*filledTestNetwork).atKeyPointer(testname) = makeTest(fork, bn, td, pd, un);
                    }
                }
            }
        }

        (*filledTest).atKeyPointer(fork.asString()) = filledTestNetwork;
    }
    return filledTest;
}

void RunTest(DifficultyTestInFilled const& _test)
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    for (auto const& v : _test.testVectors())
    {
        for (auto const& el : v.second)
        {
            if (ExitHandler::receivedExitSignal())
                break;
            VALUE const res = session.test_calculateDifficulty(
                v.first, el.currentBlockNumber, el.parentTimestamp, el.parentDifficulty, el.currentTimestamp, el.parentUncles);
            ETH_ERROR_REQUIRE_MESSAGE(res == el.currentDifficulty, _test.testName() + "/" + el.testVectorName +
                                                                       " difficulty mismatch got: `" + res.asDecString() +
                                                                       ", test want: `" + el.currentDifficulty->asDecString());
        }
    }
}

}  // namespace

namespace test
{
spDataObject DifficultyTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("DifficultyTestSuite::doTests init"));

    if (_opt.doFilling)
    {
        spDataObject filledTest;
        DifficultyTestFiller filler(_input);

        for (auto const& test : filler.tests())
        {
            if (ExitHandler::receivedExitSignal())
                break;
            (*filledTest).addSubObject(test.testName(), FillTest(test));
            TestOutputHelper::get().registerTestRunSuccess();
        }
        return filledTest;
    }
    else
    {
        DifficultyTest filledTest(_input);
        // Just check the test structure if running with --checkhash
        if (Options::get().checkhash)
            return spDataObject();
        if (Options::get().getvectors)
        {
            filledTest.registerAllVectors();
            return spDataObject();
        }

        for (auto const& test : filledTest.tests())
        {
            if (ExitHandler::receivedExitSignal())
                break;
            RunTest(test);
            TestOutputHelper::get().registerTestRunSuccess();
        }
    }
    return spDataObject();
}

}  // namespace test
