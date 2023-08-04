
#include "Type.h"

#include <memory>

namespace ir
{
    std::shared_ptr<PrimitiveDataType> MakePrimitiveDataType(PrimitiveDataType::TypeID id)
    {
        return std::make_shared<PrimitiveDataType>(id);
    }

    std::shared_ptr<PointerType> MakePointerType(std::shared_ptr<Type> ref)
    {
        return std::make_shared<PointerType>(ref);
    }

    std::shared_ptr<Type> MakeType(std::shared_ptr<Type> ptr)
    {
        // moe_assert(dynamic_cast<PointerType *>(pointer.get()));
        return ptr->get();
    }

    std::shared_ptr<ArrayType> MakeArrayType(std::shared_ptr<Type> elementType, size_t len)
    {
        return std::make_shared<ArrayType>(elementType, len);
    }

    void PointerType::setInternalType(std::shared_ptr<Type> reftype)
    {
        if (ref_->isPointer())
        {
            std::dynamic_pointer_cast<PointerType>(ref_)->setInternalType(reftype);
        }
        else if (ref_->isArray())
        {
            std::dynamic_pointer_cast<ArrayType>(ref_)->setInternalType(reftype);
        }
        else
        {
            ref_ = reftype;
        }
    };

    std::shared_ptr<Type> PointerType::getInternalType()
    {
        if (ref_->isArray())
        {
            return std::dynamic_pointer_cast<ArrayType>(ref_)->getInternalType();
        }

        else if (ref_->isPointer())
        {
            return std::dynamic_pointer_cast<PointerType>(ref_)->getInternalType();
        }
        else
        {
            return ref_;
        }
    }

    std::shared_ptr<Type> PointerType::getReferenceType()
    {
        return ref_;
    }

    void ArrayType::setInternalType(std::shared_ptr<Type> eltype)
    {
        if (eltype_->isArray())
        {
            std::dynamic_pointer_cast<ArrayType>(eltype_)->setInternalType(eltype);
        }
        else if (eltype_->isPointer())
        {
            std::dynamic_pointer_cast<PointerType>(eltype_)->setInternalType(eltype);
        }
        else
        {
            eltype_ = eltype;
        }
        return;
    }

    std::shared_ptr<Type> ArrayType::getInternalType()
    {
        if (eltype_->isArray())
        {
            return std::dynamic_pointer_cast<ArrayType>(eltype_)->getInternalType();
        }

        else if (eltype_->isPointer())
        {
            return std::dynamic_pointer_cast<PointerType>(eltype_)->getInternalType();
        }
        else
        {
            return eltype_;
        }
    };

    std::shared_ptr<Type> ArrayType::getElementType()
    {
        return eltype_;
    }
}