#include "String.h"

#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <cwctype>
#include <iostream>

String::~String() {
    if (cbuffer != nullptr) { delete[] cbuffer; }
}

String::String() {
    cbuffer = new char[24];
    capacity = 24;
    cbuffer[0]='\0';
    strSize = 0;
    genHashCode();
}

String::String(const String& a) {
    int len = a.size();
    cbuffer = nullptr;
    capacity = len+1;
    memcpy(cbuffer,a.cstr(),(a.size()+1)*sizeof(char));
    strSize = len;
    genHashCode();
}

String::String(const char* cstri) {
    int len = (int)strlen(cstri);
    cbuffer = new char[len+1];
    capacity = len+1;
    memcpy(cbuffer, cstri, (len+1)*sizeof(char));
    strSize = len;
    genHashCode();
}

String::String(const std::string& cppstr) {
    int len = (int)cppstr.size();
    cbuffer = new char[len+1];
    capacity = len+1;
    memcpy(cbuffer, cppstr.c_str(), (len+1)*sizeof(char));
    strSize = len;
    genHashCode();
}

String::String(const String& a,const String& b) {
    int len = a.size()+b.size();
    cbuffer = new char[len+1];
    capacity = len+1;
    memcpy(cbuffer, a.cstr(), a.size() * sizeof(char));
    memcpy(cbuffer+a.size(), b.cstr(), (b.size()+1) * sizeof(char));
    strSize = len;
    genHashCode();
}

String::String(char c) {
    cbuffer = new char[2];
    capacity = 2;
    cbuffer[0] = c; cbuffer[1]='\0';
    strSize = 1;
    genHashCode();
}

String::String(int i,bool hex) {
    cbuffer = new char[32];
    capacity = 32;
    if (hex) {
        snprintf(cbuffer,32,"%#010x",i);
    } else {
        snprintf(cbuffer,32,"%d",i);
    }
    strSize = (int)strlen(cbuffer);
    genHashCode();
}

String::String(float f) {
    cbuffer = new char[32];
    capacity = 32;
    snprintf(cbuffer,32,"%f",f);
    strSize = (int)strlen(cbuffer);
    genHashCode();
}

void String::genHashCode() {
    hashCode = 5381;
    for (int i=0;i<strSize;i++) {
        hashCode = ((hashCode << 5) + hashCode) + cbuffer[i];
    }
}

String& String::operator=(const String& other) {
    if (&other == this) return *this;
    if (cbuffer!=nullptr) { delete[] cbuffer; }
    cbuffer = new char[other.capacity];
    memcpy(cbuffer,other.cstr(),(other.size()+1)*sizeof(char));
    capacity = other.capacity;
    strSize = other.size();
    hashCode = other.getHashCode();
    return *this;
}

const String operator+(const String& a, const String& b) {
    return String(a, b);
}

const String operator+(const char* a, const String& b) {
    return String(String(a), b);
}

bool operator<(const String& a, const String& b) {
    return strcmp(a.cstr(), b.cstr()) < 0;
}

bool operator>(const String& a, const String& b) {
    return strcmp(a.cstr(), b.cstr()) > 0;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    return os.write(s.cstr(),s.size());
}

long long String::getHashCode() const {
    return hashCode;
}

bool String::equals(const String& other) const {
    if (other.size()!=size()) { return false; }
    return other.getHashCode()==getHashCode();
}

bool String::isEmpty() const {
    return strSize == 0;
}

int String::size() const {
    return strSize;
}

int String::findFirst(const String& fnd, int from) const {
    if (fnd.size() == 0) { return -1; }
    if (from<0) { from = 0; }
    for (int i = from; i <= size()-fnd.size(); i++) {
        if (memcmp(fnd.cstr(), cstr() + i, fnd.size() * sizeof(char)) == 0) { return i; }
    }
    return -1;
}

int String::findLast(const String& fnd, int from) const {
    if (fnd.size() == 0) { return -1; }
    if (from<0) { from = size(); }
    for (int i = from - fnd.size(); i >= 0; i--) {
        if (memcmp(fnd.cstr(), cstr() + i, fnd.size() * sizeof(char)) == 0) { return i; }
    }
    return -1;
}

const char* String::cstr() const {
    return cbuffer;
}

int String::toInt() const {
    return atoi(cbuffer);
}

float String::toFloat() const {
    return (float)atof(cbuffer);
}

String String::substr(int start,int cnt) const {
    if ((cnt<0) || (cnt+start>=size())) {
        cnt = size()-start;
    }

    char* newBuf = new char[cnt+1];
    newBuf[cnt]=L'\0';
    memcpy(newBuf,cstr()+start,cnt*sizeof(char));

    String retVal(newBuf);
    delete[] newBuf;
    return retVal;
}

char String::charAt(int pos) const {
    return (char)(cstr()[pos]);
}

