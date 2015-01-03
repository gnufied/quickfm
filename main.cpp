#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
//#include <QDebug>

#include "file_manager.hpp"

int main( int argc, char ** argv )
{
    QGuiApplication app(argc,argv);
    
    app.setWindowIcon(QIcon("/usr/share/quickfm/folder.svgz"));
    
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    
    QQmlContext* ctx = engine->rootContext();
    
    file_manager fm;
    
    ctx->setContextProperty("fm",&fm);
    
    engine->load("/usr/share/quickfm/main.qml"); 
    
    return app.exec();
}
