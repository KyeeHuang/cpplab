
#include <algorithm>
#include <vector>
class Base {
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    virtual void mf4() const;
};

class Derived : public Base {
public:
    virtual void mf1() const override;
    virtual void mf2(unsigned int x);
    virtual void mf3() &&;
    void mf4() const override;
};

class Widget {
public:
    using DataType = std::vector<double>;
    
    DataType& data() & {return values;}
    DataType data() && { return std::move(values); }

private:
    DataType values;
};

int main() {
    Widget make_widget();
    auto value = make_widget().data();

    return 0;
}
