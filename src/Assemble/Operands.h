
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
    protected:
        static int cnt;

    public:
        const int id;

        Operand();

        virtual OperandType getOpndType() const { return OperandType::Error; };

        virtual ~Operand() = default;
    };

    

}