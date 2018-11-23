#include "Timer.h"

class Temp
{
public:
    Temp(const char *name) : name(name)
    {
        std::cout << "Temp constructor with: " << name << std::endl;
    }
    ~Temp()
    {
        std::cout << "Temp destructor" << std::endl;
    }

    void testFuncMorParams(int a, double b, const char * word)
    {
        std::cout << "int: " << a << " double: " << b << " const char: " << word << " stringName:" << name <<  std::endl;
    }

    void testFuncNoParams()
    {
        std::cout << name << std::endl;
    }
private:
    std::string name;
};

int main(int argc, char *argv[])
{
    {
        int intNum = 2000;
        double doubleNum = 2.4;
        const char * str = "asdf";
        int interval = 500;
        Temp *temp = new Temp("My name");
        Timer<Temp, int, double, const char*> timer(interval, temp, &Temp::testFuncMorParams, intNum, doubleNum, str);
        timer.start();
        std::cout << "Sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        //timer.stop();
        delete temp;
    }
    std::cout << "Single shot ============" << std::endl;
    {
        int delay = 500;
        std::shared_ptr<Temp> temp(new Temp("Temp name"));
        Timer<Temp>::singleShot(delay, temp, &Temp::testFuncNoParams);
        std::cout << "Sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Timeout" << std::endl;

    return 0;
}
