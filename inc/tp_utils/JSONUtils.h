#ifndef tp_utils_JSONUtils_h
#define tp_utils_JSONUtils_h

#include "tp_utils/StringID.h"

#include "json.hpp"

#define TPJSON          tp_utils::getJSON
#define TPJSONString    tp_utils::getJSONString
#define TPJSONInt       tp_utils::getJSONNumber<int>
#define TPJSONSizeT     tp_utils::getJSONNumber<size_t>
#define TPJSONInt64T    tp_utils::getJSONNumber<int64_t>
#define TPJSONUint64T   tp_utils::getJSONNumber<uint64_t>
#define TPJSONUint16T   tp_utils::getJSONNumber<uint16_t>
#define TPJSONFloat     tp_utils::getJSONNumber<float>
#define TPJSONDouble    tp_utils::getJSONNumber<double>
#define TPJSONBool      tp_utils::getJSONBool
#define TPJSONList      tp_utils::getJSONStringList
#define TPJSONArray     tp_utils::getJSONArray
#define TPJSONStringIDs tp_utils::getJSONStringIDs

namespace tp_utils
{

//##################################################################################################
[[nodiscard]] nlohmann::json TP_UTILS_SHARED_EXPORT jsonFromString(const std::string& json);

//##################################################################################################
[[nodiscard]] nlohmann::json TP_UTILS_SHARED_EXPORT getJSON(const nlohmann::json& j,
                                                            const std::string& key,
                                                            const nlohmann::json& defaultValue=nlohmann::json());

//##################################################################################################
[[nodiscard]] float TP_UTILS_SHARED_EXPORT getJSONFloat(const nlohmann::json& j,
                                                        const std::string& key,
                                                        float defaultValue=nlohmann::json());

//##################################################################################################
template<typename T>
[[nodiscard]] T getJSONNumber(const nlohmann::json& j,
                              const std::string& key,
                              const T& defaultValue=T())
{
  if(j.is_object())
  {
    const auto it = j.find(key);
    if(it != j.end())
    {
      if(it->is_number())
      {
        try
        {
          return it->get<T>();
        }
        catch(...)
        {
        }
      }
    }
  }

  return defaultValue;
}



//##################################################################################################
[[nodiscard]] std::string TP_UTILS_SHARED_EXPORT getJSONString(const nlohmann::json& j,
                                                               const std::string& key,
                                                               const std::string& defaultValue=std::string());

//##################################################################################################
[[nodiscard]] bool TP_UTILS_SHARED_EXPORT getJSONBool(const nlohmann::json& j,
                                                      const std::string& key,
                                                      const bool& defaultValue=bool());

//##################################################################################################
[[nodiscard]] std::vector<std::string> TP_UTILS_SHARED_EXPORT getJSONStringList(const nlohmann::json& j,
                                                                                const std::string& key);

//##################################################################################################
[[nodiscard]] std::vector<nlohmann::json> TP_UTILS_SHARED_EXPORT getJSONArray(const nlohmann::json& j,
                                                                              const std::string& key);

//##################################################################################################
[[nodiscard]] std::vector<StringID> TP_UTILS_SHARED_EXPORT getJSONStringIDs(const nlohmann::json& j,
                                                                            const std::string& key);

//##################################################################################################
[[nodiscard]] nlohmann::json stringIDsToJSON(const std::vector<StringID>& stringIDs);

}

#endif
