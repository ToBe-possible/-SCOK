#include "userman_work.h"
#include "ui_userman_work.h"
#include "adduser.h"
#include "moduser.h"

#include <QList>
#include <QFont>
#include <QStandardItemModel>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QCompleter>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStringList>
#include <QMenu>
#include <QtCore>
#include "xlsxdocument.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
userman_work::userman_work(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userman_work)
{
    ui->setupUi(this);
    init();
}


userman_work::~userman_work()
{
    user_sql::close_db();
    delete ui;
}

void userman_work::init()
{
    connect(ui->line_search,&QLineEdit::textChanged,this,&userman_work::loadSearchData);
    user_sql::connect_db();
    init_font();
    loadTableData();
    adaptorTable();
}

void userman_work::init_font()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);
    this->setFont(font_man);
    ui->btn_clear->setFont(font_man);
    ui->btn_excel->setFont(font_man);
    ui->btn_exit->setFont(font_man);
    ui->btn_mod->setFont(font_man);
    ui->btn_plus->setFont(font_man);
    ui->btn_select->setFont(font_man);
    ui->btn_update->setFont(font_man);
    ui->line_search->setFont(font_man);
    //控制光标就是在输入id的lineEdit里面
    ui->line_search->setFocus();




}

void userman_work::adaptorTable()
{
    //设置表格宽度
    ui->tab_sql->setColumnWidth(0,120);
    ui->tab_sql->setColumnWidth(1,145);
    ui->tab_sql->setColumnWidth(2,120);
    ui->tab_sql->setColumnWidth(3,100);
    ui->tab_sql->setColumnWidth(4,100);
    ui->tab_sql->setColumnWidth(5,100);
    ui->tab_sql->setColumnWidth(6,100);

    //将单元格中的文本水平和垂直居中对齐
    ui->tab_sql->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tab_sql->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行



    //置水平表头的最后一部分可以伸缩，以填充剩余的空间
    ui->tab_sql->horizontalHeader()->setStretchLastSection(true);

    //隐藏垂直表头。
    // ui->tab_sql->verticalHeader()->setVisible(false);

    //设置表格的焦点策略为无焦点，即表格不会接受焦点
    ui->tab_sql->setFocusPolicy(Qt::NoFocus);

    //设置表格的选择模式为单选
    ui->tab_sql->setSelectionMode(QAbstractItemView::SingleSelection);

    //设置选择行为为选择整行
    ui->tab_sql->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置编辑触发为不可编辑，禁止用户编辑表格中的内容
    ui->tab_sql->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //启用交替行颜色，以提高表格的可读性
    ui->tab_sql->setAlternatingRowColors(true);
}

QList<QString> userman_work::getSelectedData()
{
    //获取当前选中行的内容

    //如果我只选中了一行
    QList<QString> selectedData;

    QList<QTableWidgetItem *> selectedItems = ui->tab_sql->selectedItems();

    //    QTableWidgetItem *item = new QTableWidgetItem();
    int count = selectedItems.count();

    for(int i = 0 ; i < count; i++){
        //        int row = ui->tab_sql->row(selectedItems.at(i));
        QTableWidgetItem *item = selectedItems.at(i);
        QString text = item->text(); //获取内容
        selectedData.append(text);
        qDebug()<<text;
    }

    return selectedData;
}

void userman_work::loadSearchData()
{
    QString name_scan = ui->line_search->text();

    int row_count = ui->tab_sql->rowCount();

    if(name_scan.isEmpty()){

        for(int i=0;i<row_count;i++){
            ui->tab_sql->setRowHidden(i,false);
        }
    }
    else{
        QList<QTableWidgetItem*>item_list = ui->tab_sql->findItems(ui->line_search->text(),Qt::MatchContains);

        for(int i=0;i<row_count;i++){
            ui->tab_sql->setRowHidden(i,true);

        }


        if(!item_list.empty()){
            for(int i=0;i<item_list.count();i++){
                ui->tab_sql->setRowHidden(item_list.at(i)->row(),false);
            }
        }

    }
}

