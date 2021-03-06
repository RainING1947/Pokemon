﻿#include "ui_signin.h"
#include <QMessageBox>
#include <iostream>
#include "signin.h"
#include <QEvent>
#include <QKeyEvent>

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    this->InitUI();
    this->SetEventFilter();
    this->InitConnect();
}

void SignIn::InitUI()
{
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/background.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    palette.setBrush(QPalette::Base, QBrush(QColor::QColor(236, 221, 190)));
    this->SetGosankeGif();
}

void SignIn::SetGosankeGif()
{
    QMovie* movie = new QMovie(":/blastoise.gif");
    this->ui->jenLabel->setMovie(movie);
    movie->start();
    movie = new QMovie(":/charizard.gif");
    this->ui->charLabel->setMovie(movie);
    movie->start();
    movie = new QMovie(":/bulbasaur.gif");
    this->ui->bulLabel->setMovie(movie);
    movie->start();
}

void SignIn::SetEventFilter()
{
    this->ui->userLineEdit->installEventFilter(this);
    this->ui->pwLineEdit->installEventFilter(this);
    this->ui->signInButton->installEventFilter(this);
    this->ui->topButtonSignOn->installEventFilter(this);
}

void SignIn::InitConnect()
{
    QObject::connect(this->ui->signInButton, SIGNAL(clicked(bool)), this, SLOT(onSignInClicked()));
    QObject::connect(this->ui->topButtonSignOn, SIGNAL(clicked(bool)), this, SLOT(onTopSignOnClicked()));
}

SignIn::SignIn(SocketClient* sc, QWidget *parent) :
    SignIn(parent)
{
    socketClient = sc;
}

SignIn::~SignIn()
{
    delete socketClient;
    delete ui;
}

void SignIn::receiveSwitch()
{
    this->ui->userLineEdit->clear();
    this->ui->pwLineEdit->clear();
    this->show();
}

void SignIn::onTopSignOnClicked()
{
    this->hide();
    emit switchToSignOn();
}

bool SignIn::eventFilter(QObject *watched, QEvent *event)
{
    QPalette focusinPalette = QPalette();
    QPalette focusoutPalette = QPalette();
    focusinPalette.setColor(QPalette::Base, QColor(221, 240, 237));
    focusoutPalette.setColor(QPalette::Base, Qt::white);

    //watch user name line edit
    if (watched == this->ui->userLineEdit)
    {
        //clear line eidt when it is focused
        if (event->type() == QEvent::FocusIn)
        {
            this->ui->userLineEdit->clear();
            this->ui->userLineEdit->setPalette(focusinPalette);
        }

        //add palette when it is not focused
        else if (event->type() == QEvent::FocusOut)
            this->ui->userLineEdit->setPalette(focusoutPalette);
    }

    //watch password line edit
    if (watched == this->ui->pwLineEdit)
    {
        //clear line eidt and set echo mode when it is focused
        if (event->type() == QEvent::FocusIn)
        {
            this->ui->pwLineEdit->clear();
            this->ui->pwLineEdit->setEchoMode(QLineEdit::Password);
            this->ui->pwLineEdit->setPalette(focusinPalette);
        }

        //add palette when it is not focused
        else if (event->type() == QEvent::FocusOut)
            this->ui->pwLineEdit->setPalette(focusoutPalette);
    }

    //watch sign in button
    if (watched == this->ui->signInButton)
    {
        //support keyboard operation
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Enter
                    || keyEvent->key() == Qt::Key_Return)
                emit this->ui->signInButton->clicked();
        }
    }

    //watch sign on button
    if (watched == this->ui->topButtonSignOn)
    {
        //support keyboard operation
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Enter
                    || keyEvent->key() == Qt::Key_Return)
                emit this->ui->topButtonSignOn->clicked();
        }
    }
    return QWidget::eventFilter(watched, event);
}

DWORD WINAPI SendThreadFuncSignIn(LPVOID lParam, LPVOID sParam);
DWORD WINAPI RecvThreadFuncSignIn(LPVOID lParam, SignIn* signin);

void SignIn::onSignInClicked()
{
    //user name can not be empty
    if (this->ui->userLineEdit->text().isEmpty())
        QMessageBox::information(this, "Error", QString::fromStdString("Username can't be empty"));
    else
    {
        //password can not be empty
        if (this->ui->pwLineEdit->text().isEmpty())
            QMessageBox::information(this, "Error", QString::fromStdString("Password can't be empty"));
        else
        {
            json j;
            j["symbol"] = "signin";
            j["username"] = this->ui->userLineEdit->text().toStdString();
            j["password"] = this->ui->pwLineEdit->text().toStdString();
            j["end"] = "end";
            std::string str = j.dump();

            //send username and password to server
            std::thread signinSendThread = std::thread(SendThreadFuncSignIn, socketClient, &str);
            signinSendThread.join();

            //recv feedback from server
            std::thread signinRecvThread = std::thread(RecvThreadFuncSignIn, socketClient, this);
            signinRecvThread.join();

            json recvJ = json::parse(recvString);
            recvString = "";
            bool userOnlined = recvJ["onlined"];
            bool useravailable = recvJ["useravailable"];
            bool passwordcorrect = recvJ["passwordcorrect"];

            //can not log in again if the account is already online
            if (userOnlined)
            {
                QMessageBox::information(this, "Info", "Your account is already online\nPlease log out first");
                this->ui->userLineEdit->clear();
                this->ui->pwLineEdit->clear();
            }
            else
            {
                //notice if user is not logged
                if (!useravailable)
                {
                    QMessageBox::information(this, "Info", "User not logged please sign on");
                }

                //turn to main page if user exist and password is right
                else if (passwordcorrect)
                {
                    this->ui->userLineEdit->clear();
                    this->ui->pwLineEdit->clear();
                    std::string tmp = j["username"];
                    socketClient->setPlayerName(tmp);
                    this->hide();
                    emit switchToMainPage();
                }

                //password is wrong
                else
                {
                    QMessageBox::information(this, "Info", "Wrong password");
                    this->ui->pwLineEdit->setFocus();
                }
            }
        }
    }
}

bool SignIn::setRecvStrSignIn(QString s)
{
    this->recvString = s.toStdString();
    return true;
}

DWORD WINAPI SendThreadFuncSignIn(LPVOID lParam, LPVOID sParam)
{
    std::string *sendStr = (std::string*)sParam;
    SocketClient *socketClient = (SocketClient*)lParam;
    int iResult;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    size_t len = (*sendStr).length();
    char *sendbuf = new char[len];
    strcpy(sendbuf, (*sendStr).data());
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }
    return 0;
}

DWORD WINAPI RecvThreadFuncSignIn(LPVOID lParam, SignIn* signin)
{
    SocketClient* socketClient = (SocketClient*)lParam;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(signin, "setRecvStrSignIn", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
    }
    else if (socketClient->iResult == 0)
    {
        std::cout << "Connection closed" << std::endl;
    }
    else
    {
        std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
    }
    return 0;
}
