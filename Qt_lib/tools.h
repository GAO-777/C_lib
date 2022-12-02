#ifndef TOOLS_H
#define TOOLS_H

// - - - QT - - - - - - - - - - //
#include <QTableWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QList>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QComboBox>
#include <QList>
#include <QCloseEvent>
#include <QSpinBox>
#include <QValidator>
#include <QMenu>
#include <QPlainTextEdit>
#include <QMessageBox>

// - - - Windows - - - - - - - - - - //
#include <Windows.h>

// - - - BINP - - - - - - - - - - //
#include "../../C_lib/Interfaces/ethernet_interface.h"
#include "../../C_lib/Interfaces/USB_Interface.h"
#include "binp_device.h"
/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ███████████████████████████───█─█─█─██─█────█───█───█────█─██─█───█████████████████████████████
  ███████████████████████████─███─█─█──█─█─██─██─███─██─██─█──█─█─███████████████████████████████
  ███████████████████████████───█─█─█─█──█─█████─███─██─██─█─█──█───█████████████████████████████
  ███████████████████████████─███─█─█─██─█─██─██─███─██─██─█─██─███─█████████████████████████████
  ███████████████████████████─███───█─██─█────██─██───█────█─██─█───█████████████████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
WORD* QListToWord(QList<unsigned int>* InData);
QList<unsigned int> WordToQList(WORD* InData, int size);
void printSendData(QString str,QList<unsigned int>* Addr,QList<unsigned int>* Data);
QList<unsigned int> WordToQList(WORD* InData, int size);
bool CLToQList(QString openFileName, QList<unsigned int> *Addr, QList<unsigned int> *Data);
QList<unsigned int>* EthDatagramToList(QByteArray* datagram);

enum ConnectionType{
    NONE        = 0,
    USB         = 1,
    UDP         = 2,
    SlowLink    = 3
};

struct Connection_Info {
    ConnectionType connectionType;
    // - - - USB - - - //
    QString USB_SN;
    QString USB_Description;
    QString USB_ID;
    QString USB_LocId;
    // - - - UDP - - - //
    QString IP_addrress;
    int Port;
    // - - - SlowLink - - - //
    int NumOfDownLink;  // Номер используемого DownLink'a
    bool EndToEndAddr;  // сквозная адресация (версия Юдина)
};


/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ██████████████████████████████────█────█─██─█───█────█─███───██████████████████████████████████
  ██████████████████████████████─██─█─██─█──█─█─███─██─█─███─████████████████████████████████████
  ██████████████████████████████─████─██─█─█──█───█─██─█─███───██████████████████████████████████
  ██████████████████████████████─██─█─██─█─██─███─█─██─█─███─████████████████████████████████████
  ██████████████████████████████────█────█─██─█───█────█───█───██████████████████████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void sendCMD(QString command);
    void onChange(QString);

public:
    explicit Console(QWidget *parent = nullptr);
    void commandHandler(QString commandStr);   // обработчик команд
    void print(QString s, QString type);
	void printTable(QList<unsigned int> * Data);
    void output(QString, QString type);
    void scrollDown();
    void insertPrompt(bool insertNewBlock = true);
    void onEnter();
    void lock(bool lock);

    QString prompt = "<< ";
    QString userText;
    QStringList CommandHistory;

private:
    bool isLocked;
    QStringList *history;
    int historyPos;
    void historyAdd(QString);
    void historyBack();
    void historyForward();
protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:

};

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████
  ████████████────█────█─███─█─███─█────█─██─█────██─███───█───█───██████████████████
  ████████████─██─█─██─█──█──█──█──█─██─█──█─█─██──█─████─██─████─███████████████████
  ████████████─████─██─█─█─█─█─█─█─█────█─█──█─██──█─████─██───██─███████████████████
  ████████████─██─█─██─█─███─█─███─█─██─█─██─█─██──█─████─████─██─███████████████████
  ████████████────█────█─███─█─███─█─██─█─██─█────██───█───█───██─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/

class CommandList : public QTableWidget
{
    Q_OBJECT
public:
    CommandList(QTableWidget *parent = nullptr);

    QString Name;
    int size =0;        // кол-во строк
    QList<unsigned int>* Items;
    QList<unsigned int>* Addr;
    QList<unsigned int>* Data;

