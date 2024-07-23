#include <bits/stdc++.h>
#include <cxxabi.h>
using namespace std;

template <typename T, typename... Args>
class DynamicFactory {
 public:
    static DynamicFactory& instance() {
        static DynamicFactory d;
        return d;
    }

    void regist(const string& name, function<shared_ptr<T>(Args&&...)> func) { _func_map.emplace(name, func); }

    shared_ptr<T> create(const string& name, Args... args) {
        auto it = _func_map.find(name);
        if (it == _func_map.end()) {
            return nullptr;
        } else {
            return it->second(std::forward<Args>(args)...);
        }
    }

 private:
    DynamicFactory() = default;
    DynamicFactory(const DynamicFactory&) = delete;
    DynamicFactory& operator=(const DynamicFactory&) = delete;

 private:
    unordered_map<string, function<shared_ptr<T>(Args&&...)>> _func_map;
};

template <typename T1, typename T2, typename... Args>
class DynamicCreator {
 public:
    // 在工厂中构造了<Base, Derive_func>
    // 这里的args是类型
    DynamicCreator() {
        DynamicFactory<T1, Args...>::instance().regist(get_demangle_name(), DynamicCreator::createObject);
    }

    static shared_ptr<T2> createObject(Args&&... args) { return make_shared<T2>(std::forward<Args>(args)...); }

    static string get_demangle_name() {
        char* demangle_name = abi::__cxa_demangle(typeid(T2).name(), nullptr, nullptr, nullptr);
        if (demangle_name != nullptr) {
            string ret(demangle_name);
            free(demangle_name);
            string _source("std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > ");
            string _source2("std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >");
            string _target("std::string");
            size_t pos = string::npos;
            while ((pos = ret.find(_source)) != std::string::npos) {
                ret = ret.replace(pos, _source.size(), _target.c_str(), _target.size());
            }
            while ((pos = ret.find(_source2)) != std::string::npos) {
                ret = ret.replace(pos, _source2.size(), _target.c_str(), _target.size());
            }
            return ret;
        }
        return "unknown";
    }
};

template <typename T>
class DynamicWorker {
 public:
    // 获取Base工厂里面用name_func构造的对象
    // 这里的args是数值型
    template <typename... Args>
    shared_ptr<T> create(const string& name, Args&&... args) {
        return DynamicFactory<T, Args...>::instance().create(name, std::forward<Args>(args)...);
    }
};

class A {
 public:
    A() = default;
    A(int i) { cout << i << " and "; }
    A(const string& s) { cout << s << " and "; }
    virtual void show() { cout << "A"; }
};

class B : public A {
 public:
    B() = default;
    B(int i) { cout << i << " and "; }
    B(const string& s) { cout << s << " and "; }
    void show() { cout << "B"; }
};

int main() {
    DynamicCreator<A, B, int> c;
    DynamicWorker<A> w;
    auto p = w.create("B", 1);
    // if (p != nullptr) {
    //     p->show();
    // } else {
    //     cout << "no obj" << endl;
    // }
}