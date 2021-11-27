#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_correctMiningReward_config = R"({
    "//comment" : "State Tests does not calculate mining reward in post conditions, so when filling a blockchain test out of it, the mining reward must be set",
    "Frontier":          "5000000000000000000",
    "Homestead":         "5000000000000000000",
    "EIP150":            "5000000000000000000",
    "EIP158":            "5000000000000000000",
    "Byzantium":         "3000000000000000000",
    "Constantinople":    "2000000000000000000",
    "ConstantinopleFix": "2000000000000000000",
    "Istanbul":          "2000000000000000000",
    "Berlin" :           "2000000000000000000",
    "London" :           "2000000000000000000"
})";

const string t8ntool_correctMiningReward_config = R"({
    "//comment" : "State Tests does not calculate mining reward in post conditions, so when filling a blockchain test out of it, the mining reward must be set",
    "Frontier":          "5000000000000000000",
    "Homestead":         "5000000000000000000",
    "EIP150":            "5000000000000000000",
    "EIP158":            "5000000000000000000",
    "Byzantium":         "3000000000000000000",
    "Constantinople":    "2000000000000000000",
    "ConstantinopleFix": "2000000000000000000",
    "Istanbul":          "2000000000000000000",
    "Berlin" :           "2000000000000000000",
    "London" :           "2000000000000000000",

    "//comment" : "Retesteth calculate rewards on behalf of the tool when filling state tests",
    "YOLOv1" :           "2000000000000000000",
    "YOLOv2" :           "2000000000000000000",
    "YOLOv3" :           "2000000000000000000",
    "Aleut"  :           "2000000000000000000"
})";

genRewardsCfg::genRewardsCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/correctMiningReward.json";
    (*obj)["content"] = default_correctMiningReward_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/correctMiningReward.json";
    (*obj2)["content"] = t8ntool_correctMiningReward_config;
    map_configs.addArrayObject(obj2);
}
