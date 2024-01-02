#include <iostream>
#include <memory>
#include <string>

class MyBaseClass {
public:
    MyBaseClass(const std::string& value) : m_value(value) {}
    virtual ~MyBaseClass() = default;
    virtual void Print() const {
        std::cout << "Base Class Value: " << m_value << std::endl;
    }
protected:
    std::string m_value;
};

class MyDerivedClass : public MyBaseClass {
public:
    MyDerivedClass(const std::string& value) : MyBaseClass(value) {}
    void Print() const override {
        std::cout << "Derived Class Value: " << m_value << std::endl;
    }
};

template <typename T>
class MyUniquePtr {
public:
    explicit MyUniquePtr(T* ptr = nullptr) : m_ptr(ptr) {}
    ~MyUniquePtr() {
        delete m_ptr;
    }
    T* operator->() const {
        return m_ptr;
    }
    T* get() const {
        return m_ptr;
    }
    T* release() {
        T* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }
    void reset(T* ptr = nullptr) {
        delete m_ptr;
        m_ptr = ptr;
    }
    void swap(MyUniquePtr& other) {
        std::swap(m_ptr, other.m_ptr);
    }
private:
    T* m_ptr;
};

template <typename T>
class MySharedPtr {
public:
    explicit MySharedPtr(T* ptr = nullptr) : m_ptr(ptr), m_refCount(new size_t(1)) {}
    MySharedPtr(const MySharedPtr& other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount) {
        ++(*m_refCount);
    }
    ~MySharedPtr() {
        --(*m_refCount);
        if (*m_refCount == 0) {
            delete m_ptr;
            delete m_refCount;
        }
    }
    T* operator->() const {
        return m_ptr;
    }
    T* get() const {
        return m_ptr;
    }
    size_t use_count() const {
        return *m_refCount;
    }
    void reset(T* ptr = nullptr) {
        if (ptr == m_ptr) return;
        --(*m_refCount);
        if (*m_refCount == 0) {
            delete m_ptr;
            delete m_refCount;
        }
        m_ptr = ptr;
        m_refCount = new size_t(1);
    }
    void swap(MySharedPtr& other) {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_refCount, other.m_refCount);
    }
private:
    T* m_ptr;
    size_t* m_refCount;
};

int main() {
    MyBaseClass* base = new MyBaseClass("Base Value");
    base->Print();
    {
        MyUniquePtr<MyBaseClass> unique(base);
        unique->Print();
    }
    {
        MySharedPtr<MyBaseClass> shared(new MyBaseClass("Shared Value"));
        shared->Print();
        {
            MySharedPtr<MyBaseClass> sharedCopy(shared);
            sharedCopy->Print();
        }
    }
    MyDerivedClass* derived = new MyDerivedClass("Derived Value");
    derived->Print();
    {
        MySharedPtr<MyDerivedClass> shared(derived);
        shared->Print();
    }
    return 0;
}