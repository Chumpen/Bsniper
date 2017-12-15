#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringListModel>
#include <QSqlTableModel>
#include <QStringList>
#include <QDebug>
#include <QVariant>
#include <QCloseEvent>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <qprocess.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:

private slots:
     void loadDB ();
     void useDB();
     void readGroupName();
     void closeEvent(QCloseEvent *event);
     void on_comboBox_group_activated(const QString &arg1);

     void on_actionNew_triggered();

     void on_buttonBox_new_rejected();

     void on_buttonBox_new_accepted();

     void insertDB(int uTable);
     void on_listView_doubleClicked(const QModelIndex &index);

     void on_actionAddGroup_triggered();

     void on_toolButton_cancle_clicked();

     void on_toolButton_ok_clicked();

     void on_listView_clicked(const QModelIndex &index);

     void on_actionDel_triggered();

     void deleteBookmark(int Dtab);
     void on_actionEDIT_triggered();

     void UpdateDB(int uTable);
     void on_actionAbout_triggered();

     void on_actionModifyGroup_triggered();

     void on_toolButton_clicked();

     void on_toolButton_2_clicked();

     void on_toolButton_3_clicked();

     void on_toolButton_4_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