void userman_work::loadTableData()
{
    ui->tab_sql->setRowCount(0);
    ui->tab_sql->setSortingEnabled(false);
    QStringList headerLabels;
    headerLabels << "姓名" << "电话" << "密码" << "权限";
    ui->tab_sql->setHorizontalHeaderLabels(headerLabels);

    QSqlQuery sql(m_db);


    QString str_select = QString("select * from user");
    if(sql.exec(str_select)){
        qDebug()<<"str_select successful";
        int rowCount = 0;
        //        ui->tab_sql->setColumnCount(sql.record().count());
        while(sql.next()){
            ui->tab_sql->insertRow(rowCount);
            for(int i = 0;i<sql.record().count();i++){
                // qDebug()<<sql.value(i).toString();
                QTableWidgetItem *item = new QTableWidgetItem(sql.value(i).toString());
                ui->tab_sql->setItem(rowCount,i,item);
            }
            rowCount++;

        }
        // ui->tab_sql->update();
        //        ui->tab_sql->repaint();
        // ui->tab_sql->viewport()->update();
    }
    else{
        qDebug() << "Error: Fail to query table" << sql.lastError();
    }

    for (int i = 0; i < ui->tab_sql->rowCount(); i++) {
        for (int j = 0; j < ui->tab_sql->columnCount(); j++) {
            if (ui->tab_sql->item(i, j)) {  // 检查指针是否有效
                ui->tab_sql->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            } else {
                qDebug() << "ui->tab_sql->item(" << i << ", " << j << ") is nullptr";
            }
        }
    }

}

void userman_work::on_btn_excel_clicked()
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString curTime = datetime.toString("yyyy-MM-dd hh:mm:ss");
    // QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./库存情况_'%1'.xlsx").arg(curTime),tr("Excel Files(*.xlsx)"));    //设置保存的文件名
    QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./账号管理.xlsx"),tr("Excel Files(*.xlsx)"));    //设置保存的文件名


    if (!fileName.isEmpty()) {
        // 执行导出操作，使用得到的 fileName
        qDebug()<<"fileName"<<fileName;
        QXlsx::Document xlsx;
        //列的个数
        int columnCount = ui->tab_sql->columnCount();
        //行的个数
        int rowCount = ui->tab_sql->rowCount();
        //绘制Excel的列名
        // for(int i = 0 ; i < columnCount ;i++){
        //     QTableWidgetItem *headerItem = ui->tab_sql->horizontalHeaderItem(i);

        //     if(headerItem){
        //         // 获取表头项的文本
        //         QString columnName = headerItem->text();

        //         // 将列名写入Excel文档
        //         xlsx.write(1, i + 1, columnName);  // Excel的行和列是从1开始的

        //     }
        // }

        //绘制Excel的行的内容

        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < columnCount; ++col) {
                QTableWidgetItem *item = ui->tab_sql->item(row, col);
                if (item) {
                    // 获取单元格的文本，并写入到 Excel 中
                    xlsx.write(row + 1, col + 1, item->text());  // Excel的行和列是从1开始的
                }
            }
        }
        xlsx.saveAs(fileName);
    }
}


void userman_work::on_btn_exit_clicked()
{
    exit(0);
}


void userman_work::on_btn_plus_clicked()
{
    addUser *add = new addUser();
    add->show();
}


void userman_work::on_btn_mod_clicked()
{
    QList<QString> selectedData = getSelectedData();
    if(!selectedData.empty()){
        // this->hide();
        modUser *mod = new modUser();
        //发送打开更新选中行的信号
        connect(this,&userman_work::sendSelectLineData,mod,&modUser::receiveData);
        emit sendSelectLineData(selectedData);
        mod->show();
    }
    else{
        QMessageBox::information(this,"提醒","您未选中需修改的货品",QMessageBox::Yes);
    }

}


void userman_work::on_btn_update_clicked()
{
    loadTableData();

}

