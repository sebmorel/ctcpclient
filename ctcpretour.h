#ifndef CTCPRETOUR_H
#define CTCPRETOUR_H

#include "ctcpclient.h"

namespace TypesRetour{
typedef enum TypeMessage{
    None = 0,
    Information,
    Question,
    Critique,
    Warning
}TypeMessage;

typedef enum TypeBouton{
    Ok      = 0x00000400,
    Cancel  = 0x00400000,
    Close   = 0x00200000,
    Apply   = 0x02000000,
    Yes     = 0x00004000,
    No      = 0x00010000,
    Abort   = 0x00040000,
    Retry   = 0x00080000,
    Ignore  = 0x00100000,
    NoButton= 0x00000000
} TypeBouton;
}

class CTCPCLIENTSHARED_EXPORT CTcpRetour
{
public:
    CTcpRetour();
    CTcpRetour(QString titre, QString message, TypesRetour::TypeMessage type, int bouton=TypesRetour::NoButton);

    bool                        isNull();
    QString                     message() const;
    void                        setMessage(QString msg);
    QString                     titre() const;
    void                        setTitre(QString titre);
    CTcpClient::SocketError     tcpError();
    void                        setTcpError(CTcpClient::SocketError);
    CTcpClient::SocketState     tcpState();
    void                        setTcpState(CTcpClient::SocketState);
    TypesRetour::TypeMessage    typeMessage();
    void                        setTypeMessage(TypesRetour::TypeMessage type);
    int boutons();
    void                        setBoutons(int type);

private:
    bool                        _actif;
    QString                     _titre;
    QString                     _msg;
    CTcpClient::SocketError     _tcperror;
    CTcpClient::SocketState     _tcpstate;
    TypesRetour::TypeMessage    _icon;
    TypesRetour::TypeBouton     _bouton;
};

#endif // CTCPRETOUR_H
