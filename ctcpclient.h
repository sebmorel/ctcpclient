#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include "ctcpclient_global.h"
#include <QStringList>
#include <QTcpSocket>


class QString;
class QStringList;
class QVariant;

const QString       MAJ_SUPERVISION         ("MAJsupervision");     // Superviseur->clients
const QString       MAJ_SERVEUR             ("MAJserveur");
const QString       TCP_MA_VERSION          ("VersionSV");
const QString       TCP_VERSION_CLIENTS     ("VersionSVclients");
const QString       TCP_WIN_VERSION         ("VersionWindows");
const QString       TCP_SHOW_LOG            ("LogServeur");

const QString       TCP_RECUP_TRACEUR       ("RecupTraceur");
const QString       TCP_ENVOI_TRACEUR       ("EnvoiTraceur");
const QString       TCP_RECUP_CONFIG        ("RecupCfg");
const QString       TCP_ENVOI_CONFIG        ("EnvoiCfg");
const QString       TCP_MODIF_CFG           ("ModifCfg");

const QString       TCP_GET_POSTES_CO       ("GetPosteConnected");
const QString       TCP_RECEIVE_POSTES_CO   ("ReceivePosteConnected");
const QString       TCP_MON_NOM             ("NomPosteConnect");
const QString       TCP_NOM_POSTE           ("Poste");
const QString       TCP_SEND_MESSAGE        ("SendMessage");
const QString       TCP_GODMODE             ("SuperViseurMode");

const QString       TCP_REQUETE             ("SendRequete");
const QString       TCP_SQL_OPEN            ("SqlIsOpen");
const QString       TCP_SQL_EXEC            ("SqlExec");
const QString       TCP_SQL_NEXT            ("SqlNext");
const QString       TCP_SQL_ISVALID         ("SqlIsValid");
const QString       TCP_SQL_VALUE           ("SqlValue");
const QString       TCP_SQL_PREPARE         ("SqlPrepare");
const QString       TCP_SQL_BINDVALUE       ("SqlBindValue");
const QString       TCP_SQL_BIND            ("SqlBind");
const QString       TCP_SQL_SEEK            ("SqlSeek");
const QString       TCP_SQL_ERROR           ("SqlError");
const QString       TCP_SQL_TYPE_ERROR      ("SqlTypeError");


class CTCPCLIENTSHARED_EXPORT CTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpClient(QObject *parent);
    ~CTcpClient();

//    void newThread(QObject *parent);
//    void startThread();
//    void runThread();

    void sendMessage(QString message, QString balise);
    void sendAction(QString action);
    QStringList connectedPostes();
    bool decoupMessageBalise(QString txtbrut, QString *data, QString *balise);
    void setConnectedPostes(QStringList postes);
    bool toBalises(QString msg, QString balise, QString *data);
    bool toBalises(QString msg, QString balise);
    void setRetourSql(bool b);
    void setRetourSqlValue(QVariant var);
    void setRetourSqlErreur(QString var);
    void setRetourSqlTypeErreur(int val);
    void setSuperviseurMode(bool mode);
    void setSvVersion(QString version);
    QString svVersion();

    // Fonctions pour le requètage oracle.
    void sqlRequete(QString requete);
    bool sqlExec();
    bool sqlNext();
    bool sqlIsValid();
    bool sqlSeek(int pos);
    QVariant sqlValue(int pos);
    bool sqlIsOpen();
    bool sqlOpen();
    QString sqlError();
    int sqlTypeError();
    bool sqlPrepare(QString requete);
    bool sqlBindValue(const QString &placeholder, const QVariant &val);

private:
//    bool        (*m_fctMaj)(void);
    QStringList m_lstPostesCo;
    bool        m_retourSql;
    QVariant    m_retourSqlValue;
    QString     m_retourSqlError;
    int         m_retourSqlTypeError;
    QString     m_svVersion;
//    CThreadTcp  *_thread;

signals:

public slots:

};

#endif // CTCPCLIENT_H
