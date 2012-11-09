#include "ctcpretour.h"

#include <QString>

using namespace TypesRetour;

CTcpRetour::CTcpRetour()
{
    _titre = QString::null;
    _msg = QString::null;
    _icon = TypesRetour::None;
    _bouton = TypesRetour::NoButton;
    _actif = false;
}
CTcpRetour::CTcpRetour(QString titre,
                       QString message,
                       TypeMessage type,
                       int bouton)
{
    setTitre(titre);
    setMessage(message);
    setTypeMessage(type);
    setBoutons(bouton);
    _actif = true;
}
/************************************************************************************************/
QString CTcpRetour::message() const{
    return _msg;
}
void CTcpRetour::setMessage(QString msg){
    _msg = msg;
}
/************************************************************************************************/
QString CTcpRetour::titre() const{
    return _titre;
}
void CTcpRetour::setTitre(QString titre){
    _titre = titre;
}
/************************************************************************************************/
CTcpClient::SocketError CTcpRetour::tcpError(){
    return _tcperror;
}
void CTcpRetour::setTcpError(CTcpClient::SocketError error){
    _tcperror = error;
}
/************************************************************************************************/
CTcpClient::SocketState CTcpRetour::tcpState(){
    return _tcpstate;
}
void CTcpRetour::setTcpState(CTcpClient::SocketState state){
    _tcpstate = state;
}
/************************************************************************************************/
TypesRetour::TypeMessage CTcpRetour::typeMessage(){
    return _icon;
}
void CTcpRetour::setTypeMessage(TypesRetour::TypeMessage type){
    _icon = type;
}
/************************************************************************************************/
int CTcpRetour::boutons(){
    return _bouton;
}
void CTcpRetour::setBoutons(int type){
    _bouton = (TypesRetour::TypeBouton)type;
}
/************************************************************************************************/
bool CTcpRetour::isNull(){
    return !_actif;
}
/************************************************************************************************/
