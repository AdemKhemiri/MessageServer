#include "server.h"

server::server(QObject *parent) : QObject(parent)
{

}
void server::startServer()
{
	allClients = new QVector<QTcpSocket*>;

	chatServer = new QTcpServer();

	chatServer->setMaxPendingConnections(10);
    connect(chatServer,SIGNAL(newConnection()), this, SLOT(newClientConnection()));

    if(chatServer->listen(QHostAddress::Any, serverPort))
    {
		qDebug() << "Server has started. Listen to port "+ QString::number(serverPort);
        //qDebug() << info.hostName();
	}
	else
	{
		qDebug() << "Server failed to start. ERROR " + chatServer->errorString();
	}
}
void server::newClientConnection()
{
	QTcpSocket *client = chatServer->nextPendingConnection();
	QString ipAdress = client->peerAddress().toString();
	int port = client->peerPort();

	connect(client, &QTcpSocket::disconnected, this, &server::socketDisconnected);
	connect(client, &QTcpSocket::readyRead, this, &server::socketReadyRead);
	connect(client, &QTcpSocket::stateChanged, this, &server::socketStateChanged);

    recentChat(client);         // PREVIOUS CHAT AFTER DISCONNECTED

	allClients->push_back(client);

    QByteArray dat;
    QTextStream out(&dat);
    out << allClients;
    client->write(dat);

    qDebug() << "Socket connected from: " + ipAdress + ":" + QString::number(port) + "Server Port: " + serverPort;



}


void server::socketDisconnected()
{
	QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
	QString socketIpAdress = client->peerAddress().toString();
	int port = client->peerPort();

	qDebug() << "Socket disconneted from " + socketIpAdress + ":" + QString::number(port);
}

void server::socketReadyRead()
{
	QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());

	QString socketIpAddress = client->peerAddress().toString();
	int port = client->peerPort();

	QString data = QString(client->readAll());

    qDebug() << "Message :" + data + " ("+socketIpAddress +":"+ QString::number(port) +")"; // Try implementing file logs here
    if(!Logs.open(QIODevice::Append))
    {
        qDebug() << "File doesn't exist in socketReadyRead";
        //return;
    }
    QTextStream out(&Logs);
    out << "Message :" + data + " ("+socketIpAddress +":"+ QString::number(port) +")\n";
	sendMessageToClients(data);
    Logs.close();
}

void server::socketStateChanged(QAbstractSocket::SocketState state)
{
	QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
	QString socketIpAddress = client->peerAddress().toString();
	int port = client->peerPort();

	QString desc;

	if(state == QAbstractSocket::UnconnectedState)
		desc = "The socket is not connected.";
	else if (state == QAbstractSocket::HostLookupState)
		desc = "The host is performing a host name lookup.";
	else if (state == QAbstractSocket::ConnectingState)
		desc = "The socket has started establishing a connection.";
	else if (state == QAbstractSocket::ConnectedState)
		desc = "A connection is established.";
	else if(state == QAbstractSocket::BoundState)
		desc = "The socket is bound to an address and port.";
	else if (state == QAbstractSocket::ListeningState)
		desc = "For internal use only.";
	else if (state == QAbstractSocket::ClosingState)
		desc = "The socket is about to close (data may be still waiting to be written).";

	qDebug() << "	Socket state changed ("+socketIpAddress +":"+ QString::number(port) +")" + desc;
}


void server::sendMessageToClients(QString msg)
{
	if(allClients->size() > 0)
	{
		for(int i{0}; i < allClients->size(); ++i)
		{
			if(allClients->at(i)->isOpen() && allClients->at(i)->isWritable())
			{
				allClients->at(i)->write(msg.toUtf8());

			}
		}
	}
}
void server::recentChat(QTcpSocket *client)
{
    //qDebug() << "Recent Chat is Called";;
    if(!Logs.open(QIODevice::ReadOnly))
	{
        qDebug() << "File not found from recentChat.";
        //return;
	}
	QTextStream in(&Logs);
	while(!in.atEnd())
	{
        QString line = in.readLine();

        if(line.contains("color='Blue'"))
        {

            line.remove("Message :");
            line.remove(line.indexOf(" (::"),line.length());
            line.append("<br/>");

            //line.insert(line.length(),"<br/>");
            //qDebug() << line.toUtf8();
            client->write(line.toUtf8());
            //client->write(line.append("\n").toUtf8());
        }
	}
    Logs.close();
}
