#include "sockman_work.h"
#include "ui_sockman_work.h"
#include "addsock.h"
#include "modsock.h"
#include "help.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStringList>
#include <QMenu>
#include <QtCore>
#include "xlsxdocument.h"
#include <QDir>

#include <QFileDialog>
#include<QTreeWidgetItem>


sockman_work::sockman_work(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sockman_work)
{
    //ui
    ui->setupUi(this);
    init();
    connect(ui->line_search,&QLineEdit::textChanged,this,&sockman_work::loadSearchData);
    //    modsock *mod = new modsock();
    //    connect(this,&sockman_work::gotoModsock,[=](){
    //        this->hide();
    //        mod->show();
    //    });
    // QSqlQuery sql("",m_db);

}

sockman_work::~sockman_work()
{
    sock_sql::close_db();
    delete ui;
}

Ui::sockman_work *sockman_work::getWidget()
{
    return ui;
}

void sockman_work::init()
{
    sock_sql::connect_db();
    init_font();
    adaptorTable();
    loadTableData();
    sortHeader();
}

void sockman_work::init_font()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);
    ui->btn_clear->setFont(font_man);
    ui->btn_excel->setFont(font_man);
    ui->btn_exit->setFont(font_man);
    ui->btn_mod->setFont(font_man);
    ui->btn_plus->setFont(font_man);
    ui->btn_select->setFont(font_man);
    ui->btn_update->setFont(font_man);
    ui->btn_help->setFont(font_man);
    ui->line_search->setFont(font_man);
    //控制光标就是在输入id的lineEdit里面
    ui->line_search->setFocus();

    // 设置占位文本
    ui->line_search->setPlaceholderText("请输入商品名称...");
    /*
        暂时作废
        //设置QtableWidget选中行的背景颜色是什么
        //ui->tab_sql->setStyleSheet("selection-background-color:");


        //设置QtableWidget选中行的边框是蓝色的
        // ui->tab_sql->setStyleSheet("QTableWidget::item:selected { border: 2px solid blue; }");

    */
}



void sockman_work::loadTableData()
{
    sock_sql::connect_db();

    //设置为只有零列  清空表之后再重新加载数据
    ui->tab_sql->clear();
    ui->tab_sql->setRowCount(0);
    ui->tab_sql->setSortingEnabled(false);
    //    clear_table();
    //    ui->tab_sql->setColumnCount(6);
    QStringList headerLabels;
    headerLabels << "商品名称" << "生产时间" << "仓储位置" << "库存量" << "单价" << "总价";
    ui->tab_sql->setHorizontalHeaderLabels(headerLabels);

    //    qDebug()<<"loadTableData";
   // sock_sql *dbptr = new sock_sql();
   // QSqlDatabase db = dbptr->getdatabase();

    // 连接private对象db
    QSqlQuery sql(m_db);

    if(m_db.open()){
        qDebug()<<"sockDb connect successful"<<sql.lastError();
    }
    else{
        qDebug()<<"sockDb connect fail"<<sql.lastError().text();
    }

    QString str_select = QString("select * from sock");
    // QString str_cal = QString("UPDATE sock SET total = num * price WHERE num IS NOT NULL AND price IS NOT NULL");
    // sql.exec(str_cal);
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

    setCenter();
    // sock_sql::close_db();
    //开启可排序
    // ui->tab_sql->setSortingEnabled(true);
}


void sockman_work::loadSearchData()
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

void sockman_work::receiveSelectData(const QList<QList<QString> > &selectData)
{
    ui->tab_sql->setRowCount(0);
    // 如果数据为空，直接返回
    if (selectData.isEmpty()) {
        return;
    }
    // 设置表格列数
    int columnCount = selectData.first().size();
    ui->tab_sql->setColumnCount(columnCount);

    // 填充表格数据
    for (int i = 0; i < selectData.size(); ++i) {
        const QList<QString> &rowData = selectData.at(i);

        // 插入一行
        ui->tab_sql->insertRow(ui->tab_sql->rowCount());

        // 设置每列的数据
        for (int j = 0; j < columnCount; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData.at(j));
            ui->tab_sql->setItem(i, j, item);
        }
    }
    setCenter();
}
void sockman_work::sortHeader()
{
    // ui->tab_sql->setSortingEnabled(false);
    // 获取表头
    QHeaderView *header = ui->tab_sql->horizontalHeader();

    // 设置表头可点击
    header->setSectionsClickable(true);

    // 连接点击表头的信号到槽函数
    connect(header, &QHeaderView::sectionClicked, this, &sockman_work::onHeaderClicked);

    // 启用排序功能
    ui->tab_sql->setSortingEnabled(true);
}


void sockman_work::onHeaderClicked(int index)
{
    ui->tab_sql->sortByColumn(index, ui->tab_sql->horizontalHeader()->sortIndicatorOrder());
}


void sockman_work::adaptorTable()
{
    //设置表格宽度
    ui->tab_sql->setColumnWidth(0,170);
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



    // //    首先设置QTableWidget的属性

    // ui->tab_sql->setContextMenuPolicy(Qt::CustomContextMenu);

    // //    其次，连接信号和槽，这里是实现了一个右键菜单：

    // connect(ui->tab_sql,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot(QPoint)));

}

