#pragma once
#ifndef BIGMAPDIALOG_H
#define BIGMAPDIALOG_H
#include <QDialog>
#include <QStandardItemModel>
#include <QWebEngineView>




namespace Ui {
class BigMapDialog;
}

class BigMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BigMapDialog(QWidget *parent = 0);
    ~BigMapDialog();
    void LoadMap();
    void InitMap();
    void InitTable();

    QString MapPath;
    QWebEngineView* m_pWebView;    // QWebView对象



private:
    Ui::BigMapDialog *ui;
    QString Jing;
    QString Wei;
    QString Path;
    QStandardItemModel *table_model_SearchResult;



public slots:
    void receiveJingWeiData(QString JingValue,QString WeiVaule,QString path);
    void table_view_row_clicked(const QModelIndex &index);
    void importCSVFile_Gaode();
};

#endif // BIGMAPDIALOG_H
