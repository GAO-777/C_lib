#include "tools.h"
#include<QHeaderView>

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ███████████████████████████───█─█─█─██─█────█───█───█────█─██─█───█████████████████████████████
  ███████████████████████████─███─█─█──█─█─██─██─███─██─██─█──█─█─███████████████████████████████
  ███████████████████████████───█─█─█─█──█─█████─███─██─██─█─█──█───█████████████████████████████
  ███████████████████████████─███─█─█─██─█─██─██─███─██─██─█─██─███─█████████████████████████████
  ███████████████████████████─███───█─██─█────██─██───█────█─██─█───█████████████████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
WORD* QListToWord(QList<unsigned int> *InData)
{
    WORD* OutData  = new WORD[InData->size()];
    QList<unsigned int>::iterator i;
    int j=0;
    for (i = InData->begin(); i != InData->end(); ++i){
        OutData[j] = *i;
        j++;
    }
    return OutData;
}

QList<unsigned int> WordToQList(WORD* InData, int size)
{
    QList<unsigned int> OutData = QList<unsigned int>();
    for (int i=0; i < size; ++i)
        OutData.append(InData[i]);
    return OutData;
}

void printSendData(QString str, QList<unsigned int> *Addr, QList<unsigned int> *Data)
{
    qDebug()<<"============== "<<str<<"  "<<QTime::currentTime().toString("HH:mm:ss")<<" ==============";
    for(int i=0;i<Addr->size();i++){
        qDebug()<<QString( QString("[%1]<=[%2]")
                           .arg(Addr->at(i)).arg(Data->at(i)));
    }
}

