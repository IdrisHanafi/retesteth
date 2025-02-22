#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string t8ntool_Istanbul_config = R"({
    "params" : {
        "fork" : "ETC_Phoenix",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

genIstanbulCfgETCTR::genIstanbulCfgETCTR()
{
    spDataObject obj2;
    (*obj2)["path"] = "etctranslate/genesis/Istanbul.json";
    (*obj2)["content"] = t8ntool_Istanbul_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
