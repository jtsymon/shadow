/* 
 * File:   resources.h
 * Author: jts
 *
 * Created on 25 June 2014, 3:58 PM
 */

#ifndef RESOURCES_H
#define	RESOURCES_H

#include <string>
#include <unordered_map>

template <class T>
class Resources {
    std::unordered_map<std::string, T> internal_map;
public:
    Resources();
    ~Resources();
    T get(const std::string &key);
    void set(const std::string &key, T value);

    T& operator[](const std::string &key);
    const T& operator[](const std::string &key) const;
};

template <class T>
Resources<T>::Resources() {

}

template <class T>
Resources<T>::~Resources() {
    delete this->internal_map;
}

template <class T>
T Resources<T>::get(const std::string &key) {
    return this->internal_map.at[key];
}

template <class T>
void Resources<T>::set(const std::string &key, T value) {
    this->internal_map[key] = value;
}

template <class T>
T& Resources<T>::operator[](const std::string &key) {
    return this->internal_map[key];
}

template <class T>
const T& Resources<T>::operator[](const std::string &key) const {
    return this->internal_map[key];
}

#endif	/* RESOURCES_H */

