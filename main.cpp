#include <QCoreApplication>
#include <server.h>
#include <iostream>
#include <QVector>

void showServerMenu();
void addServer();


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
    QCoreApplication b(argc, argv);



    server *Server0 = new server(9000);
    server *Server1 = new server(9111);
    server *Server2 = new server(9222);
    server *Server3 = new server(9333);

    Server0->startServer();
	Server1->startServer();
    Server2->startServer();
    Server3->startServer();

    // TODO: add a vector to store ports and send them to client maybe


	return a.exec();
}









void showServerMenu()
{
    bool serverRunning = true;
    while(serverRunning)
    {
        qDebug() << "SERVER MENU \n";
        qDebug() << "1. Add server";
        qDebug() << "2. ShutDown servers";
        int rep(0);
        qDebug() << "Chose an number: ";
        std::cin >> rep;
        switch (rep)
        {
            case 1:addServer();
                break;
        }
    }


}
void addServer()
{
    int port;
    std::cout << "Enter a new port: ";
    std::cin >> port;

    server *Server = new server(port);
    Server->startServer();
    return;
}