void sockman_work::setCenter()
{
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


void sockman_work::on_btn_exit_clicked()
{
    exit(0);
}


void sockman_work::on_btn_plus_clicked()
{
    // qDebug("cur : plus");
    //    qDebug()<<"yes"<<'\n';
    // this->hide();
    addSock *addwid = new addSock();
    addwid->show();
    //    close();
    //    delete this;
}







void sockman_work::LoadTaskSlot()
{
    loadTableData();
}

//void sockman_work::clear_table()
//{
////    ui->tab_sql->clear();
////    ui->tab_sql->clearContents();
////    ui->tab_sql->setColumnCount(0);
////    ui->tab_sql->setRowCount(0);
////    qDebug()<<ui->tab_sql->rowCount();
////    while (ui->tab_sql->rowCount()>0)
////    {
////        ui->tab_sql->removeRow(0);
//////        qDebug()<<"table was cleaned";
////        qDebug()<<"cur rowCount = "<<ui->tab_sql->rowCount();
////        ui->tab_sql->update();
////        //        ui->tab_sql->repaint();
////        ui->tab_sql->viewport()->update();
////    }
////    ui->tab_sql->update();
////    //        ui->tab_sql->repaint();
////    ui->tab_sql->viewport()->update();



//    qDebug() << "Before clear: rowCount = " << ui->tab_sql->rowCount();

//    // 阻塞信号
//    ui->tab_sql->blockSignals(true);

//    ui->tab_sql->setRowCount(0);  // 直接设置行数为0，清空所有行

//    // 恢复信号
//    ui->tab_sql->blockSignals(false);

//    qDebug() << "After clear: rowCount = " << ui->tab_sql->rowCount();
//}

void sockman_work::on_btn_clear_clicked()
{
    ui->line_search->clear();
}

//void sockman_work::rightClickedSlot(const QPoint pos)
//{
//    QTableWidgetItem* item = ui->tab_sql->itemAt(pos);
//    if (NULL == item)
//    {
//        return ;
//    }
//    //    m_iRowClicked = item->row();
//    //    qDebug() << "rightClickedSlot == " << m_iRowClicked;
//    // 弹出右键菜单，
//    QMenu *pMenu = new QMenu(this);
//    pMenu->setContextMenuPolicy(Qt::CustomContextMenu);
//    //    QAction *pModifyTask = new QAction(QString(u8"修改"),this);// 修改
//    //    QAction *pDelTask = new QAction(QString(u8"删除"),this); // 删除
//    QAction *pLoadTask = new QAction(QString(u8"刷新"),this); // 刷新

//        //    pMenu->addAction(pModifyTask);
//        //    pMenu->addAction(pDelTask);
//    pMenu->addAction(pLoadTask);
//    // 连接信号和槽
//    //    connect(pModifyTask,SIGNAL(triggered()),this,SLOT(modifyTaskSlot()));
//    //    connect(pDelTask,SIGNAL(triggered()),this,SLOT(DelTaskSlot()));


//    connect(pLoadTask,SIGNAL(triggered()),this,SLOT(LoadTaskSlot()));

//    // 右键菜单显示的地方
//    pMenu->exec(QCursor::pos());

//    //    delete pModifyTask;
//    //    delete pDelTask;


//    delete pLoadTask;
//    delete pMenu;
//}


QList<QString> sockman_work::getSelectedData()
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



void sockman_work::on_btn_mod_clicked()
{

    QList<QString> selectedData = getSelectedData();
    if(!selectedData.empty()){
        // this->hide();
        modsock *mod = new modsock();
        //发送打开更新选中行的信号
        connect(this,&sockman_work::sendSelectLineData,mod,&modsock::receiveData);
        emit sendSelectLineData(selectedData);
        mod->show();
    }
    else{
        QMessageBox::information(this,"提醒","您未选中需修改的货品",QMessageBox::Yes);
    }





}


void sockman_work::on_btn_excel_clicked()
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString curTime = datetime.toString("yyyy-MM-dd hh:mm:ss");
    // QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./库存情况_'%1'.xlsx").arg(curTime),tr("Excel Files(*.xlsx)"));    //设置保存的文件名
    QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./库存情况.xlsx"),tr("Excel Files(*.xlsx)"));    //设置保存的文件名


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

void sockman_work::on_btn_select_clicked()
{
    //信号槽
    selectsock *select = new selectsock();
    connect(select,&selectsock::sendSelectData,this,&sockman_work::receiveSelectData);
    select->show();
    // connect()
}


void sockman_work::on_btn_update_clicked()
{
    loadTableData();

}


void sockman_work::on_btn_help_clicked()
{

    help *help_wid = new help();

    help_wid->show();
    // 返回值
    std::vector<QString> strs;
    // 读取文件位置
    QFile file("help.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        return ;
    }
    // 文件流
    QTextStream stream(&file);
    // 一行一行的读
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        strs.push_back(line);
    }


}

