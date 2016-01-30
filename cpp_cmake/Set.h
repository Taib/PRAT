#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <sstream>

namespace Hyper{
template <class T>
class Set
{
  /* This class provides basic set operation function.
   * Note : A set do not contain duplication.
   * Here, a set is a vector of elements.
   */
public:
    typedef T ValueType;
    inline Set(){data.clear();}
    inline Set(T _value){data.push_back(_value);}
    inline Set(const Set<T> & _set ){this->data = _set.getDataCopy();}
    inline ~Set(){if(!data.empty()) data.clear();}

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
    inline std::vector<T> getDataCopy(){ std::vector<T> res = std::vector<T> (data); return res;}
    inline std::vector<T> getDataCopy() const {std::vector<T> res = std::vector<T> (data); return res;}
    inline const T& operator [](int i) const {return data[i];}
    inline void clear(){ if(!data.empty()) data.clear();}
    //inline void clear() const {data.clear();}
    inline void remove(long i){typename std::vector<T>::iterator it = data.begin(); it += i; data.erase(it);}
    inline long indexOf(T _val){for(long i = 0; i<data.size();i++) if(data[i] == _val) return i; return -1;}

    bool operator == (Set<T> _s2) const ;
    void operator =(Set<T> _s2) { this->clear(); this->data = _s2.getDataCopy();}

    Set(std::vector<T> _data);
    Set<T> setIntersection(const Set<T> & _s2);
    bool setUnion(const Set<T> & _s2);
    bool setContains(const T & _value);
    int add(T _value);
    std::string toString();

protected:
    std::vector<T> data;
    bool sort();
};

/////////////////////////Implementation ///////////////////////////



template <class T>
std::string Set<T>::toString(){
    std::stringstream ss;
    //ss<<"[ ";
    for(long i = 0; i<data.size(); i++){

        ss << data[i] +1<< " ";
    }
    //ss << data[data.size()-1] <<" ]";
    return ss.str();
}
template <class T>
int Set<T>::add(T _value){
    if(data.empty()){
        data.push_back(_value);
        return 0;
    }
    else{
        //typename std::vector<T>::iterator pos = data.end();
        typename std::vector<T>::iterator it = data.begin();
        long i = 0;
        for(; i<data.size(); i++, it++){
            if(data[i] >= _value){
                //pos = it;
                break;
            }
        }
        //long i = *it;
        if(i<data.size() && data[i] != _value) {
            data.insert(it, 1, _value  );
            return *it;
        }
        else
            if (i>=data.size()){
                data.insert(it, 1, _value);
                return *it;
            }

    }
    return -1;
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
    Set<T> aux;
    long i =0, j = 0;
    while((i<data.size()) && (j<_s2.size())){

        if(data[i] <_s2.at(j) ){
            if(aux.size()>0 && (aux[aux.size()-1] != data[i]))
                aux.add(data[i]);
            else{
                if(aux.size() <= 0)
                    aux.add(data[i]);
            }
            i++;
        }
        else{
            if(data[i]>_s2.at(j)){
                if(aux.size()>0 &&(aux[aux.size()-1] != _s2.at(j)))
                    aux.add(_s2.at(j));
                else{
                    if(aux.size() <= 0)
                        aux.add(_s2.at(j));
                }
                j++;
            }
            else{
                if(aux.size()>0 &&(aux[aux.size()-1] != data[i]))
                    aux.add(data[i]);
                else{
                    if(aux.size() <= 0)
                        aux.add(data[i]);
                }
                i++;
                j++;
            }
        }
    }
    for(;i<data.size();i++){
        if(data[i] != aux[aux.size()-1])
            aux.add(data[i]);
    }
    for(;j<_s2.size();j++){
        if(_s2.at(j) != aux[aux.size()-1])
            aux.add(_s2.at(j));
    }
    data.clear();
    data = aux.getDataCopy();
    aux.clear();
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
    return true;
}

}
#endif // SET_H

/*  @Tramx   */


