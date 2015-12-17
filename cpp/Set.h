#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <sstream>

namespace Hyper{
template <class T>
class Set
{
public:
    inline Set(){data.clear();}
    inline Set(const Set<T> & _set ){this->data = _set.getDataCopy();}
    inline ~Set(){data.clear();}

    inline const std::vector<T>& getData(){return data;}
    inline void setData(std::vector<T> _data){data = _data;}

    inline T at(int i){return data[i];}
    inline T at(int i) const {return data[i];}
    inline void change(int _index, T _value){data[_index] = _value;}
    inline void change(int _index, T _value) const {data[_index] = _value;}
    inline void resize(int _newSize){ this->data.resize(_newSize);}
    inline void resize(int _newSize) const { this->data.resize(_newSize);}
    inline int size(){return data.size();}
    inline int size() const {return data.size();}
    inline const T& operator [](int i){return data[i];}
    inline std::vector<T> getDataCopy(){return data;}
    inline std::vector<T> getDataCopy() const {return data;}
    inline const T& operator [](int i) const {return data[i];}
    inline void clear(){ data.clear();}
    //inline void clear() const {data.clear();}
    inline void remove(long i){typename std::vector<T>::iterator it = data.begin(); it += i; data.erase(it);}
    inline long indexOf(T _val){for(long i = 0; i<data.size();i++) if(data[i] == _val) return i; return -1;}

    bool operator == (Set<T> _s2) const ;

    Set(std::vector<T> _data);
    Set<T> setIntersection(const Set<T> & _s2);
    bool setUnion(const Set<T> & _s2);
    bool setContains(const T & _value);
    void add(T _value);
    std::string toString();

private:
    std::vector<T> data;
    bool sort();
};

/////////////////////////Implementation ///////////////////////////

template <class T>
std::string Set<T>::toString(){
    std::stringstream ss;
    ss<<"[ ";
    for(long i = 0; i<data.size()-1; i++){

        ss << data[i] << ", ";
    }
    ss << data[data.size()-1] <<" ]";
    return ss.str();
}
template <class T>
void Set<T>::add(T _value){
    if(data.size() == 0){
        data.push_back(_value);
    }
    else{
        typename std::vector<T>::iterator pos = data.end();
        for(typename std::vector<T>::iterator it = data.begin(); it !=data.end(); it++){
            if(*it >= _value){
                pos = it;
                break;
            }
        }
        data.insert(pos, 1, _value  );
    }
}

template <class T>
Set<T>::Set(std::vector<T> _data)
{
    this->data = _data;
    this->sort();
}

template <class T>
Set<T> Set<T>::setIntersection(const Set<T> & _s2){
    Set<T> aux;
    long i=0, j=0;
    while((i<data.size()) && (j<_s2.size())){
        if(data[i] == _s2.at(j)){
            if(aux.size()>0 &&(aux.at(aux.size()-1) != data[i]))
                aux.add(data[i]);
            else{
                if(aux.size() <= 0)
                    aux.add(data[i]);
            }
            i++;
            j++;
        }
        else{
            if(data[i] < _s2.at(j)){
                i++;
            }
            else{
                j++;
            }
        }
    }
    return aux;
}

template <class T>
bool Set<T>::setUnion(const Set<T> &  _s2){
    if (data.size() == 0){
        data = _s2.getDataCopy();
        return true;
    }
    std::vector<T> aux;
    long i =0, j = 0;
    while((i<data.size()) && (j<_s2.size())){

        if(data[i] <_s2.at(j) ){
            if(aux.size()>0 && (aux[aux.size()-1] != data[i]))
                aux.push_back(data[i]);
            else{
                if(aux.size() <= 0)
                    aux.push_back(data[i]);
            }
            i++;
        }
        else{
            if(data[i]>_s2.at(j)){
                if(aux.size()>0 &&(aux[aux.size()-1] != _s2.at(j)))
                    aux.push_back(_s2.at(j));
                else{
                    if(aux.size() <= 0)
                        aux.push_back(_s2.at(j));
                }
                j++;
            }
            else{
                if(aux.size()>0 &&(aux[aux.size()-1] != data[i]))
                    aux.push_back(data[i]);
                else{
                    if(aux.size() <= 0)
                        aux.push_back(data[i]);
                }
                i++;
                j++;
            }
        }
    }
    for(;i<data.size();i++){
        if(data[i] != aux[aux.size()-1])
            aux.push_back(data[i]);
    }
    for(;j<_s2.size();j++){
        if(_s2.at(j) != aux[aux.size()-1])
            aux.push_back(_s2.at(j));
    }
    data.clear();
    data = aux;
    return true;
}

template <class T>
bool Set<T>::setContains(const T & _value){
    long i = 0;
    while(i<data.size() && data[i]<=_value){
        if (data[i] == _value)
            return true;
        i++;
    }
    return false;
}

template <class T>
bool Set<T>::operator == (Set<T> _s2) const {
    if (_s2.size() != this->size())
        return false;
    for(unsigned long i = 0; i<this->size(); i++)
        if(this->at(i) != _s2.at(i))
            return false;
    return true;
}

template <class T>
bool Set<T>::sort(){
    std::vector<T> res;
    for(long i = 0; i<data.size(); i++){
        long m = i;
        for(long j = i; j<data.size(); j++){
            if(data[m] >= data[j])
                m = j;
        }
        res.push_back(data[m]);
    }
    data =res;
}

}
#endif // SET_H
