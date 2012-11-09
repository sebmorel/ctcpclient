#include "ctcpclient.h"

#include <iostream>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QThread>



void _sqlExecCmd(CTcpClient *socket, QString balise, QString cmd);
void _sqlExecCmd(CTcpClient *socket, QString balise, QString cmd = ""){
    socket->sendMessage(cmd, balise);
//    if(socket->state()
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
}



CTcpClient::CTcpClient(QObject *parent) :
    QTcpSocket(parent)
{
}

CTcpClient::~CTcpClient()
{
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::sendMessage(QString message, QString balise=QString::null)
{
    QString strMessage;
    QByteArray paquet;
    QDataStream out (&paquet, QIODevice::WriteOnly);

    if ((!balise.isNull()) && (!balise.isEmpty())){
        strMessage = "<" + balise + ">" + message + "</" + balise + ">";
    }
    else strMessage = message;

    out << (quint32)0;
    out << strMessage;
    out.device()->seek(0);
    out << (quint32) (paquet.size() - sizeof(quint32));

    write(paquet);
}
/************************************************************************************************/
void CTcpClient::sendAction(QString action){
    sendMessage("", action);
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
bool CTcpClient::decoupMessageBalise(QString txtbrut, QString *data, QString *balise)
{
//    QString strBaliseDeb, strBaliseFin;

//    // TCP_RECEIVE_POSTES_CO : réception de la liste des postes connectés, doit être reçu à chaque fois que le serveur détecte une arrivée ou un départ.
//    toBalises(TCP_RECEIVE_POSTES_CO, &strBaliseDeb, &strBaliseFin);
//    int iDebBal(txtbrut.indexOf(strBaliseDeb)), iFinBal(txtbrut.indexOf(strBaliseFin));
//    if ((-1 != iDebBal) && (-1 != iFinBal)){
//        (*data) = txtbrut.mid(iDebBal+strBaliseDeb.length(), iFinBal-iDebBal-strBaliseDeb.length());
//        (*balise) = TCP_RECEIVE_POSTES_CO;
//    }
    return false;
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
bool CTcpClient::toBalises(QString msg, QString balise)
{
    int iDeb, iFin;
    QString strBalDeb, strBalFin;
    strBalDeb = "<" + balise + ">";
    strBalFin = "</" + balise + ">";

    iDeb = msg.indexOf(strBalDeb);
    iFin = msg.indexOf(strBalFin);

    return ((-1 != iDeb) && (-1 != iFin));
}
bool CTcpClient::toBalises(QString msg, QString balise, QString *data)
{
    bool b(false);
    int iDeb, iFin;
    QString strBalDeb, strBalFin;
    strBalDeb = "<" + balise + ">";
    strBalFin = "</" + balise + ">";

    iDeb = msg.indexOf(strBalDeb);
    iFin   = msg.indexOf(strBalFin);

    if ((-1 != iDeb) && (-1 != iFin)){
        b = true;
        (*data) = msg.mid(iDeb+strBalDeb.length(), iFin-iDeb-strBalFin.length()+1);
    }

    return b;
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
QStringList CTcpClient::connectedPostes()
{
    sendMessage("", TCP_GET_POSTES_CO);
    waitForBytesWritten();
    waitForReadyRead();
    return m_lstPostesCo;
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::setConnectedPostes(QStringList postes)
{
    m_lstPostesCo = postes;
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::sqlRequete(QString requete)
{
    sendMessage(requete, TCP_REQUETE);
    waitForBytesWritten();
//    waitForReadyRead();
}
/************************************************************************************************/
bool CTcpClient::sqlExec()
{
//    sendMessage(" ", TCP_SQL_EXEC);
    _sqlExecCmd(this, TCP_SQL_EXEC);
    return m_retourSql;
}
/************************************************************************************************/
bool CTcpClient::sqlNext()
{
    _sqlExecCmd(this, TCP_SQL_NEXT);
    return m_retourSql;
}
/************************************************************************************************/
bool CTcpClient::sqlIsValid(){
    _sqlExecCmd(this, TCP_SQL_ISVALID);
    return m_retourSql;
}
/************************************************************************************************/
bool CTcpClient::sqlSeek(int pos){
    std::cout << "seek " << pos << std::endl;
    _sqlExecCmd(this, TCP_SQL_SEEK, QString::number(pos));
    return m_retourSql;
}
/************************************************************************************************/
QVariant CTcpClient::sqlValue(int pos){
    _sqlExecCmd(this, TCP_SQL_VALUE, QString::number(pos));
    return m_retourSqlValue;
}
bool CTcpClient::sqlIsOpen(){
    _sqlExecCmd(this, TCP_SQL_OPEN);
    return m_retourSql;
}
bool CTcpClient::sqlOpen(){
    return sqlIsOpen();
}
/************************************************************************************************/
bool CTcpClient::sqlPrepare(QString requete){
    _sqlExecCmd(this, TCP_SQL_PREPARE, requete);
    return m_retourSql;
}
bool CTcpClient::sqlBindValue(const QString &placeholder, const QVariant &val){
    sendMessage("<"+TCP_SQL_BIND+">"+placeholder+"</"+TCP_SQL_BIND+"><"+TCP_SQL_BINDVALUE+">"+val.toString()+"</"+TCP_SQL_BINDVALUE+">");
    waitForBytesWritten();
    waitForReadyRead();

    return m_retourSql;
}
/************************************************************************************************/
QString CTcpClient::sqlError(){
    _sqlExecCmd(this, TCP_SQL_ERROR);
    return m_retourSqlError;
}
int CTcpClient::sqlTypeError(){
    _sqlExecCmd(this, TCP_SQL_TYPE_ERROR);
    return m_retourSqlTypeError;
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::setRetourSql(bool b){
    m_retourSql = b;
}
void CTcpClient::setRetourSqlValue(QVariant var){
    m_retourSqlValue = var;
}
void CTcpClient::setRetourSqlErreur(QString var){
    m_retourSqlError = var;
}
void CTcpClient::setRetourSqlTypeErreur(int val){
    m_retourSqlTypeError = val;
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::setSuperviseurMode(bool mode){
    sendMessage("", TCP_GODMODE);
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
void CTcpClient::setSvVersion(QString version){
    m_svVersion = version;
}
QString CTcpClient::svVersion(){
    return m_svVersion;
}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
//void CTcpClient::newThread(QObject *parent)
//{
//    _thread = new CThreadTcp(this);
//}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
//void CTcpClient::startThread()
//{
//    _thread->start();
//}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
//void CTcpClient::runThread()
//{
//    _thread->run();
//}
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
