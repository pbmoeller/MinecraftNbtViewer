#include <QApplication>
#include <QWidget>

#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;



    QApplication app(argc, argv);
    QWidget widget;
    widget.show();
    return app.exec();
}