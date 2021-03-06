//
// Generated file, do not edit! Created by nedtool 4.6 from src/Base/Messages/icancloud_MPI_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "icancloud_MPI_Message_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

icancloud_MPI_Message_Base::icancloud_MPI_Message_Base(const char *name, int kind) : ::icancloud_App_NET_Message(name,kind)
{
    this->sourceRank_var = 0;
    this->destRank_var = 0;
    this->fileName_var = "";
    this->offset_var = 0;
}

icancloud_MPI_Message_Base::icancloud_MPI_Message_Base(const icancloud_MPI_Message_Base& other) : ::icancloud_App_NET_Message(other)
{
    copy(other);
}

icancloud_MPI_Message_Base::~icancloud_MPI_Message_Base()
{
}

icancloud_MPI_Message_Base& icancloud_MPI_Message_Base::operator=(const icancloud_MPI_Message_Base& other)
{
    if (this==&other) return *this;
    ::icancloud_App_NET_Message::operator=(other);
    copy(other);
    return *this;
}

void icancloud_MPI_Message_Base::copy(const icancloud_MPI_Message_Base& other)
{
    this->sourceRank_var = other.sourceRank_var;
    this->destRank_var = other.destRank_var;
    this->fileName_var = other.fileName_var;
    this->offset_var = other.offset_var;
}

void icancloud_MPI_Message_Base::parsimPack(cCommBuffer *b)
{
    ::icancloud_App_NET_Message::parsimPack(b);
    doPacking(b,this->sourceRank_var);
    doPacking(b,this->destRank_var);
    doPacking(b,this->fileName_var);
    doPacking(b,this->offset_var);
}

void icancloud_MPI_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::icancloud_App_NET_Message::parsimUnpack(b);
    doUnpacking(b,this->sourceRank_var);
    doUnpacking(b,this->destRank_var);
    doUnpacking(b,this->fileName_var);
    doUnpacking(b,this->offset_var);
}

unsigned int icancloud_MPI_Message_Base::getSourceRank() const
{
    return sourceRank_var;
}

void icancloud_MPI_Message_Base::setSourceRank(unsigned int sourceRank)
{
    this->sourceRank_var = sourceRank;
}

unsigned int icancloud_MPI_Message_Base::getDestRank() const
{
    return destRank_var;
}

void icancloud_MPI_Message_Base::setDestRank(unsigned int destRank)
{
    this->destRank_var = destRank;
}

const char * icancloud_MPI_Message_Base::getFileName() const
{
    return fileName_var.c_str();
}

void icancloud_MPI_Message_Base::setFileName(const char * fileName)
{
    this->fileName_var = fileName;
}

int icancloud_MPI_Message_Base::getOffset() const
{
    return offset_var;
}

void icancloud_MPI_Message_Base::setOffset(int offset)
{
    this->offset_var = offset;
}

class icancloud_MPI_MessageDescriptor : public cClassDescriptor
{
  public:
    icancloud_MPI_MessageDescriptor();
    virtual ~icancloud_MPI_MessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(icancloud_MPI_MessageDescriptor);

icancloud_MPI_MessageDescriptor::icancloud_MPI_MessageDescriptor() : cClassDescriptor("icancloud_MPI_Message", "icancloud_App_NET_Message")
{
}

icancloud_MPI_MessageDescriptor::~icancloud_MPI_MessageDescriptor()
{
}

bool icancloud_MPI_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<icancloud_MPI_Message_Base *>(obj)!=NULL;
}

const char *icancloud_MPI_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int icancloud_MPI_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int icancloud_MPI_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *icancloud_MPI_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sourceRank",
        "destRank",
        "fileName",
        "offset",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int icancloud_MPI_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceRank")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destRank")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileName")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "offset")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *icancloud_MPI_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
        "string",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *icancloud_MPI_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int icancloud_MPI_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    icancloud_MPI_Message_Base *pp = (icancloud_MPI_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string icancloud_MPI_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    icancloud_MPI_Message_Base *pp = (icancloud_MPI_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSourceRank());
        case 1: return ulong2string(pp->getDestRank());
        case 2: return oppstring2string(pp->getFileName());
        case 3: return long2string(pp->getOffset());
        default: return "";
    }
}

bool icancloud_MPI_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    icancloud_MPI_Message_Base *pp = (icancloud_MPI_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceRank(string2ulong(value)); return true;
        case 1: pp->setDestRank(string2ulong(value)); return true;
        case 2: pp->setFileName((value)); return true;
        case 3: pp->setOffset(string2long(value)); return true;
        default: return false;
    }
}

const char *icancloud_MPI_MessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *icancloud_MPI_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    icancloud_MPI_Message_Base *pp = (icancloud_MPI_Message_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


