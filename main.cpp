#ifdef _WIN32
#include <windows.h>
#include"test_planet.h"
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

#include "Widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QTest::qExec(new test_Planet, argc, argv);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
