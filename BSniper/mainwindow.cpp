#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>

QString baseDir = QDir::homePath();
QString BSnip_path = baseDir + "/.BSniper";
QString databaseFile = "/BSniper.3db";
QString DB_path = BSnip_path + databaseFile;
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

QString Group_Active;
QString name_Active;
QString name_Active_Comment;

QString Active_Browser = "/usr/bin/google-chrome-stable ";

bool EditBM = false;
bool EditGroup = false;
bool aboutShow = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame_new->hide();
    ui->frame_group->hide();
    ui->frame_about->hide();

    loadDB();
    useDB();

    Group_Active = "Google";

    on_comboBox_group_activated(Group_Active);
    name_Active = "";
    ui->statusBar->showMessage(name_Active);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    db.close();
    event->accept();
}

void MainWindow::loadDB()
{

    db.setDatabaseName(DB_path);
    qDebug() << DB_path;

}

void MainWindow::useDB()
{
    if (db.open()){
        qDebug() << "Hi......!";
        readGroupName();
    }
}

//uTable = 0 เพิ่มรายชื่อใหม่ลงในฐานข้อมูล
//uTable = 1 เพิ่มกลุ่มใหม่ลงฐานข้อมูล
void MainWindow::insertDB(int uTable)
{
    if (uTable == 0){
        QSqlQuery query(db);
        query.prepare("INSERT INTO bsniper ( bs_name, bs_groupname, bs_address, bs_remark ) "
                      "VALUES (:bs_name, :bs_groupname, :bs_address, :bs_remark )");
        query.bindValue( ":bs_name", ui->lineEdit_name->text());
        query.bindValue( ":bs_groupname", Group_Active);
        query.bindValue( ":bs_address", ui->lineEdit_Address->text());
        query.bindValue( ":bs_remark", ui->lineEdit_Remark->text());
        query.exec();
    }else if (uTable == 1) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO bs_group ( g_name, g_remark ) "
                      "VALUES (:g_name, :g_remark )");
        query.bindValue( ":g_name", ui->lineEdit_Ngroup->text());
        query.bindValue( ":g_remark", "Group");
        query.exec();
    }
}

//เอาข้อมูลกลุ่มจากฐานข้อมูลใส่ใน comboBox_group
void MainWindow::readGroupName ()
{
    QSqlQuery query(db);
    query.prepare("SELECT g_name FROM bs_group ORDER BY g_name;");
    query.exec();
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    ui->comboBox_group->setModel(queryModel);

}

void MainWindow::on_comboBox_group_activated(const QString &arg1)
{
    QSqlQuery query(db);
    Group_Active = arg1;
    query.prepare("SELECT bs_name, bs_address FROM bsniper WHERE bs_groupname == '"+arg1+"' ORDER BY bs_name;");
    query.exec();
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    ui->listView->setModel(queryModel);
    ui->statusBar->showMessage("Use on Group:: " + Group_Active);
    ui->label_5->setText(arg1);
    ui->comboBox_group->setCurrentText(Group_Active);
}

void MainWindow::on_actionNew_triggered()
{
    ui->frame_new->show();

}

void MainWindow::on_buttonBox_new_rejected()
{
    ui->lineEdit_name->clear();
    ui->lineEdit_Address->clear();
    ui->lineEdit_Remark->clear();
    ui->frame_new->hide();
    EditBM = false;
}

void MainWindow::on_buttonBox_new_accepted()
{
    if(!EditBM){
        insertDB(0);
        on_comboBox_group_activated(Group_Active);
        ui->lineEdit_name->clear();
        ui->lineEdit_Address->clear();
        ui->lineEdit_Remark->clear();
        ui->frame_new->hide();
        EditBM = false;
    }else {
        UpdateDB(0);
        on_comboBox_group_activated(Group_Active);
        ui->lineEdit_name->clear();
        ui->lineEdit_Address->clear();
        ui->lineEdit_Remark->clear();
        ui->frame_new->hide();
        EditBM = false;
    }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    QSqlQuery query(db);
    query.prepare("SELECT bs_address FROM bsniper WHERE bs_name == '"+index.data().toString()+"';");
    query.exec();
    qDebug() << query.first();
    qDebug() << query.record().value(0).toString();

    QProcess *proc = new QProcess();
    if(ui->radioChrome->isChecked()){
        Active_Browser = "/usr/bin/google-chrome-stable ";
    }else if (ui->radioFirefox->isChecked()) {
        Active_Browser = "/usr/bin/firefox ";
    }else if (ui->radioOpera->isChecked()) {
        Active_Browser = "/usr/bin/opera ";
    }
    proc->start(Active_Browser + query.record().value(0).toString());
}

void MainWindow::on_actionAddGroup_triggered()
{
    ui->frame_group->show();
}

void MainWindow::on_toolButton_cancle_clicked()
{
    ui->frame_group->hide();
    EditGroup = false;
}

void MainWindow::on_toolButton_ok_clicked()
{
    if (!EditGroup){
    insertDB(1);
    readGroupName();
    Group_Active = ui->lineEdit_Ngroup->text();
    on_comboBox_group_activated(Group_Active);
    ui->lineEdit_Ngroup->clear();
    ui->frame_group->hide();
    EditGroup = false;
    }else {
        UpdateDB(1);
        readGroupName();
        Group_Active = ui->lineEdit_Ngroup->text();
        on_comboBox_group_activated(Group_Active);
        ui->lineEdit_Ngroup->clear();
        ui->frame_group->hide();
        EditGroup = false;
    }
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    name_Active = index.data().toString();
    QSqlQuery query(db);
    query.prepare("SELECT bs_remark FROM bsniper WHERE bs_name == '"+index.data().toString()+"';");
    query.exec();
    qDebug() << query.first();

    name_Active_Comment = query.record().value(0).toString();

    if(name_Active_Comment == ""){
        ui->statusBar->showMessage(name_Active);
    }else {
         ui->statusBar->showMessage(name_Active_Comment);
    }
    query.clear();

}

