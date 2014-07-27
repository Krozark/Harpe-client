#include <QApplication>
#include <patcher/Patcher.hpp>

extern "C" {
    int _get_output_format( void ){ return 0; } //to avoid linker error on windows
}


int main(int argc,char* argv[])
{
    QApplication App(argc, argv);

    patcher::Patcher patcher("harpe-client");
    patcher.show();

    patcher.start();

    return App.exec();
}