bool CLToQList(QString openFileName,QList<unsigned int> *Addr, QList<unsigned int> *Data){
    /* Функция преобразует пользовательский файл в список команд */
    QStringList inputData;
    QFile file(openFileName);

    if ( (file.exists()) && (file.open(QIODevice::ReadOnly))){
        while(!file.atEnd())
            inputData << file.readLine();   // Отписываем все строки в массив
        file.close();
/*==========================================================================================*\
- - - - - - - - - - - - - Расшивровка - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
        // - - - пробегаем по массиву строк - - - //
        QStringList::iterator i;
        for (i = inputData.begin(); i != inputData.end(); ++i){
            QString line = i->toStdString().c_str();
            line.remove("\n");

            if(line.indexOf("-") >= 0) continue;

            // - - - Запись - - - - - - - - - - - - - - - - - - - - - - - - //
            if(line.indexOf("<=") > 0){
                QStringList data = line.split("<=");

                // - - - поиск адреса - - - - //
                if(data[0].indexOf(":") > 0){
                    int startAdd = data[0].split(":")[0].toInt();
                    int endAdd = data[0].split(":")[1].toInt();
                    for(int i=0;i<=endAdd-startAdd;i++)
                        Addr->append(startAdd+i);
                }else
                    Addr->append(data[0].toInt());

                // - - - поиск данных - - - - //
                if(data[1].indexOf(":") > 0){
                    int startData = data[1].split(":")[1].toInt();
                    int endData = data[1].split(":")[1].toInt();
                    for(int i=0;i<=endData-startData;i++)
                        Data->append(startData+i);
                }else
                    Data->append(data[1].toInt());
            }

            // - - - Чтение - - - - - - - - - - - - - - - - - - - - - - - - //
            if(line.indexOf("=>") > 0){
                QStringList data = line.split("=>");

                // - - - поиск адреса - - - - //
                if(data[0].indexOf(":") > 0){
                    int startAdd = data[0].split(":")[0].toInt();
                    int endAdd = data[0].split(":")[1].toInt();
                    for(int i=0;i<=endAdd-startAdd;i++)
                        Addr->append(startAdd+i);
                }else
                    Addr->append(data[0].toInt());

            }



        }
    }
}


/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ██████████████████████████████────█────█─██─█───█────█─███───██████████████████████████████████
  ██████████████████████████████─██─█─██─█──█─█─███─██─█─███─████████████████████████████████████
  ██████████████████████████████─████─██─█─█──█───█─██─█─███───██████████████████████████████████
  ██████████████████████████████─██─█─██─█─██─███─█─██─█─███─████████████████████████████████████
  ██████████████████████████████────█────█─██─█───█────█───█───██████████████████████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
#include <QScrollBar>
#include <QTextBlock>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);

    setMinimumHeight(100);
    setSizePolicy(policy);


    QPalette p = palette();
    setPalette(p);

    history = new QStringList;
    historyPos = 0;
    insertPrompt(false);
    isLocked = false;
}

void Console::commandHandler(QString commandStr)
{
    QStringList commandArray = commandStr.split(" ");

    if(commandArray[0] == "do"){
        qDebug()<<"sdfs";
    }
}

void Console::print(QString s, QString type)
{
    textCursor().insertBlock();
    QTextCharFormat format;

    if(type == "e"){
        format.setForeground(Qt::red);
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "ERROR : " + s;
    } else if(type == "w"){
        format.setForeground(QBrush(QColor(236,124,38)));
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "WARNING : " + s;
    } else if(type == "m"){
        format.setForeground(Qt::blue);
        QFont font;
        font.setBold(false);
        format.setFont(font);
    }
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(s);
}


void Console::onEnter()
{
    moveCursor(QTextCursor::End);
    if(textCursor().positionInBlock() == prompt.length())
    {
        insertPrompt();
        return;
    }
    QString cmd = textCursor().block().text().mid(prompt.length());

    //isLocked = true;
    historyAdd(cmd);
    commandHandler(cmd);
}

void Console::lock(bool lock)
{
    isLocked = lock;
    insertPrompt();
}

void Console::keyPressEvent(QKeyEvent *e)
{
    if(isLocked)
        return;
    switch (e->key()) {
    case Qt::Key_Return:
            onEnter();
        break;
    case Qt::Key_Backspace:
        if( e->modifiers() == Qt::NoModifier
               && textCursor().positionInBlock() > prompt.length())
                QPlainTextEdit::keyPressEvent(e);
        break;
   case Qt::Key_Up:
            historyBack();
        break;

    case Qt::Key_Down :
             historyForward();
         break;

    case Qt::Key_Left :
        if(textCursor().positionInBlock() != prompt.length())
             moveCursor(QTextCursor::PreviousCharacter);
         break;

    case Qt::Key_End :
             moveCursor(QTextCursor::EndOfBlock);
         break;

    case Qt::Key_Home :
             moveCursor(QTextCursor::StartOfBlock);
             moveCursor(QTextCursor::NextWord);
         break;

    default:
                QPlainTextEdit::keyPressEvent(e);
        break;
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}


void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
void Console::output(QString s, QString type)
{
    textCursor().insertBlock();
    QTextCharFormat format;


    if(type == "e"){
        format.setForeground(Qt::red);
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "ERROR : " + s;
    } else if(type == "w"){
        format.setForeground(QBrush(QColor(236,124,38)));
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "WARNING : " + s;
    } else if(type == "m"){
        format.setForeground(Qt::blue);
        QFont font;
        font.setBold(false);
        format.setFont(font);
    }
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(s);
    insertPrompt();
    isLocked = false;
}

void Console::insertPrompt(bool insertNewBlock)
{
    if(insertNewBlock)
        textCursor().insertBlock();
    QTextCharFormat format;
    format.setForeground(Qt::black);
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(prompt);
    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void Console::historyAdd(QString cmd)
{
    history->append(cmd);
    historyPos = history->length();
}

void Console::historyBack()
{
    if(!historyPos)
    return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(prompt + history->at(historyPos-1));
    setTextCursor(cursor);
    historyPos--;
}

void Console::historyForward()
{
    if(historyPos == history->length())
    return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    if(historyPos == history->length() - 1)
    cursor.insertText(prompt);
    else
    cursor.insertText(prompt + history->at(historyPos + 1));
    setTextCursor(cursor);
    historyPos++;
}

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████
  ████████████────█────█─███─█─███─█────█─██─█────██─███───█───█───██████████████████
  ████████████─██─█─██─█──█──█──█──█─██─█──█─█─██──█─████─██─████─███████████████████
  ████████████─████─██─█─█─█─█─█─█─█────█─█──█─██──█─████─██───██─███████████████████
  ████████████─██─█─██─█─███─█─███─█─██─█─██─█─██──█─████─████─██─███████████████████
  ████████████────█────█─███─█─███─█─██─█─██─█────██───█───█───██─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
CommandList::CommandList(QTableWidget *parent) :
    QTableWidget(parent)
{
    Name = "";
    Items = new QList<unsigned int>();
    Addr = new QList<unsigned int>();
    Data = new QList<unsigned int>();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&CommandList::customContextMenuRequested, this, &CommandList::customContextMenuRequest);

    // = = = Выполняем заполнение QTableWidget записями с помощью цикла = = = //
    setColumnCount(3);   // Указываем число колонок
    setShowGrid(true);   // Включаем сетку
    setHorizontalHeaderLabels(QStringList() << "" << " Address"<<"Data" );
    resizeColumnToContents(0);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    resizeColumnToContents(0);

    QFont font = horizontalHeader()->font();
    font.setBold (true);
    horizontalHeader()->setFont(font);
}

void CommandList::createRow(int s)
{
    // = = = Выполняем заполнение QTableWidget записями с помощью цикла = = = //
    for(int i = size; i<s; i++){
        // Вставляем строку
        insertRow(i);
        // Создаём виджет, который будет содержать в себе чекбокс
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();      // объявляем и инициализируем чекбокс
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // создаём слой с привязкой к виджету
        layoutCheckBox->addWidget(checkBox);            // Устанавливаем чекбокс в слой
        layoutCheckBox->setAlignment(Qt::AlignCenter);  // Отцентровываем чекбокс
        layoutCheckBox->setContentsMargins(0,0,0,0);    // Устанавливаем нулевые отступы

        checkBox->setChecked(true);

        setCellWidget(i,0, checkBoxWidget);
        // Далее забираем все данные из результата запроса и устанавливаем в остальные поля
        setItem(i,1, new QTableWidgetItem("0"));
        setItem(i,2, new QTableWidgetItem("0"));
        setRowHeight(i,25);
        setColumnWidth(i,60);

    }
    resizeColumnToContents(0);

    size = s;
}

void CommandList::deleteRow()
{
    this->removeRow(this->currentRow());
}

void CommandList::updateQLists()
{
    Items->clear();
    Addr->clear();
    Data->clear();

    for(int i = 0; i < size; i++){
        // Забираем виджет из слоя и кастуем его в QCheckBox
        QWidget *item = (cellWidget(i,0));
        QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
        QTableWidgetItem* lineAddr = this->item(i, 1);
        QTableWidgetItem* lineData = this->item(i, 2);
        if(checkB->isChecked())
            if(lineAddr->text() != ""){
                Items->append(static_cast<unsigned int>(i));
                Addr->append(static_cast<unsigned int>(lineAddr->text().toInt()));
                Data->append(static_cast<unsigned int>(lineData->text().toInt()));
            }
    }
}

void CommandList::fill_Data(QList<unsigned int> *Items,unsigned int* InData)
{
    QList<unsigned int>::iterator j;

    int i =0;
    for (j = Items->begin(); j != Items->end(); ++j){
         QTableWidgetItem* lineData = this->item(*j, 2);
         lineData->setText(QString::number(InData[i]));
         i++;
    }
}

void CommandList::fill_Data(QList<unsigned int> *Items, QList<unsigned int> *InData)
{
    QList<unsigned int>::iterator j;
    int i =0;
    for (j = Items->begin(); j != Items->end(); ++j){
         QTableWidgetItem* lineData = this->item(*j, 2);
         lineData->setText(QString::number(InData->at(i)));
         i++;
    }
}

void CommandList::getDataToSave()
{
    Items->clear();
    Addr->clear();
    Data->clear();
    for(int i = 0; i < size; i++){
        // Забираем виджет из слоя и кастуем его в QCheckBox
        QWidget *item = (cellWidget(i,0));
        QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
        QTableWidgetItem* lineAddr = this->item(i, 1);
        QTableWidgetItem* lineData = this->item(i, 2);
        Items->append(checkB->isChecked());
        Addr->append(lineAddr->text().toInt());
        Data->append(lineData->text().toInt());
    }

}

void CommandList::setDataToLoad(int size,QList<unsigned int>* Items,QList<unsigned int>* Addr,QList<unsigned int>* Data)
{
    clearList();
    createRow(size);
    for (int i=0;i<size;i++){
        QWidget *item = (cellWidget(i,0));
        QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
        QTableWidgetItem* lineAddr = this->item(i, 1);
        QTableWidgetItem* lineData = this->item(i, 2);
        lineAddr->setText(QString::number(Addr->at(i)));
        lineData->setText(QString::number(Data->at(i)));
        if(Items->at(i) == 1) checkB->setCheckState(Qt::Checked);
        else checkB->setCheckState(Qt::Unchecked);
    }
}

void CommandList::clearList()
{
    for(int i = 0; i < this->rowCount(); i++){
        // Забираем виджет из слоя и кастуем его в QCheckBox
        QWidget *item = (cellWidget(i,0));
        QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
        QTableWidgetItem* lineAddr = this->item(i, 1);
        QTableWidgetItem* lineData = this->item(i, 2);
        checkB->setChecked(true);
        lineAddr->setText("0");
        lineData->setText("0");
    }
}

void CommandList::save(QString saveFileName)
{
    QSettings* List_Save = new QSettings(saveFileName,QSettings::IniFormat);
    // Сохраняем данные QList о состоянии ячеек
    getDataToSave();

    // = = = = = Сохраняем лист  = = = = = //
    List_Save->beginWriteArray(Name);
    for (int i = 0; i < size; ++i) {
        List_Save->setArrayIndex(i);
        List_Save->setValue("Check",Items->at(i));
        List_Save->setValue("Addr", Addr->at(i));
        List_Save->setValue("Data", Data->at(i));
    }
    List_Save->endArray();
    List_Save->sync();
}

void CommandList::load(QString openFileName)
{
    QSettings* List_Save = new QSettings(openFileName,QSettings::IniFormat);
    // = = = = = Загружаем лист  = = = = = //
    QList<unsigned int>* WriteList_Items = new QList<unsigned int>();
    QList<unsigned int>* WriteList_Addr = new QList<unsigned int>();
    QList<unsigned int>* WriteList_Data= new QList<unsigned int>();
    int size_w = List_Save->beginReadArray(Name);
    for (int i = 0; i < size_w; ++i){
        List_Save->setArrayIndex(i);
        WriteList_Items->append(List_Save->value("Check").toInt());
        WriteList_Addr->append(List_Save->value("Addr").toInt());
        WriteList_Data->append(List_Save->value("Data").toInt());
      }
    List_Save->endArray();
    setDataToLoad(size_w,WriteList_Items,WriteList_Addr,WriteList_Data);
    delete WriteList_Items;
    delete WriteList_Addr;
    delete WriteList_Data;
}
void CommandList::customContextMenuRequest(const QPoint &pos)
{
    QMenu* menu = new QMenu();
    auto DeleteRow_Action = menu->addAction("Delete");
    connect(DeleteRow_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            for(int p = range.bottomRow();p>=range.topRow();p--)
                this->removeRow(p);
        }
    });

    auto InsertRow_Action = menu->addAction("Insert");
    connect(InsertRow_Action, &QAction::triggered, this, [this](){
        int row_pos;
        if(this->currentRow() == -1) row_pos = 0;
        else row_pos = this->currentRow();
        insertRow(row_pos);
        // Создаём виджет, который будет содержать в себе чекбокс
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();      // объявляем и инициализируем чекбокс
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // создаём слой с привязкой к виджету
        layoutCheckBox->addWidget(checkBox);            // Устанавливаем чекбокс в слой
        layoutCheckBox->setAlignment(Qt::AlignCenter);  // Отцентровываем чекбокс
        layoutCheckBox->setContentsMargins(0,0,0,0);    // Устанавливаем нулевые отступы
        checkBox->setChecked(true);
        setCellWidget(row_pos,0, checkBoxWidget);
        setItem(row_pos,1, new QTableWidgetItem("0"));
        setItem(row_pos,2, new QTableWidgetItem("0"));
        setRowHeight(row_pos,25);
        setColumnWidth(row_pos,60);
    });

    auto Increment_Action = menu->addAction("Increment");
    connect(Increment_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            int topValueleft =  this->item(range.topRow(),range.leftColumn())->text().toInt();
            int topValueRight =  this->item(range.topRow(),range.rightColumn())->text().toInt();
            for(int p=range.topRow()+1;p<=range.bottomRow();p++){
                this->item(p,range.leftColumn())->setText(QString::number(topValueleft+p));
                this->item(p,range.rightColumn())->setText(QString::number(topValueRight+p));
            }
        }

    });

    auto Decrement_Action = menu->addAction("Decrement");
    connect(Decrement_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            int topValueleft =  this->item(range.topRow(),range.leftColumn())->text().toInt();
            int topValueRight =  this->item(range.topRow(),range.rightColumn())->text().toInt();
            for(int p=range.topRow()+1;p<=range.bottomRow();p++){
                this->item(p,range.leftColumn())->setText(QString::number(topValueleft-p));
                this->item(p,range.rightColumn())->setText(QString::number(topValueRight-p));
            }
        }

    });

    auto Clear_Action = menu->addAction("Clear");
    connect(Clear_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            for(int p=range.topRow();p<=range.bottomRow();p++){
                if(range.leftColumn()!=0)
                    this->item(p,range.leftColumn())->setText(QString::number(0));
                this->item(p,range.rightColumn())->setText(QString::number(0));
            }
        }

    });

    auto Disable_Action = menu->addAction("Disable");
    connect(Disable_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            for(int p=range.topRow();p<=range.bottomRow();p++){
                QWidget *item = (cellWidget(p,0));
                QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
                checkB->setChecked(false);
            }
        }

    });

    auto Enable_Action = menu->addAction("Enable");
    connect(Enable_Action, &QAction::triggered, this, [this](){
        QList<QTableWidgetSelectionRange> listRange = selectedRanges();
        for(int i=0;i<listRange.size();i++){
            auto range = listRange.at(i);
            for(int p=range.topRow();p<=range.bottomRow();p++){
                QWidget *item = (cellWidget(p,0));
                QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
                checkB->setChecked(true);
            }
        }

    });

    menu->exec(QCursor::pos());
}
/*===============================================================================================*\
  ████████████████████████████████████████████████████████████████████████████████████████████████
  ███████████████████████████────█─███─█─███─█───█────██────█───█───██████████████████████████████
  ███████████████████████████─██─█─███─█─███─██─██─██──█─████─████─███████████████████████████████
  ███████████████████████████────█─█─█─█─█─█─██─██─██──█─█──█───██─███████████████████████████████
  ███████████████████████████─█─██─────█─────██─██─██──█─██─█─████─███████████████████████████████
  ███████████████████████████─█─███─█─███─█─██───█────██────█───██─███████████████████████████████
  ████████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/

RW_Widget::RW_Widget(QWidget *parent) :
           QWidget(parent)
{
    Save_pb = new QPushButton("Save");
    connect(Save_pb,&QPushButton::clicked,this,&RW_Widget::saveListsData);
    Load_pb = new QPushButton("Load");
    connect(Load_pb,&QPushButton::clicked,this,&RW_Widget::loadListsData);
    Clear_pb= new QPushButton("Clear all");
    connect(Clear_pb,&QPushButton::clicked,this,&RW_Widget::clearRows);
    // - - - Компановка - - - //
    QHBoxLayout *THL = new QHBoxLayout();
    QSpacerItem* rightSpaserTH = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    THL->addSpacerItem(rightSpaserTH);
    THL->addWidget(Save_pb);
    THL->addWidget(Load_pb);
    THL->addWidget(Clear_pb);
    QSpacerItem* leftSpaserTH = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    THL->addSpacerItem(leftSpaserTH);

    CreateRows = new QPushButton("Add lines");
    connect(CreateRows,&QPushButton::clicked,this,&RW_Widget::addRows);
    Num_Rows = new QSpinBox();
    Num_Rows->setMinimumWidth(50);
    Num_Rows->setMaximum(100);
    // - - - Компановка - - - //
    QHBoxLayout *ADL = new QHBoxLayout();
    ADL->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    ADL->addWidget(CreateRows);
    ADL->addWidget(Num_Rows);
    ADL->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));


    QHBoxLayout *RW_HL = new QHBoxLayout();
    WriteList   = new CommandList();
    WriteList->Name = "WriteList";
    ReadList    = new CommandList();
    ReadList->Name = "ReadList";
    RW_HL->addWidget(WriteList);
    RW_HL->addWidget(ReadList);

    Write_pb = new QPushButton("Write");
    connect(Write_pb,&QPushButton::clicked,this,&RW_Widget::write_pb_clicked);
    Read_pb  = new QPushButton("Read");
    connect(Read_pb,&QPushButton::clicked,this,&RW_Widget::read_pb_clicked);

    QHBoxLayout *RWbutton_HL = new QHBoxLayout();
    RWbutton_HL->addWidget(Write_pb);
    RWbutton_HL->addWidget(Read_pb);

    QVBoxLayout *MVL = new QVBoxLayout();
    MVL->insertLayout(0,THL,0);
    MVL->insertLayout(1,ADL,0);
    MVL->insertLayout(2,RW_HL,0);
    MVL->insertLayout(3,RWbutton_HL,0);
    this->setLayout(MVL);
}

void RW_Widget::addRows()
{
    int num_rows = Num_Rows->value();
    WriteList->createRow(num_rows);
    ReadList->createRow(num_rows);
}

void RW_Widget::clearRows()
{
    WriteList->clearList();
    ReadList->clearList();
}

void RW_Widget::saveListsData()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                "Save file",
                                                QDir::currentPath(),
                                                "INI files (*.ini);;All files (*.*)");
    WriteList->save(saveFileName);
    ReadList->save(saveFileName);
}

void RW_Widget::loadListsData()
{
    QString openFileName = QFileDialog::getOpenFileName(this,
                                                        "Open file",
                                                        QDir::currentPath(),
                                                        "INI files (*.ini);;All files (*.*)");
   WriteList->load(openFileName);
   ReadList->load(openFileName);
}

QList<unsigned int>* RW_Widget::getWriteAddr()
{
    WriteList->updateQLists();
    return WriteList->Addr;
}

QList<unsigned int> *RW_Widget::getWriteData()
{
    WriteList->updateQLists();
    return WriteList->Data;
}

QList<unsigned int> *RW_Widget::getReadAddr()
{
    ReadList->updateQLists();
    return ReadList->Addr;
}

void RW_Widget::fillReadList(QList<unsigned int>* data)
{
    ReadList->fill_Data(ReadList->Items,data);
}

void RW_Widget::block()
{
    Write_pb->setEnabled(false);
    Read_pb->setEnabled(false);
}

void RW_Widget::unblock()
{
    Write_pb->setEnabled(true);
    Read_pb->setEnabled(true);
}
/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ████████████────█────█─██─█─██─█───█────█───█───█────█─██─█───█───█───█─█─█────████████████████
  ████████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█─███─████─██─█─█─██─████████████████
  ████████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█───█───██─██─█─█────████████████████
  ████████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─███─█─████─██─█─█─███████████████████
  ████████████────█────█─██─█─██─█───█────██─██───█────█─██─█───█───██─██───█─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/

#include "ui_connectionsetup.h"

ConnectionsBar *ParentCB;

ConnectionSetup::ConnectionSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionSetup)
{
    ui->setupUi(this);
    ParentCB = static_cast<ConnectionsBar*>(parent);
    ParentCB->SettingsConnection_pb->setEnabled(false);
    connect(ui->USB_ConnectionType_rb,&QRadioButton::clicked,this,&ConnectionSetup::PageSwitch);
    connect(ui->UDP_ConnectionType_rb,&QRadioButton::clicked,this,&ConnectionSetup::PageSwitch);
    connect(ui->SlowLink_ConnectionType_rb,&QRadioButton::clicked,this,&ConnectionSetup::PageSwitch);

    USB_page        = new QWidget(this);
    UDP_page        = new QWidget(this);
    SlowLink_page   = new QWidget(this);

    ui->MVL_Pages->addWidget(USB_page);
    ui->MVL_Pages->addWidget(UDP_page);
    ui->MVL_Pages->addWidget(SlowLink_page);
    USB_page->setVisible(false);
    UDP_page->setVisible(false);
    SlowLink_page->setVisible(false);

    init_Pages();

}

ConnectionSetup::~ConnectionSetup()
{
    delete ui;
}

void ConnectionSetup::PageSwitch()
{
    if(ui->USB_ConnectionType_rb->isChecked()){
        UDP_page->setVisible(false);
        USB_page->setVisible(true);
        NumOfDownLink_sp->setVisible(false);
        Num_DL_label->setVisible(false);
    }else if(ui->UDP_ConnectionType_rb->isChecked()){
        UDP_page->setVisible(true);
        USB_page->setVisible(false);
    }else if(ui->SlowLink_ConnectionType_rb->isChecked()){
        UDP_page->setVisible(false);
        USB_page->setVisible(true);
        NumOfDownLink_sp->setVisible(true);
        Num_DL_label->setVisible(true);
    }
}

void ConnectionSetup::UpdateDiviceList()
{
    USB_SN_cb->clear();
    FT_STATUS ftStatus;
    DWORD numDevs;
    // create the device information list
    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus == FT_OK) {
        printf("Number of devices is %d\n",numDevs);
    }
    if (numDevs > 0) {
        USB_DevInfo =
                (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
        ftStatus = FT_GetDeviceInfoList(USB_DevInfo,&numDevs);
        if (ftStatus == FT_OK) {
            for (int i = 0; i < static_cast<int>(numDevs); i++) {
                USB_SN_cb->addItem(QString(USB_DevInfo[i].SerialNumber));
            }
        }
    }
    USB_SN_cb->update();
}

void ConnectionSetup::USB_page_ChangedDevice(int i)
{
    USB_Description_l->setText(USB_DevInfo[i].Description);
    USB_ID_l->setText(QString::number(USB_DevInfo[i].ID));
    USB_LocId_l->setText(QString::number(USB_DevInfo[i].LocId));

}

void ConnectionSetup::UpdateSetup()
{
    if(ui->USB_ConnectionType_rb->isChecked()){
        ParentCB->ConnectionInfo.connectionType = USB;
        if(USB_SN_cb->currentIndex()>=0){
            ParentCB->ConnectionInfo.USB_SN = USB_DevInfo[USB_SN_cb->currentIndex()].SerialNumber;
        }
    }else if(ui->UDP_ConnectionType_rb->isChecked()){
        ParentCB->ConnectionInfo.connectionType = UDP;
        ParentCB->ConnectionInfo.IP_addrress = IPaddress->text();
        ParentCB->ConnectionInfo.Port = Port->value();
    }
    else if(ui->SlowLink_ConnectionType_rb->isChecked()){
        ParentCB->ConnectionInfo.connectionType = SlowLink;
        ParentCB->ConnectionInfo.NumOfDownLink = NumOfDownLink_sp->value();
        if(USB_SN_cb->currentIndex()>=0){
            ParentCB->ConnectionInfo.USB_SN = USB_DevInfo[USB_SN_cb->currentIndex()].SerialNumber;
        }
    }

    ParentCB->SettingsChanged(ParentCB->ConnectionInfo);
}

void ConnectionSetup::init_Pages()
{
/*==========================================================================================*\
- - - - - - - - - - - - - USB PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    QVBoxLayout* MVL_USBpage = new QVBoxLayout();
    QHBoxLayout* HL_Description = new QHBoxLayout();
    QHBoxLayout* HL_ID = new QHBoxLayout();
    QHBoxLayout* HL_LocId = new QHBoxLayout();

    USB_SN_cb = new QComboBox(this);
    connect(USB_SN_cb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,&ConnectionSetup::USB_page_ChangedDevice);

    USB_Description_l = new QLabel();
    USB_ID_l = new QLabel();
    USB_LocId_l = new QLabel();

    USB_UpdateDeviceList_pb = new QPushButton("Обновить список устройств");
    connect(USB_UpdateDeviceList_pb,&QPushButton::clicked,this,&ConnectionSetup::UpdateDiviceList);

    QLabel* Description_l =  new QLabel("Description: ");
    Description_l->setFont(QFont("Segoe UI",11, QFont::Bold));
    HL_Description->addWidget(Description_l,0,Qt::AlignLeft);
    HL_Description->addWidget(USB_Description_l,1,Qt::AlignLeft);

    QLabel* ID_l =  new QLabel("ID:          ");
    ID_l->setFont(QFont("Segoe UI",11, QFont::Bold));
    HL_ID->addWidget(ID_l,0,Qt::AlignLeft);
    HL_ID->addWidget(USB_ID_l,1,Qt::AlignLeft);

    QLabel* LocID_l =  new QLabel("LocID:       ");
    LocID_l->setFont(QFont("Segoe UI",11, QFont::Bold));
    HL_LocId->addWidget(LocID_l,0,Qt::AlignLeft);
    HL_LocId->addWidget(USB_LocId_l,1,Qt::AlignLeft);

    MVL_USBpage->addWidget(USB_SN_cb);
    MVL_USBpage->insertLayout(1,HL_Description,0);
    MVL_USBpage->insertLayout(2,HL_ID,0);
    MVL_USBpage->insertLayout(3,HL_LocId,0);
    MVL_USBpage->addWidget(USB_UpdateDeviceList_pb);

    USB_page->setLayout(MVL_USBpage);
    UpdateDiviceList();

/*==========================================================================================*\
- - - - - - - - - - - - - UDP PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
   // QVBoxLayout* MVL_UDPpage = new QVBoxLayout();
    //QHBoxLayout* HL_IP = new QHBoxLayout();
    //QHBoxLayout* HL_Port = new QHBoxLayout();

    QGridLayout* MVL_UDPpage = new QGridLayout();
    IPaddress = new QLineEdit();
    QLabel* IP_label = new QLabel("IP address :");
    IP_label->setAlignment(Qt::AlignRight);
    IP_label->setMinimumSize(100,20);
    IPaddress->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    IPaddress->setMinimumSize(100,20);
    IPaddress->setText("192.168.0.20");

    MVL_UDPpage->addWidget(IP_label,0,0,Qt::AlignRight);
    MVL_UDPpage->addWidget(IPaddress,0,1,Qt::AlignLeft);
   // HL_IP->addWidget(IP_label,0,Qt::AlignHCenter);
   // HL_IP->addWidget(IPaddress,0,Qt::AlignHCenter);
    //MVL_UDPpage->addWidget(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QLabel* Port_label = new QLabel("Port :");
    //Port_label->setFont(QFont("Segoe UI",11, QFont::Bold));
    Port_label->setAlignment(Qt::AlignRight);
    Port = new QSpinBox();
    Port->setMinimumSize(100,20);
    //Port->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    Port->setMaximum(900000);
    Port->setValue(49320);
    MVL_UDPpage->addWidget(Port_label,1,0,Qt::AlignRight);
    MVL_UDPpage->addWidget(Port,1,1,Qt::AlignLeft);
    //HL_Port->addWidget(Port_label,0,Qt::AlignRight);
    //HL_Port->addWidget(Port,0,Qt::AlignLeft);
    //HL_Port->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

   // MVL_UDPpage->insertLayout(0,HL_IP,0);
   // MVL_UDPpage->insertLayout(1,HL_Port,0);

    UDP_page->setLayout(MVL_UDPpage);

/*==========================================================================================*\
- - - - - - - - - - - - - Slow Link PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    NumOfDownLink_sp = new QSpinBox();
    Num_DL_label = new QLabel("Number Down Link :");

    QHBoxLayout* SL_HL = new QHBoxLayout();
    SL_HL->addWidget(Num_DL_label,0,Qt::AlignLeft);
    SL_HL->addWidget(NumOfDownLink_sp,1,Qt::AlignLeft);
    MVL_USBpage->insertLayout(0,SL_HL,0);
}

void ConnectionSetup::on_ApplyButton_clicked()
{
    ParentCB->SettingsConnection_pb->setEnabled(true);
    ParentCB->openDevice_pb->setEnabled(true);
    UpdateSetup();
    close();
}

void ConnectionSetup::closeEvent(QCloseEvent *event)
{
    on_ApplyButton_clicked();
    event->accept();
}
/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ███████████────█────█─██─█─██─█───█────█───█───█────█─██─█───█────██────█────██████████████████
  ███████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█─███─██──█─██─█─██─██████████████████
  ███████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█───█────██────█────██████████████████
  ███████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─███─█─██──█─██─█─█─███████████████████
  ███████████────█────█─██─█─██─█───█────██─██───█────█─██─█───█────██─██─█─█─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/

ConnectionsBar::ConnectionsBar(QWidget *parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *ConnectionsBarLayout = new QHBoxLayout(this);
    NameConnection          = new QLabel();
    SettingsConnection_pb   = new QPushButton("Setup",parent);
    connect(SettingsConnection_pb,&QPushButton::clicked,this,&ConnectionsBar::callConnectionSetup);
    openDevice_pb           = new QPushButton("Сonnect",parent);
    connect(openDevice_pb,&QPushButton::clicked,this,&ConnectionsBar::openDevice);

    Status_l = new QLabel();
    Status_l->setFixedSize(15,15);
    Status_l->setStyleSheet("background-color: red;");
    ConnectionsBarLayout->addWidget(SettingsConnection_pb,0,Qt::AlignLeft);
    ConnectionsBarLayout->addWidget(openDevice_pb,1,Qt::AlignLeft);
    ConnectionsBarLayout->addWidget(Status_l,3,Qt::AlignLeft);
    ConnectionsBarLayout->addWidget(NameConnection,4,Qt::AlignLeft);

    ConnectionInfo.connectionType = NONE;
}

void ConnectionsBar::setStatus(bool status, QString name)
{
    if (status){
        Status_l->setStyleSheet("background-color: green;");
        NameConnection->setText(name);
        SettingsConnection_pb->setEnabled(false);
        openDevice_pb->setText("Disconnect");
    }
    else{
         Status_l->setStyleSheet("background-color: red;");
         NameConnection->setText("");
         SettingsConnection_pb->setEnabled(true);
         openDevice_pb->setText("Сonnect");
    }
}

void ConnectionsBar::callConnectionSetup()
{
    ConnectionSetup* Setup = new ConnectionSetup(this);
    Setup->setWindowFlags(Qt::Window);
    Setup->show();
    openDevice_pb->setEnabled(false);
    SettingsConnection_pb->setEnabled(false);
}

void ConnectionsBar::openDevice()
{
    if(ConnectionInfo.connectionType == NONE)
        callConnectionSetup();
    else
        emit OpenDevice();
}

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  █████████────█────█─██─█─██─█───█────█───█───█────█─██─█─███─█────█─██─█────█────█───█────█████
  █████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█──█──█─██─█──█─█─██─█─████─███─██─█████
  █████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█─█─█─█────█─█──█────█─█──█───█────█████
  █████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─█─███─█─██─█─██─█─██─█─██─█─███─█─██████
  █████████────█────█─██─█─██─█───█────██─██───█────█─██─█─███─█─██─█─██─█─██─█────█───█─█─██████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
ConnectionManager::ConnectionManager()
{
    // - - - Инициализация - - - //
    NameConnection = "";
    ConnectionInfo.connectionType = NONE;
    statusConnection = false;
    Eth_Device = new Ethernet_Interface();
    USB_Device = new USB_Interface();
    MCHS = new MCHS_Imitator();
}

ConnectionManager::ConnectionManager(Connection_Info CI)
{
    // - - - Инициализация - - - //
    NameConnection = "";
    ConnectionInfo = CI;
    statusConnection = false;
    Eth_Device = new Ethernet_Interface();
    USB_Device = new USB_Interface();
    MCHS = new MCHS_Imitator();
}

bool ConnectionManager::connectDevice()
{
/*==========================================================================================*\
- - - - - - - - - - - - - UDP PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    //34665
    if(ConnectionInfo.connectionType == UDP){
        if(statusConnection == false){
            NameConnection = ConnectionInfo.IP_addrress;
            statusConnection = Eth_Device->init(ConnectionInfo.IP_addrress.toStdString().c_str(),ConnectionInfo.Port);
            if(!statusConnection){
                Message = "Connection error! : "+QString::number(Eth_Device->LastError);
                return false;
            }
            return true;
        }
        if(statusConnection == true){
            statusConnection = !Eth_Device->closeSocket();
            if(!statusConnection){
                Message = "Disconnection error! : "+QString::number(Eth_Device->LastError);
                return false;
            }
            return true;
        }
    }
/*==========================================================================================*\
- - - - - - - - - - - - - USB PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    if(ConnectionInfo.connectionType == USB){
        if(statusConnection == false){
            // - - - Подлючение - - - //
            NameConnection = ConnectionInfo.USB_SN;
            USB_Device->Select_Device(ConnectionInfo.USB_SN.toStdString().c_str());
            statusConnection = USB_Device->Open_Device();
            if(!statusConnection)
                Message = "Connection error!";
        }
        if(statusConnection == true){
            // - - - Отключение - - - //
            statusConnection = !USB_Device->Close_Device();
            if(!statusConnection)
                Message = "Disconnection error!";
        }
    }
/*==========================================================================================*\
- - - - - - - - - - - - - Slow Link PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    if(ConnectionInfo.connectionType == SlowLink){
        if(statusConnection == false){
            // - - - Подлючение - - - //
            NameConnection = "DownLink: " + QString::number(ConnectionInfo.NumOfDownLink);
            MCHS->setSN(ConnectionInfo.USB_SN);
            statusConnection = MCHS->openUSB();
            MCHS->GeneratorsOff();
            if(!statusConnection){
                Message = "Connection error!";
                return false;
            }
            return true;
        }
        if(statusConnection == true){
            // - - - Отключение - - - //
            statusConnection = !MCHS->closeUSB();
            if(!statusConnection){
                Message = "Disconnection error!";
                return false;
            }
            return true;
        }
    }
}

bool ConnectionManager::write(QList<unsigned int> *Addr, QList<unsigned int> *Data)
{
    if(Addr->size() == 0){
        Message = "Address list is empty!";
        return false;
    }

    if(ConnectionInfo.connectionType == UDP){
        if(Addr->size() == Data->size()){
            bool status;
            status = Eth_Device->write(QListToWord(Addr),QListToWord(Data),Addr->size());
            if(!status){
                Message = "Sending error! : "+QString::number(Eth_Device->LastError);
                return false;
            }
        }else{
            Message = "Addr->size() != Data->size()";
            return false;
        }
    }

    if(ConnectionInfo.connectionType == SlowLink){
        if(Addr->size() == Data->size()){
            bool status;
            status = MCHS->CLink_TxRx(ConnectionInfo.NumOfDownLink,202,Addr,Data);
            if(!status){
                Message = "Sending error! : "+QString::number(Eth_Device->LastError);
                return false;
            }
        }else{
            Message = "Addr->size() != Data->size()";
            return false;
        }
    }


    return true;
}

bool ConnectionManager::read(QList<unsigned int> *Addr, QList<unsigned int> *Data)
{
    if(Addr->size() == 0){
        Message = "Address list is empty!";
        return false;
    }

    if(ConnectionInfo.connectionType == UDP){
            WORD* OutData  = new WORD[Addr->size()];
            bool status;
            status = Eth_Device->read(QListToWord(Addr),OutData,Addr->size());
            if(!status){
                Message = "Receive error! : "+QString::number(Eth_Device->LastError);
                return false;
            }
            *Data = WordToQList(OutData,Addr->size());
    }

    if(ConnectionInfo.connectionType == SlowLink){
        bool status;
        status = MCHS->CLink_TxRx(ConnectionInfo.NumOfDownLink,193,Addr,new QList<unsigned int>);
        QList<unsigned int> *OutData = new QList<unsigned int>();
        MCHS->ReadDataFromDownLink(ConnectionInfo.NumOfDownLink,OutData);
        for(int i = 0;i<Addr->size();i++){
            Data->append(OutData->at(i));
        }
        if(!status){
            Message = "Receive error! : "+QString::number(Eth_Device->LastError);
            return false;
        }
    }

    return true;
}