void MainWindow::on_actionDel_triggered()
{
    //Del bookmark
    deleteBookmark(0);
    on_comboBox_group_activated(Group_Active);
    name_Active = "";
    ui->statusBar->showMessage(name_Active);
}

void MainWindow::deleteBookmark(int Dtab)
{
    if (Dtab == 0){
        QSqlQuery query(db);
        query.prepare("DELETE FROM bsniper WHERE bs_name = '" + name_Active + "'");
        query.exec();
    }else if (Dtab == 1) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO bs_group ( g_name, g_remark ) "
                      "VALUES (:g_name, :g_remark )");
        query.bindValue( ":g_name", ui->lineEdit_Ngroup->text());
        query.bindValue( ":g_remark", "Group");
        query.exec();
        query.clear();
    }
}

//แก้ไขรายการที่เลือก
void MainWindow::on_actionEDIT_triggered()
{

    EditBM = true;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM bsniper WHERE bs_name == '"+name_Active+"';");
    query.exec();
    qDebug() << query.first();

    ui->lineEdit_name->setText(query.record().value(2).toString());
    ui->lineEdit_Address->setText(query.record().value(3).toString());
    ui->lineEdit_Remark->setText(query.record().value(4).toString());

    ui->frame_new->show();
}

//uTable = 0
//uTable = 1
void MainWindow::UpdateDB(int uTable)
{
    if (uTable == 0){
        QSqlQuery query(db);
        QString SQLX = "UPDATE bsniper SET bs_name = '" + ui->lineEdit_name->text() +
                      "', bs_groupname = '" + Group_Active +
                      "', bs_address = '" + ui->lineEdit_Address->text() +
                      "', bs_remark = '" + ui->lineEdit_Remark->text() +
                      "' WHERE bs_name = '" + name_Active + "'; ";
        query.exec(SQLX);
        qDebug() << SQLX;

    }else if (uTable == 1) {
        QSqlQuery query(db);
        QString SQLG = "UPDATE bs_group SET g_name = '"+ui->lineEdit_Ngroup->text()+"', g_remark = 'Group' "
                                                                                    "WHERE g_name = '"+ Group_Active +"';";
        query.exec(SQLG);
        query.prepare("SELECT * FROM bsniper;");
        query.exec();
        qDebug() << query.first();

        QString SQLXL = "UPDATE bsniper SET bs_groupname = '" + ui->lineEdit_Ngroup->text() + "' WHERE bs_groupname = '"+ Group_Active +"';";
        query.exec(SQLXL);
        qDebug() << SQLG;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    if (aboutShow == false){
        ui->frame_about->show();
        aboutShow = true;
    }else if (aboutShow == true) {
        ui->frame_about->hide();
        aboutShow = false;
    }
}

//แก้ไขกลุ่มปัจจุบัน
void MainWindow::on_actionModifyGroup_triggered()
{
    EditGroup = true;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM bs_group WHERE g_name == '"+ Group_Active +"';");
    query.exec();
    qDebug() << query.first();

    ui->lineEdit_Ngroup->setText(Group_Active);
    ui->frame_group->show();
}

void MainWindow::on_toolButton_clicked()
{
    QProcess *proc = new QProcess();
    if(ui->radioChrome->isChecked()){
        Active_Browser = "/usr/bin/chromium-browser ";
    }else if (ui->radioFirefox->isChecked()) {
        Active_Browser = "/usr/bin/firefox ";
    }else if (ui->radioOpera->isChecked()) {
        Active_Browser = "/usr/bin/opera ";
    }
    proc->start(Active_Browser + "https://www.blogger.com/");
}

void MainWindow::on_toolButton_2_clicked()
{
    QProcess *proc = new QProcess();
        if(ui->radioChrome->isChecked()){
            Active_Browser = "/usr/bin/chromium-browser ";
        }else if (ui->radioFirefox->isChecked()) {
            Active_Browser = "/usr/bin/firefox ";
        }else if (ui->radioOpera->isChecked()) {
            Active_Browser = "/usr/bin/opera ";
        }
        proc->start(Active_Browser + "https://www.facebook.com/");
}

void MainWindow::on_toolButton_3_clicked()
{
    QProcess *proc = new QProcess();
        if(ui->radioChrome->isChecked()){
            Active_Browser = "/usr/bin/chromium-browser ";
        }else if (ui->radioFirefox->isChecked()) {
            Active_Browser = "/usr/bin/firefox ";
        }else if (ui->radioOpera->isChecked()) {
            Active_Browser = "/usr/bin/opera ";
        }
        proc->start(Active_Browser + "https://translate.google.co.th");
}

void MainWindow::on_toolButton_4_clicked()
{
    QProcess *proc = new QProcess();
        if(ui->radioChrome->isChecked()){
            Active_Browser = "/usr/bin/chromium-browser ";
        }else if (ui->radioFirefox->isChecked()) {
            Active_Browser = "/usr/bin/firefox ";
        }else if (ui->radioOpera->isChecked()) {
            Active_Browser = "/usr/bin/opera ";
        }
        proc->start("/opt/google/chrome/google-chrome --profile-directory=Default --app-id=hmjkmjkepdijhoojdojkdfohbdgmmhki");
}
