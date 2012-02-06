#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QTextEdit>
#include <QTextStream>
#include <QListWidgetItem>
#include <QMessageBox>
#include <string>
#include <set>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void getfullPathNamesList(std::set<std::string>& set_input);
    void getSaveDirectory(std::string s_input);
    bool updateListOfWords(const QString& fileName, const QString& s_buffer);
    void loadListOfKeyWords();
    void loadListOfNames();
    void loadPluginFiles();
    void parseFile(std::set<std::string>& set_input, std::map<std::string, std::pair<std::string,std::string> >& m_output, std::map<std::string, std::pair<std::string,std::string> >& m_output2);
    void setUpComboBox_2();
    void setPluginEditFile();
    void loadPluginEditFile();

    void createActions();
    void createMenus();


private slots:
    void on_btn_addFiles_clicked();

    void on_btn_unselectFiles_clicked();

    void on_btn_unselectAll_clicked();

    void on_btn_start_clicked();

    void on_btn_saveTo_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_tree_viewOriginal_clicked(const QModelIndex &index);

    void on_tree_viewResult_clicked(const QModelIndex &index);

    void on_btn_adminPath_clicked();

    void on_btn_findPlugin_clicked();

    void on_btn_savePlugin_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_btn_editPlugin_clicked();

    void on_btn_removePlugin_clicked();

    void on_btn_updateWordsList_clicked();

    void about();

    void quit();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_tree_chooseFile_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_fileSystemModel;
    QStringListModel *m_fullPathNamesModel;
    QStringList *m_fullPathNamesList;
    QString m_saveDirectory;
    QDir *m_dir;
    QTextEdit *m_textEdit;
    QTextDocument *m_textDocument;
    QTextDocument *m_textOriginal;
    QTextDocument *m_textObfuscated;
    QMenu *mainMenu;
    QMenu *helpMenu;
    QAction *quitAct;
    QAction *aboutAct;

    std::map<std::string,std::pair<std::string, std::string> > m_pluginDescription;
    std::map<std::string,std::pair<std::string, std::string> > m_pluginNameToFile;
    std::map<QString,QString> map_editFile;
    std::map<QString,QString>::iterator m_iter;
};
#endif // MAINWINDOW_H
