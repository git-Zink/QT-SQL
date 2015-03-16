#include "database.h"
#include "ui_database.h"
#include<QSqlDatabase>
#include<QMessageBox>
#include<QTableWidget>
#include<QSqlQuery>
#include<QSqlError>
QSqlDatabase db;
int CurTable=-1;

Database::Database(QWidget *parent) :
    QMainWindow(parent),
ui(new Ui::Database)
{
    ui->setupUi(this);

    ui->add->setEnabled(false);
    ui->search->setEnabled(false);
    ui->del->setEnabled(false);
    ui->change->setEnabled(false);
    ui->createReport->setEnabled(false);
    ui->Report->setVisible(false);
    ui->menu_3->setEnabled(false);
    ui->Sort1->setVisible(false);
    ui->Sort2->setVisible(false);
    ui->Sort3->setVisible(false);
    ui->Sort4->setVisible(false);


    ui->menu_2->setEnabled(false);

    ui->Dialog->setVisible(false);
    ui->changeButton->setVisible(false);
    ui->searchButton->setVisible(false);
    ui->addButton->setVisible(false);
    ui->frame->setVisible(false);

    connect(ui->connection,SIGNAL(triggered()),this,SLOT(myconnect1()));
    connect(ui->actionMusician,SIGNAL(triggered()),this,SLOT(showMusician()));
    connect(ui->actionMusInBand,SIGNAL(triggered()),this,SLOT(showMusInBand()));
    connect(ui->actionBand,SIGNAL(triggered()),this,SLOT(showBand()));
    connect(ui->actionLabel,SIGNAL(triggered()),this,SLOT(showLabel()));
    connect(ui->actionAlbum,SIGNAL(triggered()),this,SLOT(showAlbum()));
    connect(ui->add,SIGNAL(clicked()),this,SLOT(addRecord()));
    connect(ui->combo,SIGNAL(currentIndexChanged(int)),SLOT(refDialog()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelDialog()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(acceptDialog()));
    connect(ui->del,SIGNAL(clicked()),this,SLOT(buttonDel()));
    connect(ui->search,SIGNAL(clicked()),this,SLOT(search()));
    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(acceptSearch()));
    connect(ui->change,SIGNAL(clicked()),this,SLOT(change()));
    connect(ui->changeButton,SIGNAL(clicked()),this,SLOT(changeButton()));
    connect(ui->createReport,SIGNAL(triggered()),this,SLOT(CreateReport()));
    connect(ui->closeReport,SIGNAL(clicked()),this,SLOT(closeReport()));
    connect(ui->info_label,SIGNAL(triggered()),this,SLOT(InfoLabel()));
    connect(ui->info_mus,SIGNAL(triggered()),this,SLOT(InfoMus()));
    connect(ui->infoAccept,SIGNAL(clicked()),this,SLOT(InfoAccept()));
    connect(ui->infoCancel,SIGNAL(clicked()),this,SLOT(InfoCancel()));
}
void Database::InfoAccept()
{
    QSqlQuery q;
    QStringList st;
    if(ui->info->text()=="Insert label's name:")
    {
        q.prepare("exec label_ '"+ui->whatinfo->text()+"'");
        st.push_back("BandName");
        st.push_back("Member");
        ui->table->setRowCount(0);
        ui->table->setColumnCount(2);
    }
    else
        if(ui->info->text()=="Insert musician's name:")
        {
            q.prepare("exec musician_info '"+ui->whatinfo->text()+"'");
            st.push_back("Name");
            st.push_back("BandName");
            st.push_back("Style");
            st.push_back("LabelName");
            ui->table->setRowCount(0);
            ui->table->setColumnCount(4);
        }
        else
        {
            QMessageBox::information(this,"Error","Something wrong, sry");
            return;
        }
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        return;
    }
    int curRow=0;
    while(q.next())
    {
        ui->table->setRowCount(ui->table->rowCount()+1);
        ui->table->setHorizontalHeaderLabels(st);
        for(int i=0;i<ui->table->columnCount();++i)
        {
            QTableWidgetItem *item=new QTableWidgetItem(q.value(i).toString());
            ui->table->setItem(curRow,i,item);
        }
        ++curRow;
    }
    ui->frame->setVisible(0);
    ui->table->setEnabled(1);
    ui->menu_3->setEnabled(1);
    ui->add->setEnabled(1);
    ui->change->setEnabled(1);
    ui->del->setEnabled(1);
    ui->search->setEnabled(1);
    ui->menu->setEnabled(1);
    CurTable=-1;
}
void Database::InfoCancel()
{
    ui->frame->setVisible(0);
    ui->table->setEnabled(1);
    ui->menu_3->setEnabled(1);
    ui->add->setEnabled(1);
    ui->change->setEnabled(1);
    ui->del->setEnabled(1);
    ui->search->setEnabled(1);
    ui->menu->setEnabled(1);
}
void Database::InfoLabel()
{
    ui->frame->setVisible(true);
    ui->table->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);
    ui->info->setText("Insert label's name:");
}
void Database::InfoMus()
{
    ui->frame->setVisible(true);
    ui->table->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);
    ui->info->setText("Insert musician's name:");
}
void Database::closeReport()
{
    ui->Report->setVisible(false);
    ui->table->setEnabled(1);
    ui->add->setEnabled(1);
    ui->change->setEnabled(1);
    ui->menu_3->setEnabled(1);
    ui->del->setEnabled(1);
    ui->search->setEnabled(1);
    ui->menu->setEnabled(1);
}
void Database::CreateReport()
{
    ui->Report->setVisible(true);
    ui->table->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);

    QSqlQuery q;
    q.prepare("select * from Label");
    int num=0;
    QString label;
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        ui->Report->setVisible(false);
        ui->table->setEnabled(1);
        ui->add->setEnabled(1);
        ui->menu_3->setEnabled(1);
        ui->change->setEnabled(1);
        ui->del->setEnabled(1);
        ui->search->setEnabled(1);
        ui->menu->setEnabled(1);
        return;
    }
    while(q.next())
    {
        QSqlQuery query;
        query.prepare("select count('Band.Label_ID') from Band where Band.Label_ID= "+q.value(0).toString());
        if(!query.exec())
        {
            QMessageBox::information(this,"Error",query.lastError().text());
            ui->Report->setVisible(false);
            ui->table->setEnabled(1);
            ui->add->setEnabled(1);
            ui->change->setEnabled(1);
            ui->del->setEnabled(1);
            ui->menu_3->setEnabled(1);
            ui->search->setEnabled(1);
            ui->menu->setEnabled(1);
            return;
        }
        query.next();
        if(num<query.value(0).toInt())
        {
            num=query.value(0).toInt();
            label=q.value(0).toString();
        }
    }
    ui->PopLab_num->setText(label);

    q.prepare("select b.Name,COUNT(a.band_id) as NumAlbom from band as b,album as a where a.band_id=b.band_id group by b.name order by NumAlbom desc");
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        ui->Report->setVisible(false);
        ui->table->setEnabled(1);
        ui->add->setEnabled(1);
        ui->menu_3->setEnabled(1);
        ui->change->setEnabled(1);
        ui->del->setEnabled(1);
        ui->search->setEnabled(1);
        ui->menu->setEnabled(1);
    }
    q.next();
    ui->mostAlbom->setText(q.value(0).toString());

    q.prepare("select Band_ID,Name from Band");
    num=0;
    QString name;
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        ui->Report->setVisible(false);
        ui->table->setEnabled(1);
        ui->add->setEnabled(1);
        ui->change->setEnabled(1);
        ui->menu_3->setEnabled(1);
        ui->del->setEnabled(1);
        ui->search->setEnabled(1);
        ui->menu->setEnabled(1);
        return;
    }
    while(q.next())
    {
        QSqlQuery query;
        query.prepare("select count('Band_ID') from MusInBand where Band_ID = "+q.value(0).toString());
        if(!query.exec())
        {
            QMessageBox::information(this,"Error",query.lastError().text());
            ui->Report->setVisible(false);
            ui->table->setEnabled(1);
            ui->add->setEnabled(1);
            ui->change->setEnabled(1);
            ui->del->setEnabled(1);
            ui->menu_3->setEnabled(1);
            ui->search->setEnabled(1);
            ui->menu->setEnabled(1);
            return;
        }
        query.next();
        if(num<query.value(0).toInt())
        {
            num=query.value(0).toInt();
            name=q.value(1).toString();
        }
    }
    ui->LargeGroup->setText(name);

}
void Database::changeButton()
{
    if(ui->lineEdit_1->text()=="" || ui->lineEdit_2->text()=="" || ui->lineEdit_3->text()=="" ||
       ui->lineEdit_4->text()=="")
    {
        QMessageBox::information(this,"Error","Some fields are empty");
        return;
    }
    QString tab,col;
    switch(CurTable)
    {
    case 0:
        tab="Label";
        col="Label_ID";
        break;
    case 1:
        tab="Band";
        col="Band_ID";
        break;
    case 2:
        tab="Album";
        col="Album_ID";
        break;
    case 3:
        tab="MusInBand";
        break;
    case 4:
        tab="Musician";
        col="Musician_ID";
        break;
    default:
       QMessageBox::information(this,"Error","Something wrong, sry");
       return;
    }
    QString st="select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME = '"+ui->combo->currentText()+"'";
    QSqlQuery q(st);
    QString str="update "+tab+" set ";
    if(CurTable!=3)
        q.next();
    q.next();
    if(CurTable==3)
        str+=q.value(0).toString()+"="+ui->lineEdit_1->text()+", ";
    else
        str+=q.value(0).toString()+"='"+ui->lineEdit_1->text()+"', ";
    q.next();
    if(CurTable==0 || CurTable==4)
        str+=q.value(0).toString()+"='"+ui->lineEdit_2->text()+"'";
    else
        str+=q.value(0).toString()+"="+ui->lineEdit_2->text();
    q.next();
    if(CurTable==1 || CurTable==2)
        str+=", "+q.value(0).toString()+"="+ui->lineEdit_3->text()+",";
    else
        str+=", "+q.value(0).toString()+"='"+ui->lineEdit_3->text()+"',";
    q.next();
    if(CurTable==2)
        str+=q.value(0).toString()+"="+ui->lineEdit_4->text();
    else
        str+=q.value(0).toString()+"='"+ui->lineEdit_4->text()+"'";
    q.next();
    if(CurTable!=3)
        str+=" where "+col+"="+ui->table->item(ui->table->currentRow(),0)->text();
    else
        str+=" where Musician_ID="+ui->table->item(ui->table->currentRow(),0)->text()+
             "and Band_ID="+ui->table->item(ui->table->currentRow(),1)->text();
    q.prepare(str);
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        return;
    }
    else
    {
       QMessageBox::information(this,"Success","Update done");
       ui->combo->setVisible(1);
       ui->label->setVisible(1);
       ui->changeButton->setVisible(0);
       ui->Dialog->setVisible(false);
       ui->table->setEnabled(1);
       ui->add->setEnabled(1);
       ui->change->setEnabled(1);
       ui->menu_3->setEnabled(1);
       ui->del->setEnabled(1);
       ui->search->setEnabled(1);
       ui->combo->setVisible(1);
       ui->label->setVisible(1);
       ui->menu->setEnabled(1);
       switch(CurTable)
       {
       case 0:
           showLabel();
           break;
       case 1:
           showBand();
           break;
       case 2:
           showAlbum();
           break;
       case 3:
           showMusInBand();
           break;
       case 4:
           showMusician();
           break;
       }

    }
}
void Database::change()
{
    if(ui->table->currentRow()==-1)
    {
        QMessageBox::information(this,"Error","No one select");
        return;
    }
    if(CurTable==-1)
    {
        QMessageBox::information(this,"Error","Something wrong, sry");
        return;
    }
    ui->Dialog->setVisible(true);
    ui->combo->setVisible(0);
    ui->label->setVisible(0);
    ui->table->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);
    ui->changeButton->setVisible(1);
    if(CurTable==3)
    {
        ui->lineEdit_1->setText(ui->table->item(ui->table->currentRow(),0)->text());
        ui->lineEdit_2->setText(ui->table->item(ui->table->currentRow(),1)->text());
        ui->lineEdit_3->setText(ui->table->item(ui->table->currentRow(),2)->text());
        ui->lineEdit_4->setText(ui->table->item(ui->table->currentRow(),3)->text());
    }
    else
    {
        ui->lineEdit_1->setText(ui->table->item(ui->table->currentRow(),1)->text());
        ui->lineEdit_2->setText(ui->table->item(ui->table->currentRow(),2)->text());
        ui->lineEdit_3->setText(ui->table->item(ui->table->currentRow(),3)->text());
        ui->lineEdit_4->setText(ui->table->item(ui->table->currentRow(),4)->text());
    }
    ui->combo->setCurrentIndex(CurTable);
    refDialog();
}
void Database::acceptSearch()
{
    QSqlQuery q;
    bool f=false;
    int a=ui->combo->currentIndex();
    QString st="select * from "+ui->combo->currentText();
    if(ui->lineEdit_1->text()!="")
    {
        if(a==3)
            st+=" where "+ui->label_1->text()+"= "+ui->lineEdit_1->text();
            else
                st+=" where "+ui->label_1->text()+"= '"+ui->lineEdit_1->text()+"'";
        f=true;
    }
    if(ui->lineEdit_2->text()!="")
    {
        if(!f)
            st+=" where ";
        else
            st+=" and ";
        f=true;
        if(a==0 || a==4)
            st+=ui->label_2->text()+"='"+ui->lineEdit_2->text()+"'";
        else
            st+=ui->label_2->text()+"="+ui->lineEdit_2->text();
    }
    if(ui->lineEdit_3->text()!="")
        {
            if(!f)
                st+=" where ";
            else
                st+=" and ";
            if(a==1 || a==2)
                st+=ui->label_3->text()+"="+ui->lineEdit_3->text();
            else
                st+=ui->label_3->text()+"='"+ui->lineEdit_3->text()+"'";
            f=true;
        }
    if(ui->lineEdit_4->text()!="")
        {
            if(!f)
                st+=" where ";
            else
                st+=" and ";
            if(a!=2)
                st+=ui->label_4->text()+"='"+ui->lineEdit_4->text()+"'";
            else
                st+=ui->label_4->text()+"="+ui->lineEdit_4->text();
            f=true;
        }
    if(ui->Sort1->isChecked())
        st+=" order by "+ui->label_1->text();
    if(ui->Sort2->isChecked())
        st+=" order by "+ui->label_2->text();
    if(ui->Sort3->isChecked())
        st+=" order by "+ui->label_3->text();
    if(ui->Sort4->isChecked())
        st+=" order by "+ui->label_4->text();
    st+=";";
    q.prepare(st);
    if(!q.exec())
    {
        QMessageBox::information(this,"Error",q.lastError().text());
        return;
    }
    else
    {
        QString str="select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME = '"+ui->combo->currentText()+"'";
        QSqlQuery query(str);
        QStringList st;
        int NumCol=0;
        while(query.next())
        {
            ++NumCol;
            st.push_back(query.value(0).toString());
        }
        ui->table->setColumnCount(NumCol);
        ui->table->setHorizontalHeaderLabels(st);
        int k=0;
        ui->table->setRowCount(0);
        while(q.next())
        {
            int a=ui->table->rowCount();
            ui->table->setRowCount(a+1);
            for(int i=0;i<NumCol;++i)
            {
                QTableWidgetItem *item = new QTableWidgetItem(q.value(i).toString());
                ui->table->setItem(k,i,item);
            }
            ++k;
        }
    }
    CurTable=ui->combo->currentIndex();
    ui->Dialog->setVisible(false);
    ui->searchButton->setVisible(false);
    ui->Sort1->setVisible(false);
    ui->Sort2->setVisible(false);
    ui->Sort3->setVisible(false);
    ui->Sort4->setVisible(false);

    ui->table->setEnabled(1);
    ui->add->setEnabled(1);
    ui->menu_3->setEnabled(1);
    ui->change->setEnabled(1);
    ui->del->setEnabled(1);
    ui->search->setEnabled(1);
    ui->menu->setEnabled(1);
}
void Database::search()
{
    ui->Sort1->click();
    ui->Dialog->setVisible(true);
    if(CurTable!=-1)
        ui->combo->setCurrentIndex(CurTable);
    else
        ui->combo->setCurrentIndex(0);
    ui->table->setEnabled(0);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);
    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->searchButton->setVisible(1);
    refDialog();
}
void Database::buttonDel()
{
    if(ui->table->currentRow()==-1)
    {
        QMessageBox::information(this,"Error","No one select");
        return;
    }
    QString tab,col;
    switch(CurTable)
    {
    case 0:
        tab="Label";
        col="Label_ID";
        break;
    case 1:
        tab="Band";
        col="Band_ID";
        break;
    case 2:
        tab="Album";
        col="Album_ID";
        break;
    case 3:
        break;
    case 4:
        tab="Musician";
        col="Musician_ID";
        break;
    default:
       QMessageBox::information(this,"Error","Something wrong, sry");
       return;
    }
    QSqlQuery q;
    if(CurTable==3)
            q.prepare("delete MusInBand where Musician_ID = " + ui->table->item(ui->table->currentRow(),0)->text()+
                      "and Band_ID = "+ui->table->item(ui->table->currentRow(),1)->text());
        else
            q.prepare("delete "+tab+" where "+col+"= " +ui->table->item(ui->table->currentRow(),0)->text());
    if(!q.exec())
        QMessageBox::information(this,"Error",q.lastError().text());
    else
    {
        ui->table->removeRow(ui->table->currentRow());
         QMessageBox::information(this,"Success","done");
     }

}
void Database::acceptDialog()
{
    if(ui->lineEdit_1->text()=="" || ui->lineEdit_2->text()=="" || ui->lineEdit_3->text()=="" ||
       ui->lineEdit_4->text()=="")
    {
        QMessageBox::information(this,tr("Error"),tr("Somes fields are empty"));
        return;
    }
    else
    {
        QString st="select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME = '"+ui->combo->currentText()+"'";
        QSqlQuery q(st);
        QString str="";
        if(ui->combo->currentIndex()!=3)
            q.next();
        q.next();
        int a=1;
        str=q.value(0).toString();
        while(q.next())
        {
            str+=", "+q.value(0).toString();
            ++a;
        }
        q.prepare("insert into "+ui->combo->currentText()+" ("+str+") "
                  "values (?, ?, ?, ?)");

        if(ui->combo->currentIndex()==3)
            q.addBindValue(ui->lineEdit_1->text().toInt());
        else
            q.addBindValue(ui->lineEdit_1->text());

        if(ui->combo->currentIndex()==0 || ui->combo->currentIndex()==4)
            q.addBindValue(ui->lineEdit_2->text());
        else
            q.addBindValue(ui->lineEdit_2->text().toInt());

        if(ui->combo->currentIndex()==1 || ui->combo->currentIndex()==2)
            q.addBindValue(ui->lineEdit_3->text().toInt());
        else
           q.addBindValue(ui->lineEdit_3->text());
        if(ui->combo->currentIndex()==2)
            q.addBindValue(ui->lineEdit_4->text().toInt());
        else
            q.addBindValue(ui->lineEdit_4->text());

        if(!q.exec())
        {
            QMessageBox::information(this,tr("Error"),q.lastError().text());
            return;
        }
        else
        {
            QMessageBox::information(this,tr("Success"),tr("query done"));
            ui->Dialog->setVisible(false);
            ui->addButton->setVisible(false);
            ui->table->setEnabled(1);
            ui->add->setEnabled(1);
            ui->change->setEnabled(1);
            ui->del->setEnabled(1);
            ui->menu_3->setEnabled(1);
            ui->search->setEnabled(1);
            ui->menu->setEnabled(1);
            switch(ui->combo->currentIndex())
            {
            case 0:
                showLabel();
                break;
            case 1:
                showBand();
                break;
            case 2:
                showAlbum();
                break;
            case 3:
                showMusInBand();
                break;
            case 4:
                showMusician();
                break;
            }

        }
    }
}
void Database::cancelDialog()
{
    ui->Dialog->setVisible(false);
    ui->addButton->setVisible(false);
    ui->searchButton->setVisible(false);
    ui->changeButton->setVisible(false);
    ui->combo->setVisible(1);
    ui->label->setVisible(1);
    ui->table->setEnabled(1);
    ui->add->setEnabled(1);
    ui->menu_3->setEnabled(1);
    ui->change->setEnabled(1);
    ui->del->setEnabled(1);
    ui->search->setEnabled(1);
    ui->menu->setEnabled(1);
    ui->Sort1->setVisible(false);
    ui->Sort2->setVisible(false);
    ui->Sort3->setVisible(false);
    ui->Sort4->setVisible(false);

}
void Database::refDialog()
{
 //   CurTable=ui->combo->currentIndex();
    if(ui->searchButton->isVisible())
    {
        ui->Sort1->setVisible(1);
        ui->Sort2->setVisible(1);
        ui->Sort3->setVisible(1);
        ui->Sort4->setVisible(1);
    }
    ui->label_1->setVisible(1);
    ui->label_2->setVisible(1);
    ui->label_3->setVisible(1);
    ui->label_4->setVisible(1);
    ui->lineEdit_1->setVisible(1);
    ui->lineEdit_2->setVisible(1);
    ui->lineEdit_3->setVisible(1);
    ui->lineEdit_4->setVisible(1);
    QSqlQuery q;
    QString st;
    st="select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME = '"+ui->combo->currentText()+"'";
    q.prepare(st);
    q.exec();
    if(ui->combo->currentIndex()!=3)
        q.next();
    q.next();
    ui->label_1->setText(q.value(0).toString());
    q.next();
    ui->label_2->setText(q.value(0).toString());
    q.next();
    ui->label_3->setText(q.value(0).toString());
    q.next();
    ui->label_4->setText(q.value(0).toString());
}
Database::~Database()
{
    delete ui;
}
void Database::MyOutputTable(QString TN)
{
    QString st="select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME = '"+TN+"'";
    QSqlQuery q(st);
    QStringList a;
    int NumCol=0;
    while(q.next())
    {
        ++NumCol;
        a.push_back(q.value(0).toString());
    }
    ui->table->setColumnCount(NumCol);
    ui->table->setHorizontalHeaderLabels(a);
    st="select * from "+TN;
    q.prepare(st);
    q.exec();
    int k=0;
    ui->table->setRowCount(0);
    while(q.next())
    {
        int a=ui->table->rowCount();
        ui->table->setRowCount(a+1);
        for(int i=0;i<NumCol;++i)
        {
            QTableWidgetItem *item = new QTableWidgetItem(q.value(i).toString());
            ui->table->setItem(k,i,item);
        }
        ++k;
    }
}
void Database::addRecord()
{
    ui->Dialog->setVisible(1);
    ui->addButton->setVisible(1);

    if(CurTable!=-1)
        ui->combo->setCurrentIndex(CurTable);
    else
        ui->combo->setCurrentIndex(0);

    ui->table->setEnabled(0);
    ui->add->setEnabled(0);
    ui->change->setEnabled(0);
    ui->del->setEnabled(0);
    ui->menu_3->setEnabled(false);
    ui->search->setEnabled(0);
    ui->menu->setEnabled(0);

    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");


    refDialog();
}
void Database::showMusician()
{
   MyOutputTable("Musician");
   CurTable=4;
}
void Database::showMusInBand()
{
   MyOutputTable("MusInBand");
   CurTable=3;
}
void Database::showBand()
{
   MyOutputTable("Band");
   CurTable=1;
}
void Database::showLabel()
{
   MyOutputTable("Label");
   CurTable=0;
}
void Database::showAlbum()
{
    MyOutputTable("Album");
    CurTable=2;
}
void Database::myconnect1()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setDatabaseName("mydb");
    db.setUserName("sa");
    db.setPort(1433);
    db.setPassword("12345");
    if(db.open())
    {
        QMessageBox::information(this,tr("Success"),tr("Connection established"));
        QSqlQuery q("select table_name from INFORMATION_SCHEMA.TABLES");
        while(q.next())
            ui->combo->addItem(q.value(0).toString());
        ui->add->setEnabled(true);
        ui->search->setEnabled(true);
        ui->del->setEnabled(true);
        ui->change->setEnabled(true);
        ui->menu_2->setEnabled(true);
        ui->createReport->setEnabled(true);
        ui->menu_3->setEnabled(true);
    }
    else
        QMessageBox::information(this,tr("Error"),tr("Connection failed"));
}





















