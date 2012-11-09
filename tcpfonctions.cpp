#include "tcpfonctions.h"

#include <QHostAddress>
//#include <QAbstractButton>

#include "ctcpclient.h"
#include "cconfigparser.h"
#include "GlobalesFonctions.h"
#include "ctcpretour.h"

using namespace TypesRetour;

void _fctTcpConnexion(CTcpClient *socket)
{
    socket->connectToHost(cfgLecture("SERVEUR", "ip"), cfgLecture("SERVEUR", "port").toInt());
}

void _fctTcpConnectEvent(CTcpClient *socket)
{
}

CTcpRetour _evtTcpDonneesRecues(CTcpClient *socket, quint32 *tailleMsg)
{
//    CMessageBox *box;
    // Réception des données envoyées par le serveur. Elles sont "triées" selon la balise contenue puis distribuées où il se doit où elles seront traitées.

    // Réception des données.
    QDataStream in(socket);
    QString messageRecu, strRetour;
    CTcpRetour tcpretour;

    forever{
        if ((*tailleMsg)==0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint32)) return tcpretour;
            in >> (*tailleMsg);
        }
        if (socket->bytesAvailable() < (*tailleMsg)) return tcpretour;
        in >> messageRecu;

        // Recherche dess.
        if(socket->toBalises(messageRecu, TCP_RECEIVE_POSTES_CO, &strRetour)){
            QStringList lstPostesCo(strRetour.split("#"));

            socket->setConnectedPostes(lstPostesCo);
        }

        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_SEND_MESSAGE, &strRetour)){
            tcpretour = CTcpRetour("Un message est arrivé", strRetour, TypesRetour::Information, TypesRetour::Ok);

//            box = new CMessageBox(CMessageBox::Information, "Un message est arrivé",
//                                  strRetour,
//                                  CMessageBox::Ok, NULL, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
//            box->show();
        }

        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_SQL_EXEC, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_SEEK, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_ISVALID, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_NEXT, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_OPEN, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_PREPARE, &strRetour)
                || socket->toBalises(messageRecu, TCP_SQL_BINDVALUE, &strRetour))
        {
            socket->setRetourSql(strRetour == "1");
        }

        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_SQL_VALUE, &strRetour)){
            socket->setRetourSqlValue(QVariant(strRetour));
        }

        /************************************************************************************************/
        if (socket->toBalises(messageRecu, MAJ_SUPERVISION)){
//            ((CLanceur*)socket->parent())->timerMaj();
        }

        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_VERSION_CLIENTS)){
//            socket->sendMessage(((CLanceur*)socket->parent())->versionSV(), TCP_MA_VERSION);
        }
        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_SQL_ERROR, &strRetour)){
            socket->setRetourSqlErreur(strRetour);
        }
        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_SQL_TYPE_ERROR, &strRetour)){
            socket->setRetourSqlTypeErreur(strRetour.toInt());
        }
        /************************************************************************************************/
        if (socket->toBalises(messageRecu, TCP_RECUP_TRACEUR)){     // Renvoie du fichier log
            QFile fLog("SuperVision.log");
            fLog.open(QFile::ReadOnly);
            QDataStream data(&fLog);
            char *flux = new char[fLog.size()];
            data.readRawData(flux, fLog.size());

            QString balise0 = "<"+TCP_ENVOI_TRACEUR+">";
            QString balise1 = "</"+TCP_ENVOI_TRACEUR+">";
            QByteArray paquet;
            QDataStream out(&paquet, QIODevice::WriteOnly);
            out << (quint32)0;
            out << balise0;
            out << data;
            out << balise1;
            out.device()->seek(0);
            out << (quint32)(paquet.size()-sizeof(quint32));

            socket->write(paquet);
//            socket->sendMessage("", TCP_ENVOI_TRACEUR);
        }


        (*tailleMsg) = 0;
    }
    return tcpretour;
}

void _evtTcpConnecte(CTcpClient *socket){
}

void _evtTcpDeco(CTcpClient *socket){
    socket->abort();
    socket->connectToHost(cfgLecture("SERVEUR", "ip"), cfgLecture("SERVEUR", "port").toInt());
}

