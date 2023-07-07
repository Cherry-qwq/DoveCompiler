namespace IR
{
  class TypeI
  {
  public:
    virtual ~TypeI() = default;
  };
  
  class Type : public TypeI
  {
    enum TypeID
    {

    };
  };
}
