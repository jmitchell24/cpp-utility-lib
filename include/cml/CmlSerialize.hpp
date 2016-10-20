#pragma once

#include "CmlGroup.hpp"
#include "../Exception.hpp"

namespace util
{
namespace cml
{
    class SerializeException : public Exception
    {
    public:
        SerializeException(Segment const& message)
            : Exception(sprint(STR_LITERAL("(serialize error) #"), message))
        {}
    };
}

    inline void cmlSerialize(Segment const& name,          char      const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, unsigned char      const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name,          short     const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, unsigned short     const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name,          int       const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, unsigned int       const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name,          long      const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, unsigned long      const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name,          long long const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, unsigned long long const x, cml::Group& group) { group.getNamedItem(name).set(x); }

    inline void cmlSerialize(Segment const& name,      float  const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name,      double const x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, long double const x, cml::Group& group) { group.getNamedItem(name).set(x); }

    inline void cmlSerialize(Segment const& name, string_t const& x, cml::Group& group) { group.getNamedItem(name).set(x); }
    inline void cmlSerialize(Segment const& name, bool const x, cml::Group& group) { group.getNamedItem(name).set(x); }


    inline void cmlUnserialize(Segment const& name,          char     & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse char")); }
    inline void cmlUnserialize(Segment const& name, unsigned char     & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse unsigned char")); }
    inline void cmlUnserialize(Segment const& name,          short    & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse short")); }
    inline void cmlUnserialize(Segment const& name, unsigned short    & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse unsigned short")); }
    inline void cmlUnserialize(Segment const& name,          int      & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse int")); }
    inline void cmlUnserialize(Segment const& name, unsigned int      & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse unsigned int")); }
    inline void cmlUnserialize(Segment const& name,          long     & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse long")); }
    inline void cmlUnserialize(Segment const& name, unsigned long     & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse unsigned long")); }
    inline void cmlUnserialize(Segment const& name,          long long& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse long long")); }
    inline void cmlUnserialize(Segment const& name, unsigned long long& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse unsigned long long")); }

    inline void cmlUnserialize(Segment const& name,      float & x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse float")); }
    inline void cmlUnserialize(Segment const& name,      double& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse double")); }
    inline void cmlUnserialize(Segment const& name, long double& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse long double")); }

    inline void cmlUnserialize(Segment const& name, bool& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse char")); }
    inline void cmlUnserialize(Segment const& name, string_t& x, cml::Group const& group) { if (!group.getNamedItem(name).tryGet(x)) throw cml::SerializeException(STR_LITERAL("failed to parse string")); }


    inline void cmlUnserializeDefault(Segment const& name,          char     & x,          char      const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<char>(d); }
    inline void cmlUnserializeDefault(Segment const& name, unsigned char     & x, unsigned char      const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<unsigned char>(d); }
    inline void cmlUnserializeDefault(Segment const& name,          short    & x,          short     const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<short>(d); }
    inline void cmlUnserializeDefault(Segment const& name, unsigned short    & x, unsigned short     const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<unsigned short>(d); }
    inline void cmlUnserializeDefault(Segment const& name,          int      & x,          int       const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<int>(d); }
    inline void cmlUnserializeDefault(Segment const& name, unsigned int      & x, unsigned int       const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<unsigned int>(d); }
    inline void cmlUnserializeDefault(Segment const& name,          long     & x,          long      const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<long>(d); }
    inline void cmlUnserializeDefault(Segment const& name, unsigned long     & x, unsigned long      const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<unsigned long>(d); }
    inline void cmlUnserializeDefault(Segment const& name,          long long& x,          long long const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<long long>(d); }
    inline void cmlUnserializeDefault(Segment const& name, unsigned long long& x, unsigned long long const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<unsigned long long>(d); }

    inline void cmlUnserializeDefault(Segment const& name,      float & x,      float  const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<float>(d); }
    inline void cmlUnserializeDefault(Segment const& name,      double& x,      double const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<double>(d); }
    inline void cmlUnserializeDefault(Segment const& name, long double& x, long double const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<long double>(d); }

    inline void cmlUnserializeDefault(Segment const& name, bool& x, bool const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<bool>(d); }
    inline void cmlUnserializeDefault(Segment const& name, string_t& x, string_t const d, cml::Group const& group) { x = group.getNamedItem(name).getDefault<string_t>(d); }
}
