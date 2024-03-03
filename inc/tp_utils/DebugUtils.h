#ifndef tp_utils_DebugUtils_h
#define tp_utils_DebugUtils_h

#include "tp_utils/CallbackCollection.h"

#include <sstream>
#include <unordered_set>

#define tpWarning tp_utils::DebugHelper(tp_utils::DBG::Manager::instance().produceWarning())
#define tpDebug tp_utils::DebugHelper(tp_utils::DBG::Manager::instance().produceDebug())

//##################################################################################################
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  os << "( ";
  for(const auto& i : v)
    os << i << ' ';
  os << ')';
  return os;
}

//##################################################################################################
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& v)
{
  os << "( ";
  for(const auto& i : v)
    os << i << ' ';
  os << ')';
  return os;
}

namespace tp_utils
{

//##################################################################################################
enum class MessageType
{
  Warning,
  Debug
};

//##################################################################################################
enum class DebugType
{
  Console,
  Table
};
//##################################################################################################
std::string TP_UTILS_EXPORT getCurrentTimestamp();

//##################################################################################################
void TP_UTILS_EXPORT installSignalHandler();

//##################################################################################################
std::function<void(MessageType, const std::string&)> TP_UTILS_EXPORT installMessageHandler(const std::function<void(MessageType, const std::string&)>& callback);

//##################################################################################################
void TP_UTILS_EXPORT installDateTimeMessageHandler();

//##################################################################################################
void TP_UTILS_EXPORT installDateTimeMemoryMessageHandler();

//##################################################################################################
class TeeMessageHandler
{
public:
  //################################################################################################
  TeeMessageHandler(const std::string& path);

  //################################################################################################
  ~TeeMessageHandler();

private:
  std::function<void(MessageType, const std::string&)> m_previous;
};

//##################################################################################################
class TP_UTILS_EXPORT DebugMode
{
  TP_NONCOPYABLE(DebugMode);
public:
  //################################################################################################
  DebugMode(const std::string& classPath, DebugType debugType=DebugType::Console);

  //################################################################################################
  ~DebugMode();

  //################################################################################################
  bool operator()();

  //################################################################################################
  //! Sets a large blob to debug that may be served to the user in a single chunk
  void setTable(const std::string& table);

  //################################################################################################
  static void installTableCallback(std::function<void(const std::string&, DebugType, const std::string&)> callback);

  //################################################################################################
  static void enable(const std::string& classPath, DebugType debugType, bool enabled);

  //################################################################################################
  static std::vector<std::string> classPaths(DebugType debugType);

private:
  struct Private;
  friend struct Private;
  Private* d;
};

//##################################################################################################
class TP_UTILS_EXPORT DebugBuffer : public std::stringbuf
{
public:
  //################################################################################################
    int sync() override;
};

namespace DBG
{
//##################################################################################################
struct TP_UTILS_EXPORT Base
{
  TP_NONCOPYABLE(Base);

  Base()=default;
  virtual ~Base()=default;
  virtual std::ostream& operator()()=0;
};

//##################################################################################################
struct TP_UTILS_EXPORT FactoryBase
{
  TP_NONCOPYABLE(FactoryBase);

  FactoryBase()=default;
  virtual ~FactoryBase()=default;
  virtual Base* produce()=0;
};

//##################################################################################################
template<typename T>
struct FactoryTemplate : public FactoryBase
{
  TP_NONCOPYABLE(FactoryTemplate);

  FactoryTemplate()=default;
  ~FactoryTemplate() override = default;
  Base* produce() override
  {
    return new T();
  }
};

//##################################################################################################
struct TP_UTILS_EXPORT Manager
{
  Manager();
  ~Manager();

  void setWarning(FactoryBase* warningFactory);
  Base* produceWarning();

  void setDebug(FactoryBase* debugFactory);
  Base* produceDebug();

  static Manager& instance();

  // This is not thread safe but we get away with it...
  CallbackCollection<void(MessageType, const std::string&)> debugCallbacks;

  struct Private;
  Private* d;
};
}

//##################################################################################################
struct TP_UTILS_EXPORT DebugHelper
{
  TP_NONCOPYABLE(DebugHelper);

  DebugHelper(DBG::Base* dbg);
  ~DebugHelper();
  std::ostream& operator()();

  DBG::Base* m_dbg;
};

//##################################################################################################
void TP_UTILS_EXPORT installDefaultMessageHandler();

}

#endif