CTcpRetour _evtTcpErreur(CTcpClient *socket, CTcpClient::SocketError socketError/*, QWidget *parent*/){
//    CMessageBox *box;
    CTcpRetour tcpretour;
//    int iChoix;

    switch (socketError){
    // Erreur inconnue !!!
    case CTcpClient::UnknownSocketError:
        break;

    // Connexion refusée.
    case CTcpClient::ConnectionRefusedError:
        tcpretour = CTcpRetour("ConnectionRefusedError",
                               "Le serveur refuse la connexion.\nMerci de contacter Sébastien ou Vincent.",
                               TypesRetour::Critique, TypesRetour::Ok);
//        box = new CMessageBox(CMessageBox::Critical, "ConnectionRefusedError",
//                              "Le serveur refuse la connexion.\nMerci de contacter Sébastien ou Vincent.",
//                              CMessageBox::Ok, parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
//        box->exec();
//        delete box;
        break;

    // Connexion avec le serveur perdue.
    case CTcpClient::RemoteHostClosedError:
        break;

    // Adresse du serveur inconnue.
    case CTcpClient::HostNotFoundError:
        tcpretour = CTcpRetour("HostNotFoundError", "Personne à l'adresse <"+socket->peerAddress().toString()+">",
                               TypesRetour::Critique, TypesRetour::Ok);
//        box = new CMessageBox(CMessageBox::Critical, "HostNotFoundError", "Personne à l'adresse <"+socket->peerAddress().toString()+">",
//                              CMessageBox::Ok, parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
//        box->exec();
//        delete box;
        break;

    // Privilèges insuffisants pour se connecter.
    case CTcpClient::SocketAccessError:
        tcpretour = CTcpRetour("SocketAccessError",
                               "Privilège du poste insuffisant.\nContactez Sébastien ou Vincent.",
                               TypesRetour::Critique, TypesRetour::Ok);
//        box = new CMessageBox(CMessageBox::Critical, "SocketAccessError",
//                              "Privilège du poste insuffisant.\nContactez Sébastien ou Vincent.",
//                              CMessageBox::Ok, parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
//        iChoix = box->exec();
//        if (CMessageBox::Retry == iChoix){
//            socket->reset();
//            socket->connectToHost(cfgLecture("SERVEUR", "ip"), cfgLecture("SERVEUR", "port").toInt());
//        }
//        else parent->close();

//        delete box;
        break;

    // Ressources insuffisantes pour se connecter.
    case CTcpClient::SocketResourceError:
        break;

    // Time out.
    case CTcpClient::SocketTimeoutError:
        break;

    // Données envoyées trop grandes.
    case CTcpClient::DatagramTooLargeError:
        break;

    // Erreur sur le réseau.
    case CTcpClient::NetworkError:
        tcpretour = CTcpRetour("NetworkError", "Impossible de se connecter au réseau. "
                               "Veuillez vérifier que TOUT les cables soient correctement branchés, puis cliquez sur <Retry>.",
                               TypesRetour::Critique, TypesRetour::Retry | TypesRetour::Close);
//        box = new CMessageBox(CMessageBox::Critical, "NetworkError",
//                              "Impossible de se connecter au réseau. "
//                              "Veuillez vérifier que TOUT les cables soient correctement branchés, puis cliquez sur <Retry>.",
//                              CMessageBox::Retry | CMessageBox::Close, parent,
//                              Qt::Popup | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
//        iChoix = box->exec();
//        if (CMessageBox::Retry == iChoix){
//            socket->reset();
//            socket->connectToHost(cfgLecture("SERVEUR", "ip"), cfgLecture("SERVEUR", "port").toInt());
//        }
//        else parent->close();

//        delete box;
        break;

    // Adresse de connexion erronée.
    case CTcpClient::SocketAddressNotAvailableError:
        break;

    // Les erreurs suivantes ne devraient pas être utiles.
    case CTcpClient::AddressInUseError:
    case CTcpClient::UnsupportedSocketOperationError:
    case CTcpClient::ProxyAuthenticationRequiredError:
    case CTcpClient::SslHandshakeFailedError:
    case CTcpClient::UnfinishedSocketOperationError:
    case CTcpClient::ProxyConnectionRefusedError:
    case CTcpClient::ProxyConnectionClosedError:
    case CTcpClient::ProxyConnectionTimeoutError:
    case CTcpClient::ProxyNotFoundError:
    case CTcpClient::ProxyProtocolError:
        break;
    }
    return tcpretour;
}

CTcpRetour _evtTcpStateChange(CTcpClient *socket, CTcpClient::SocketState etat/*, CMessageBox **box, QWidget *parent*/){
    CTcpRetour retour;
//    if (NULL != (*box)){
//        (*box)->close();
//        delete (*box);
//        (*box) = NULL;
//    }
    switch(etat){
    case CTcpClient::ConnectingState:
        retour = CTcpRetour("Serveur news", "Connexion en cours... Merci de patienter", TypesRetour::Information);
//        (*box) = new CMessageBox(CMessageBox::Information, "Serveur news",
//                                 "Connexion cours... Merci de patienter", CMessageBox::NoButton, parent);//Qt::WindowStaysOnTopHint);
//        (*box)->show();
//        foreach(QAbstractButton *but, (*box)->buttons()) but->hide();
        break;

    case CTcpClient::UnconnectedState:
        retour = CTcpRetour("Serveur news", "Aucune connexion", TypesRetour::Critique);
//        (*box) = new CMessageBox(QMessageBox::Critical, "Serveur news", "Aucune connexion", CMessageBox::NoButton, parent, 0);
        break;

    case CTcpClient::ConnectedState:
    case CTcpClient::HostLookupState:
    case CTcpClient::BoundState:
    case CTcpClient::ListeningState:
    case CTcpClient::ClosingState:
        break;
    }
    return retour;
}


