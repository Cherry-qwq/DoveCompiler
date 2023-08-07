#include <cstdint>

namespace asmgen
{
    enum class OperandType
    {
        Error,
        VRegisterR,
        VRegisterS,
        Immediate,
        StackPointer,
        Label,
        None
    };

    class Operand
    {
    public:
        void setCount(int32_t count)
        {
            cnt = count;
        };

        int getCount() const
        {
            return cnt;
        };

        const int id;

        Operand();

        virtual OperandType getOpndType() const { return OperandType::Error; };

        virtual ~Operand() = default;

    protected:
        int cnt;
    };

}