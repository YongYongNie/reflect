#include "stdafx.h"
#include "Reflect.h"
#include <map>
#include <set>

struct TypeInfo {
    TypeInfo()
        : major(1)
        , minor(0) {

    }

    int major;
    int minor;
    std::wstring classID;

    bool operator < (const TypeInfo& other) const {
        if (classID != other.classID) {
            return classID < other.classID;
        }
        if (major != other.major) {
            return major < other.major;
        }
        return minor < other.minor;
    }
};

struct ReflectInfo {
    std::map<std::wstring, std::set<TypeInfo>> classIDMapRTTi; // 运行时类型id与类型的关系
    std::map<TypeInfo, IReflectObject*> typeMap;
};

static inline ReflectInfo& GetRefMag() {
    static ReflectInfo g_reflectInfo;
    return g_reflectInfo;
}

void RegisterReflectObject(IReflectObject* pObj, int versionMajor, int versionMinor) {
    TypeInfo info;

    info.major = versionMajor;
    info.minor = versionMinor;
    info.classID = pObj->GetName();

    GetRefMag().classIDMapRTTi[pObj->GetName()].insert(info);
    GetRefMag().typeMap[info] = pObj;
}

void UnRegisterReflectObject(IReflectObject* pObj, int versionMajor, int versionMinor) {
    TypeInfo info;

    info.major = versionMajor;
    info.minor = versionMinor;
    info.classID = pObj->GetName();

    GetRefMag().typeMap.erase(info);
    GetRefMag().classIDMapRTTi[info.classID].erase(info);
}

IReflectObject* GetReflectObject(const std::wstring& pTypeName) {
    TypeInfo info;

    auto itorRtti = GetRefMag().classIDMapRTTi.find(pTypeName);
    if (itorRtti != GetRefMag().classIDMapRTTi.end()) {
        if (!itorRtti->second.empty()) {
            info = *itorRtti->second.rbegin();
        }
    }

    auto ptem = GetRefMag().typeMap.find(info);
    if (ptem != GetRefMag().typeMap.end()) {
        return ptem->second;
    }
    return nullptr;
}