    void createRow(int s);
    void deleteRow();
    void updateQLists();    // Обновление списков: Items,Addr,Data
    void fill_Data(QList<unsigned int> *Items, unsigned int* InData);
    void fill_Data(QList<unsigned int> *Items, QList<unsigned int>* InData);
    void getDataToSave();
    void setDataToLoad(int size,QList<unsigned int>* Items,QList<unsigned int>* Addr,QList<unsigned int>* Data);
    void clearList();
    void save(QString saveFileName);
    void load(QString openFileName);
    void customContextMenuRequest(const QPoint &pos);
};
/*===============================================================================================*\
  ████████████████████████████████████████████████████████████████████████████████████████████████
  ███████████████████████████────█─███─█─███─█───█────██────█───█───██████████████████████████████
  ███████████████████████████─██─█─███─█─███─██─██─██──█─████─████─███████████████████████████████
  ███████████████████████████────█─█─█─█─█─█─██─██─██──█─█──█───██─███████████████████████████████
  ███████████████████████████─█─██─────█─────██─██─██──█─██─█─████─███████████████████████████████
  ███████████████████████████─█─███─█─███─█─██───█────██────█───██─███████████████████████████████
  ████████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
class RW_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit RW_Widget(QWidget *parent = nullptr);
private:
    // - - - Компоненты - - - //
    QPushButton*    Save_pb;
    QPushButton*    Load_pb;
    QPushButton*    Clear_pb;
    QPushButton*    CreateRows;
    QSpinBox*       Num_Rows;
    CommandList*    WriteList;
    CommandList*    ReadList;
    QPushButton*    Write_pb;
    QPushButton*    Read_pb;
public:
    // - - - Методы - - - //
    void    addRows();
    void    clearRows();
    void    saveListsData();
    void    loadListsData();
    QList<unsigned int>* getWriteAddr();    // Возвращает адреса, в которые нужно писать
    QList<unsigned int>* getWriteData();    // Возвращает данные, в которые нужно записать
    QList<unsigned int>* getReadAddr();     // Возвращает адреса, в которые нужно прочесть
    void    fillReadList(QList<unsigned int>* data);
    void    block();                        // Блокирует кнопочки от использования
    void    unblock();                      // Разблокирует кнопочки от использования
signals:
    void    write_pb_clicked();
    void    read_pb_clicked();
};

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ████████████────█────█─██─█─██─█───█────█───█───█────█─██─█───█───█───█─█─█────████████████████
  ████████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█─███─████─██─█─█─██─████████████████
  ████████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█───█───██─██─█─█────████████████████
  ████████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─███─█─████─██─█─█─███████████████████
  ████████████────█────█─██─█─██─█───█────██─██───█────█─██─█───█───██─██───█─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
namespace Ui {
class ConnectionSetup;
}

class ConnectionSetup : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionSetup(QWidget *parent = nullptr);
    ~ConnectionSetup();

    void init_Pages();
    void PageSwitch();

    void UpdateDiviceList();
    void USB_page_ChangedDevice(int i);
    void UpdateSetup();

/*==========================================================================================*\
- - - - - - - - - - - - - USB PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    QWidget *USB_page;
    QComboBox* USB_SN_cb;
    QLabel* USB_Description_l;
    QLabel* USB_ID_l;
    QLabel* USB_LocId_l;
    QPushButton* USB_UpdateDeviceList_pb;
    FT_DEVICE_LIST_INFO_NODE *USB_DevInfo;  // Список подлюченных USB устройств

/*==========================================================================================*\
- - - - - - - - - - - - - UDP PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    QWidget* UDP_page;
    QLineEdit*  IPaddress;
    QSpinBox*  Port;
/*==========================================================================================*\
- - - - - - - - - - - - - SlowLink PAGE - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
\*==========================================================================================*/
    QLabel*     Num_DL_label;
    QSpinBox*   NumOfDownLink_sp;      // Номер Down_Link'a, к которому подлючено устройство
    QLabel*     EndtoEndAddr_label;
    QCheckBox*  EndtoEndAddr_cb;


private slots:
    void on_ApplyButton_clicked();
    void closeEvent(QCloseEvent *event);
private:
    Ui::ConnectionSetup *ui;
};


/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  ███████████────█────█─██─█─██─█───█────█───█───█────█─██─█───█────██────█────██████████████████
  ███████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█─███─██──█─██─█─██─██████████████████
  ███████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█───█────██────█────██████████████████
  ███████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─███─█─██──█─██─█─█─███████████████████
  ███████████────█────█─██─█─██─█───█────██─██───█────█─██─█───█────██─██─█─█─███████████████████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/

class ConnectionsBar : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionsBar(QWidget *parent = nullptr);
    QWidget *ConnectionsBar_w;
    QPushButton* SettingsConnection_pb;
    QPushButton* openDevice_pb;         // Испускает сигнал OpenDevice, чтобы родитель инициировал обмен данными

    // = = = = = Данные о выбранном соединении = = = = = //
    Connection_Info ConnectionInfo;     // Параметры соединения
    QLabel*  Status_l;                  // Cтатус соединения
    QLabel*  NameConnection;
    void setStatus(bool status, QString name);
    void callConnectionSetup();         // Вызывает окно настройки соединения
    void openDevice();                  // Испускает сигнал OpenDevice

signals:
    void SettingsChanged(Connection_Info CI);
    void OpenDevice();
};

/*===============================================================================================*\
  ███████████████████████████████████████████████████████████████████████████████████████████████
  █████████────█────█─██─█─██─█───█────█───█───█────█─██─█─███─█────█─██─█────█────█───█────█████
  █████████─██─█─██─█──█─█──█─█─███─██─██─███─██─██─█──█─█──█──█─██─█──█─█─██─█─████─███─██─█████
  █████████─████─██─█─█──█─█──█───█─█████─███─██─██─█─█──█─█─█─█────█─█──█────█─█──█───█────█████
  █████████─██─█─██─█─██─█─██─█─███─██─██─███─██─██─█─██─█─███─█─██─█─██─█─██─█─██─█─███─█─██████
  █████████────█────█─██─█─██─█───█────██─██───█────█─██─█─███─█─██─█─██─█─██─█────█───█─█─██████
  ███████████████████████████████████████████████████████████████████████████████████████████████
\*===============================================================================================*/
class ConnectionManager
{

public:
    ConnectionManager();
    ConnectionManager(Connection_Info CI);

    Connection_Info ConnectionInfo;     // Информация о подлючении
    QString NameConnection;
    bool statusConnection;              // Статус текущего подлючения
    QString Message;
    int Message_ex;
    Ethernet_Interface* Eth_Device;     // Ethernet соединение
    USB_Interface*      USB_Device;     // USB соединение
    MCHS_Imitator*      MCHS;           // Имитатор МЧС, который будет управять устройством

    void setConnectionSettings(Connection_Info CI){ ConnectionInfo = CI;}
    bool connectDevice();
    bool write(QList<unsigned int>* Addr,QList<unsigned int>* Data);
    bool read(QList<unsigned int>* Addr,QList<unsigned int>* Data);
};


#endif // TOOLS_H
