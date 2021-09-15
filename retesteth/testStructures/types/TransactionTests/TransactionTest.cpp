#include <Options.h>
#include <TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testStructures/types/Ethereum/TransactionReader.h>

#include "EthChecks.h"
#include "TransactionTest.h"

using namespace test;
using namespace test::teststruct;

TransactionTest::TransactionTest(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("TransactionTestFiller", el->getKey()));
            m_tests.push_back(TransactionTestInFilled(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


TransactionTestInFilled::TransactionTestInFilled(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "TransactionTestInFilled " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Required}},
             {"result", {{DataType::Object}, jsonField::Required}},
             {"txbytes", {{DataType::String}, jsonField::Required}}});

        m_name = _data->getKey();
        m_rlp = spBYTES(new BYTES(_data->atKey("txbytes").asString()));
        m_readTransaction = readTransaction(m_rlp); // ?? broken rlp ??
        for (auto const& el : _data->atKey("result").getSubObjects())
        {
            if (el->count("exception"))
                m_expectExceptions.emplace(FORK(el->getKey()), el->atKey("exception").asString());
            else
            {
                spFH32 hash(new FH32(el->atKey("hash")));
                spFH20 sender(new FH20(el->atKey("sender")));
                m_acceptedTransactions.emplace(FORK(el->getKey()), HashSender{hash, sender});
            }
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("TransactionTestFilled convertion error: ") + _ex.what());
    }
}
