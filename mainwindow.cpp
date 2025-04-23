#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          networkManager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);

    connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::onConvertClicked);

    QStringList currencies = {"USD", "EUR", "PLN", "GBP", "JPY"};
    ui->fromCurrencyComboBox->addItems(currencies);
    ui->toCurrencyComboBox->addItems(currencies);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConvertClicked() {
    QString amountStr = ui->amountLineEdit->text();
    QString fromCurrency = ui->fromCurrencyComboBox->currentText();
    QString toCurrency = ui->toCurrencyComboBox->currentText();

    bool ok;
    double amount = amountStr.toDouble(&ok);
    if (!ok || amount <= 0) {
        ui->resultLabel->setText("Nieprawidłowa kwota");
        return;
    }

    QString apiKey = "TU_WKLEJ_SWOJ_API_KEY";       //   https://exchangerate.host/dashboard
    QString url = QString("https://api.exchangerate.host/convert?access_key=%1&from=%2&to=%3&amount=%4")
            .arg(apiKey)
            .arg(fromCurrency)
            .arg(toCurrency)
            .arg(amount);

    qDebug() << "URL:" << url;

    QUrl qurl(url);
    QNetworkRequest request(qurl);

    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onNetworkReply(reply);
    });
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        ui->resultLabel->setText("Błąd sieci: " + reply->errorString());
        qDebug() << "Network error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    qDebug() << "API RESPONSE:" << responseData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj.contains("success") && jsonObj["success"].toBool()) {
        if (jsonObj.contains("result")) {
            double result = jsonObj["result"].toDouble();
            ui->resultLabel->setText(QString::number(result, 'f', 2));
        } else {
            ui->resultLabel->setText("Brak wyniku w odpowiedzi");
        }
    } else if (jsonObj.contains("error")) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        QString errorMsg = errorObj["info"].toString();
        ui->resultLabel->setText("Błąd API: " + errorMsg);
    } else {
        ui->resultLabel->setText("Nieoczekiwana odpowiedź");
    }

    reply->deleteLater();
}
