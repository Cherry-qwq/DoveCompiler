
#include <cstdint>

namespace utils
{
  class Counter
  {
  public:
    Counter() : counter_(0){};
    Counter(int32_t init) : counter_(init){};
    int32_t next()
    {
      return counter_++;
    }
    int32_t get()
    {
      return counter_;
    }

  protected:
    int32_t counter_;
  };
} // namespace 
