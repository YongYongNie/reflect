#pragma once
#include <string>
#include <memory>

class IIReflectBase {
  public:
    virtual ~IIReflectBase() { }

  protected:
    IIReflectBase() { }

  private:
};

class IReflectObject {
  protected:
    class placeholder {
      public:
        virtual ~placeholder() { }

      public:
        virtual IIReflectBase* CreateValueType() const = 0;
    };

    template<typename ValueType>
    class holder : public placeholder {
      public:
        virtual IIReflectBase* CreateValueType() const {
            return new ValueType;
        }
    };

  public:
    virtual ~IReflectObject() { }

    virtual const std::wstring& GetName() const = 0;

    template<class T>
    auto CreateObject() const ->T* {
        T* pResult = nullptr;
        std::unique_ptr<placeholder> pHolder(CreateHolder());
        std::unique_ptr<IIReflectBase> pObject(pHolder->CreateValueType());

        pResult = dynamic_cast<T*>(pObject.get());
        if (pResult) {
            pObject.release();
        }
        return pResult;
    }
  protected:
    virtual placeholder* CreateHolder() const = 0;

  protected:
    IReflectObject() { }

  private:
};

template<class value_type>
class CReflectObject : public IReflectObject {
  public:
    virtual const std::wstring& GetName() const {
        return m_strName;
    }
    void SetName(const std::wstring& pName) {
        m_strName = pName;
    }
  protected:
    virtual placeholder* CreateHolder() const {
        return new holder<value_type>;
    }
  private:
    std::wstring m_strName;
};

void RegisterReflectObject(IReflectObject* pObj, int versionMajor, int versionMinor);
void UnRegisterReflectObject(IReflectObject* pObj, int versionMajor, int versionMinor);

IReflectObject* GetReflectObject(const std::wstring& pTypeName);

template<class T>
inline auto Ref_Dyn_CreateObject(const std::wstring& pTypeName) -> T* {
    IReflectObject* pobj = GetReflectObject(pTypeName);
    if (!pobj) {
        return nullptr;
    }
    return pobj->CreateObject<T>();
}

#define REF_DECLARE_DYNCREATE(class_name) \
	public: \
	static CReflectObject<class_name> class##class_name;


#define REF_IMPLEMENT_DYNCREATE(class_name, type_name, versionMajor, versionMinor)\
	CReflectObject<class_name> class_name::class##class_name;\
	struct RegisterObject##class_name {\
	     RegisterObject##class_name() {\
			class_name::class##class_name.SetName(type_name); \
			RegisterReflectObject(&class_name::class##class_name, versionMajor, versionMinor);\
		}\
        ~RegisterObject##class_name() {\
			UnRegisterReflectObject(&class_name::class##class_name, versionMajor, versionMinor);\
        }\
	};\
	static RegisterObject##class_name g_sRegisterObject##class_name;


#define REF_RUNTIME_DYNCREATE(class_name, type_name) Ref_Dyn_CreateObject<class_name>(type_name)