String String::replace(const String& fnd,const String& rplace) const {
    if (fnd.size()==0) { return *this; }

    int newSize = strSize;
    for (int i=0;i<strSize-fnd.size()+1;) {
        if (memcmp(fnd.cstr(),cstr()+i,fnd.size()*sizeof(char))==0) {
            newSize+=rplace.size()-fnd.size();
            i+=fnd.size();
        } else {
            i++;
        }
    }

    char* newBuf = new char[newSize+1];
    int i=0; int j=0;
    while (i<strSize) {
        bool found = i<strSize-fnd.size()+1;
        if (found) {
            found = memcmp(fnd.cstr(),cstr()+i,fnd.size()*sizeof(char))==0;
        }
        if (found) {
            memcpy(newBuf+j,rplace.cstr(),rplace.size()*sizeof(char));
            i+=fnd.size(); j+=rplace.size();
        } else {
            newBuf[j]=cstr()[i];
            i++; j++;
        }
    }
    newBuf[j]=L'\0';

    String retVal(newBuf);
    delete[] newBuf;
    return retVal;
}

String String::toUpper() const {
    char* newBuf = new char[capacity];
    for (int i = 0; i<strSize; i++) {
        newBuf[i] = toupper(cbuffer[i]);
    }
    newBuf[strSize] = L'\0';

    String retVal(newBuf);
    delete[] newBuf;
    return retVal;
}

String String::toLower() const {
    char* newBuf = new char[capacity];
    for (int i = 0; i<strSize; i++) {
        newBuf[i] = tolower(cbuffer[i]);
    }
    newBuf[strSize] = L'\0';

    String retVal(newBuf);
    delete[] newBuf;
    return retVal;
}

String String::trim() const {
    if (size()==0) { return ""; }

    char* newBuf = new char[capacity];
    int leadingPos = 0;
    while (charAt(leadingPos) == ' ' || charAt(leadingPos) == '\t') {
        leadingPos++;
        if (leadingPos>=size()) {
            return *this;
        }
    }
    int trailingPos = size()-1;
    while (charAt(trailingPos) == ' ' || charAt(trailingPos) == '\t') {
        trailingPos--;
        if (trailingPos<0) {
            return *this;
        }
    }

    int newLength = 1;
    newBuf[0] = charAt(leadingPos);
    for (int i=leadingPos+1;i<=trailingPos;i++) {
        if (charAt(i-1)!=' ' || charAt(i)!=' ') {
            newBuf[newLength]=charAt(i);
            newLength++;
        }
    }
    newBuf[newLength]=L'\0';

    String retVal = newBuf;
    delete[] newBuf;
    return retVal;
}

std::vector<String> String::split(const String& needle, bool removeEmptyEntries) const {
    std::vector<String> retVal;
    String haystack = String(*this);
    while (haystack.findFirst(needle) > -1) {
        String adder = haystack.substr(0, haystack.findFirst(needle));
        retVal.push_back(adder);
        haystack = haystack.substr(adder.size()+needle.size());
    }
    // Add the rest of the string to the vector.
    retVal.push_back(haystack);

    if (removeEmptyEntries) {
        for (int i = 0; i < retVal.size(); i++) {
            if (retVal[i].isEmpty()) {
                retVal.erase(retVal.begin() + i);
                i--;
            }
        }
    }

    return retVal;
}

String String::join(const std::vector<String>& vect, const String& separator) {
    if (vect.size() <= 0) {
        return String("");
    }

    String retVal = vect[0];
    for (int i = 1; i < vect.size(); i++) {
        retVal = retVal + separator + vect[i];
    }

    return retVal;
}

String String::resourcePath() const {
#if defined(__APPLE__) && defined(__OBJC__)
    String dummyName = "Dummy.txt";
    int period = dummyName.findFirst(".");
    NSString* name = dummyName.substr(0, period).nsstr();
    NSString* ext = dummyName.substr(period+1).nsstr();

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* path = [bundle pathForResource: name ofType: ext];

    if (path == nullptr) {
        return String("");
    }

    // Manipulate the resulting cString.
    const char* cPath = [path cStringUsingEncoding: NSUTF8StringEncoding];
    String strPath = String(cPath);
    strPath = strPath.substr(0, strPath.size() - dummyName.size());
    strPath = strPath + (*this);

    return strPath;
#endif
    return *this;
}

String String::unHex() const {
    int isUnhexing = 0;

    char resultBuffer[64];
    int resultSize = 0;

    int tempAscVal = 0;
    for (int i=0;i<size();i++) {
        if (isUnhexing==0) {
            if (cbuffer[i]==L'%') {
                isUnhexing = 1;
            } else {
                resultBuffer[resultSize]=cbuffer[i];
                resultSize++;
            }
        } else {
            int toDec = 0;
            if (cbuffer[i]>=L'0' && cbuffer[i]<=L'9') {
                toDec = cbuffer[i]-L'0';
            } else if (cbuffer[i]>=L'a' && cbuffer[i]<=L'f') {
                toDec = cbuffer[i]+10-L'a';
            } else if (cbuffer[i]>=L'A' && cbuffer[i]<=L'F') {
                toDec = cbuffer[i]+10-L'A';
            }
            if (isUnhexing==1) {
                tempAscVal = toDec<<4;
                isUnhexing=2;
            } else if (isUnhexing==2) {
                tempAscVal |= toDec;
                resultBuffer[resultSize]=tempAscVal;
                resultSize++;
                isUnhexing=0;
            }
        }
    }
    resultBuffer[resultSize]='\0';
    return String(resultBuffer);
}
