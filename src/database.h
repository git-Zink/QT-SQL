#ifndef DATABASE_H
#define DATABASE_H

#include <QMainWindow>

namespace Ui {
    class Database;
}

class Database : public QMainWindow
{
    Q_OBJECT
    void MyOutputTable(QString);
public:
    explicit Database(QWidget *parent = 0);
    ~Database();
public slots:
    void myconnect1();
    void showMusician();
    void showMusInBand();
    void showAlbum();
    void showLabel();
    void showBand();
    void addRecord();
    void refDialog();
    void cancelDialog();
    void acceptDialog();
    void buttonDel();
    void search();
    void acceptSearch();
    void change();
    void changeButton();
    void CreateReport();
    void closeReport();
    void InfoMus();
    void InfoLabel();
    void InfoAccept();
    void InfoCancel();
private:
    Ui::Database *ui;
};

#endif // DATABASE_H
