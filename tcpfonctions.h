#ifndef TCPFONCTIONS_H
#define TCPFONCTIONS_H

#include "ctcpclient.h"
//#include <QtGlobal>

//class QWidget;
class CTcpClient;
//class CMessageBox;
class CTcpRetour;

void CTCPCLIENTSHARED_EXPORT _fctTcpConnexion(CTcpClient *socket);
void CTCPCLIENTSHARED_EXPORT _fctTcpConnectEvent(CTcpClient *socket);
CTcpRetour CTCPCLIENTSHARED_EXPORT _evtTcpDonneesRecues(CTcpClient *socket, quint32 *tailleMsg);
void CTCPCLIENTSHARED_EXPORT _evtTcpConnecte(CTcpClient *socket);
void CTCPCLIENTSHARED_EXPORT _evtTcpDeco(CTcpClient *socket);
CTcpRetour CTCPCLIENTSHARED_EXPORT _evtTcpErreur(CTcpClient *socket, CTcpClient::SocketError socketError/*, QWidget *parent=0*/);
CTcpRetour CTCPCLIENTSHARED_EXPORT _evtTcpStateChange(CTcpClient *socket, CTcpClient::SocketState etat/*, CMessageBox **box, QWidget *parent*/);

#endif // TCPFONCTIONS_H
