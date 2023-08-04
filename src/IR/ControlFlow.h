#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "Instruction.h"
namespace ir
{

    class BasicBlock;
    class CompUnit : public User
    {
    public:
        CompUnit(std::shared_ptr<Type> type, std::string name)
            : User(std::move(type), name){};
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "CompUnit " + getName() + " ";
            helper.add(output);
            helper.indent();
            for (auto global_object : global_objects_)
            {
                global_object.getValue()->dump(helper);
            }
            helper.unindent();
            return output;
        }

        void addGlobalObject(std::shared_ptr<User> global_object)
        {
            global_objects_.push_back(Use(std::move(global_object), this));
        }

        std::shared_ptr<std::vector<Use>> getGlobalObject()
        {
            return std::make_shared<std::vector<Use>>(global_objects_);
        }

    protected:
        std::vector<Use> global_objects_;
    };

    class Function : public User
    {
    public:
        Function(std::shared_ptr<Type> type, std::string name)
            : User(std::move(type), std::move(name)) { is_function_ = true; };
        std::string dump(DumpHelper &helper) const override;
        void addBasicBlock(std::shared_ptr<BasicBlock> basic_block);
        std::shared_ptr<std::vector<std::shared_ptr<BasicBlock>>> getBasicBlocks();

    protected:
        std::vector<std::shared_ptr<BasicBlock>> basic_blocks_;
    };

    class Icmp : public Instruction
    {
    public:
        enum class ICompId
        {
            EQ,
            NE,
            GT,
            GE,
            LT,
            LE,
            Default,
        };

        explicit Icmp(ICompId cid, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, std::string name)
            : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), lhs_(lhs, this), rhs_(rhs, this), comp_id_(cid){};

        std::string getOperatorLiterial(ICompId id) const
        {
            switch (id)
            {
            case ICompId::EQ:
                return "==";
            case ICompId::NE:
                return "!=";
            case ICompId::GT:
                return ">";
            case ICompId::GE:
                return ">=";
            case ICompId::LT:
                return "<";
            case ICompId::LE:
                return "<=";
            default:
                throw std::runtime_error("Unknown ICompId");
            }
        }

        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Icmp " + getOperatorLiterial(comp_id_) + " " + lhs_.getValue()->getName() + " " + rhs_.getValue()->getName() + " ";
            helper.add(output);
            return output;
        }

    protected:
        Use lhs_;
        Use rhs_;
        ICompId comp_id_ = ICompId::EQ;
    };

    class Fcmp : public Instruction
    {
    public:
        enum class FCompId
        {
            EQ,
            NE,
            GT,
            GE,
            LT,
            LE,
            Default,
        };

        explicit Fcmp(FCompId cid, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, std::string name)
            : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), lhs_(lhs, this), rhs_(rhs, this), comp_id_(cid){};

        std::string getOperatorLiterial(FCompId id) const
        {
            switch (id)
            {
            case FCompId::EQ:
                return "==";
            case FCompId::NE:
                return "!=";
            case FCompId::GT:
                return ">";
            case FCompId::GE:
                return ">=";
            case FCompId::LT:
                return "<";
            case FCompId::LE:
                return "<=";
            default:
                throw std::runtime_error("Unknown FCompId");
            }
        }

        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Fcmp " + getOperatorLiterial(comp_id_) + " " + lhs_.getValue()->getName() + " " + rhs_.getValue()->getName() + " ";
            helper.add(output);
            return output;
        }

    protected:
        Use lhs_;
        Use rhs_;
        FCompId comp_id_ = FCompId::EQ;
    };

    class Return : public Instruction
    {
    public:
        Return(std::shared_ptr<User> retVal, std::string name)
            : Instruction(retVal->getType(), name, 1), ret_val_(retVal, this)
        {
            is_terminate_inst_ = true;
        };
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Return " + ret_val_.getValue()->getName() + " ";
            helper.add(output);
            return output;
        }

    protected:
        Use ret_val_;
    };

    class JPLabel : public Instruction
    {
    public:
        explicit JPLabel(std::string label_name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), label_name, 0), label_name_(label_name){};
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "JPLabel " + getName() + ": ";
            helper.add(output);
            return output;
        }

    protected:
        std::string label_name_;
    };

    class Label : public Instruction
    {
    public:
        explicit Label(std::shared_ptr<JPLabel> jplabel) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), jplabel->getName(), 0), jplabel_(jplabel){};
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Label " + getName() + "";
            // helper.add(output);
            return output;
        }

        std::shared_ptr<JPLabel> getJPLabel()
        {
            return jplabel_;
        }

    protected:
        std::shared_ptr<JPLabel> jplabel_;
    };

    class Phi : public Instruction
    {
        // TODO
    protected:
        std::map<std::shared_ptr<JPLabel>, std::shared_ptr<Value>> vecPair;
    };

    class Call : public Instruction
    {
    public:
        explicit Call(std::shared_ptr<Function> callee, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), callee_(callee){};
        explicit Call(std::shared_ptr<Function> callee, std::vector<std::shared_ptr<Value>> args, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2), callee_(callee)
        {
            for (auto &arg : args)
            {
                args_.push_back(Use(arg, this));
            }
        };

    protected:
        std::shared_ptr<Function> callee_;
        std::vector<Use> args_ = std::vector<Use>();
    };

    class Br : public Instruction
    {
    public:
        Br(std::shared_ptr<Value> condition, std::shared_ptr<Label> t_label, std::shared_ptr<Label> f_label, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 1), has_condition_(true), condition_(condition, this), t_label_(t_label, this), f_label_(f_label, this)
        {
            is_terminate_inst_ = true;
        };
        Br(std::shared_ptr<Label> t_label, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), name, 0), t_label_(t_label, this)
        {
            is_terminate_inst_ = true;
        };
        std::string dump(DumpHelper &helper) const
        {
            std::string output = "Br ";
            if (has_condition_)
            {
                output += condition_.getValue()->dump(helper) + ", " + t_label_.getValue()->dump(helper) + ", " + f_label_.getValue()->dump(helper) + " ";
            }
            else
            {
                output += t_label_.getValue()->dump(helper) + " ";
            }
            helper.add(output);
            return output;
        }
        bool hasCondition()
        {
            return has_condition_;
        }
        void setCondition(std::shared_ptr<Value> condition)
        {
            condition_ = Use(condition, this);
            has_condition_ = true;
        }
        std::shared_ptr<Value> getCondition()
        {
            return condition_.getValue();
        }

        std::shared_ptr<Label> getTLabel()
        {
            return std::dynamic_pointer_cast<Label>(t_label_.getValue());
        }
        std::shared_ptr<Label> getFLabel()
        {
            return std::dynamic_pointer_cast<Label>(f_label_.getValue());
        }

    protected:
        bool has_condition_ = false;
        Use condition_ = Use();
        Use t_label_ = Use();
        Use f_label_ = Use();
    };
}
