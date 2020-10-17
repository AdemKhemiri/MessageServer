#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QHostInfo>

#include <QtDebug>
#include <QVector>
#include <QFile>
class server : public QObject
{
	Q_OBJECT

private:
	int serverPort;
    QFile Logs;

	QTcpServer *chatServer;
	QVector<QTcpSocket*> *allClients;

    void recentChat(QTcpSocket*);
public:
	explicit server(QObject *parent = nullptr);

    server(int port)            //////////////// CONSTRUCTOR ////////////////////////
        : serverPort{port}{
        QString name ;
        name = "Logs for Port " + QString::number(serverPort);
        Logs.setFileName(name);
        if(!Logs.open(QIODevice::Append))
            {
                qDebug() << "File does not exists from server.h";
                //return;
            }
        Logs.close();
    };
    ~server() {Logs.close();} /////////////////// DESTRUCTOR ////////////////////////
	void startServer();
	void sendMessageToClients(QString msg);
public slots:
	void newClientConnection();
	void socketDisconnected();
	void socketReadyRead();
	void socketStateChanged(QAbstractSocket::SocketState state);
signals:

};

#endif // SERVER_H
