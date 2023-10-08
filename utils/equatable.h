#ifndef EQUATABLE_H
#define EQUATABLE_H

template<typename T>
class Equatable {
public:
    virtual bool operator== (T const& other) const =0;
    virtual bool operator!= (T const& other) const =0;
};

#endif // EQUATABLE_H
